# SQLite 3.36.0 — Learning Summary

> Reading notes based on the SQLite amalgamation in
> `learning/sqlite/` (`sqlite3.c`, `sqlite3.h`, `sqlite3ext.h`).
>
> **How to use this document**
> - **New to databases?** Start with §0 (background concepts) before anything else.
> - **Know SQL but not SQLite internals?** Skim §0, then read §5 (architecture) and §6 (the VDBE).
> - **Here for the API?** Jump to §3, §4, and §11.5–§11.14.
> - **Looking for one topic in the code?** Use the navigation table in §15.

---

## 0. Background: database concepts you need first

SQLite is a **relational database management system (RDBMS)** implemented as a C library. Before diving into its source, it helps to have a mental model of how database engines are built in general. This section is the "prerequisite reading" — everything here is generic database theory, with notes on how SQLite does (or doesn't) follow the textbook approach.

### 0.1 What a database engine is

A database engine answers two core questions:

1. **What does the data mean?** — described by a *schema* (tables, columns, types, constraints).
2. **Where and how is the data stored?** — described by *file organization*, *access methods*, and *transactions*.

A full DBMS has roughly these responsibilities:

```
user / application
        │  SQL text or API calls
        ▼
┌──────────────────────────────────────────────┐
│  Query interface  (SQL parser, planner,      │
│                    optimizer, executor)      │
├──────────────────────────────────────────────┤
│  Transaction manager  (ACID guarantees)      │
├──────────────────────────────────────────────┤
│  Access methods  (B-tree, hash, heap)        │
├──────────────────────────────────────────────┤
│  Buffer / page manager  (cache, locking)     │
├──────────────────────────────────────────────┤
│  Disk / storage manager  (files, VFS, I/O)   │
└──────────────────────────────────────────────┘
```

SQLite implements **all** of these layers inside a single `.c` file. That vertical stack is exactly the layered architecture described in §5 — tokenizer → parser → code generator → VDBE → B-tree → pager → VFS.

### 0.2 Embedded vs. client–server

| | Client–server (PostgreSQL, MySQL) | Embedded (SQLite) |
|---|---|---|
| Process | Separate server daemon | Library linked into the app |
| Communication | TCP / socket protocol | Direct C function calls |
| Config | Users, roles, ports, tuning | None (zero-config) |
| Storage | Shared directory | One file per database |
| Concurrency | Many clients over network | One writer, many readers on one machine |
| Typical use | Web backends, analytics | Phones, browsers, desktops, embedded devices |

**Why this matters for reading the code**: SQLite has no network layer, no authentication subsystem, and no per-connection server thread. Everything happens in the caller's address space, which is why the API is a simple `open / prepare / step / finalize` sequence rather than a wire protocol.

### 0.3 The relational model in five minutes

- **Relation / table** — a set of rows with a common set of named columns.
- **Row / tuple** — one record; SQLite gives every row-of-a-normal-table a hidden 64-bit integer **rowid** (the primary key unless you declare one).
- **Column / attribute** — a named field; has an optional declared type and constraints (`NOT NULL`, `UNIQUE`, `CHECK`, `DEFAULT`).
- **Primary key** — a column (or set) that uniquely identifies a row.
- **Foreign key** — a column that references the primary key of another table; enforces referential integrity.
- **Index** — an auxiliary structure that speeds up lookups; SQLite implements every index as a separate B-tree.
- **View** — a named, stored query (no data of its own).
- **Trigger** — a program that runs automatically on `INSERT`/`UPDATE`/`DELETE`.
- **NULL** — "unknown"; SQL uses **three-valued logic** (`TRUE`, `FALSE`, `UNKNOWN`), so `NULL = NULL` is not true.

### 0.4 SQL: what the engine actually receives

SQL statements fall into a few groups:

| Group | Examples | What the engine does |
|-------|----------|----------------------|
| DDL (schema) | `CREATE TABLE`, `ALTER`, `DROP` | Modify the system catalog / schema |
| DML (data) | `SELECT`, `INSERT`, `UPDATE`, `DELETE` | Read or modify rows |
| TCL (transactions) | `BEGIN`, `COMMIT`, `ROLLBACK`, `SAVEPOINT` | Group statements atomically |
| DCL (access control) | `GRANT`, `REVOKE` | *Not supported by SQLite* |
| Other | `PRAGMA`, `ATTACH`, `VACUUM`, `ANALYZE`, `EXPLAIN` | Engine-specific commands |

Crucially, SQL is **declarative**: you describe *what* result you want, not *how* to compute it. Deciding "how" — which index to use, in what order to join tables, whether to sort — is the job of the **query planner/optimizer** (§0.9).

### 0.5 How a query becomes an answer (the classic pipeline)

Nearly every relational engine — and certainly SQLite — processes a query in these stages:

```
SQL text
   │  1. TOKENIZER      split into tokens (keywords, names, literals)
   ▼
tokens
   │  2. PARSER         check grammar, build a parse tree
   ▼
AST / parse tree
   │  3. ANALYZER / RESOLVER   bind names → real tables/columns/functions
   ▼
bound query
   │  4. OPTIMIZER      choose access paths, join order, algorithms
   ▼
query plan
   │  5. CODE GENERATOR  emit an executable program
   ▼
bytecode (VDBE program in SQLite)
   │  6. EXECUTOR / RUNTIME
   ▼
rows
```

SQLite maps each stage to real files: `tokenize.c` (1), the Lemon-generated parser (2), `resolve.c` (3), `where.c` (4), the `select/insert/update/delete` files (5), and `vdbe.c` (6).

### 0.6 Storage engines and pages

Databases do not read and write individual bytes; they move fixed-size **pages** (blocks). SQLite's default page size is 4096 bytes. Concepts:

- **Page** — the unit of I/O and caching. SQLite's page is the B-tree node and the pager's cache unit.
- **Slotted page** — a page layout with a header, a slot/pointer array, and a cell (record) region that grows from the end. SQLite uses exactly this (§11.1).
- **Heap file** — rows stored in no particular order. SQLite has no heap files; every table is a B-tree.
- **Index-organized table** — rows stored in key order inside a B-tree. **All** SQLite tables are index-organized by rowid.
- **Freelist** — a list of pages that have been freed and can be reused.

### 0.7 B-trees — the workhorse access method

A **B-tree** is a self-balancing search tree whose nodes are pages:

- Each node holds many keys (a **fanout** of hundreds), so the tree is shallow — a few levels cover millions of rows.
- Lookup, insert and delete are **O(log n)**.
- **B+tree** variant: all data lives in leaves, and leaves are linked — ideal for **range scans** (`WHERE x BETWEEN …`) and ordered traversal.

SQLite stores:

- Each **table** as a B+tree keyed by rowid (data in leaves).
- Each **index** as a B+tree keyed by the indexed column(s), with the rowid as the payload.

That is why `btree.c` is one of the largest files in the amalgamation — it is the heart of persistent storage.

### 0.8 Indexes and the read/write trade-off

An index is a redundant, sorted structure that lets the engine find rows without scanning the whole table.

- **Cost**: more disk space, and every `INSERT`/`UPDATE`/`DELETE` must also update the index.
- **Benefit**: turns a full scan (O(n)) into a lookup (O(log n)).
- **Composite index**: on multiple columns; column order matters.
- **Covering index**: contains every column a query needs, so the table itself is never touched.
- **Statistics**: the optimizer needs to know how selective an index is; `ANALYZE` collects this into `sqlite_stat1`/`sqlite_stat4`.

An index is chosen by the optimizer — not by the user — which is why query planning is such a big part of any engine.

### 0.9 Query optimization

The optimizer's job is to find an *equivalent but cheaper* plan. Typical decisions:

- Which index (if any) to use for each table.
- In what order to join tables.
- Which join algorithm to use (nested-loop, hash, merge). **SQLite only implements nested-loop joins.**
- Whether to sort or use an index that is already ordered.
- Whether to push a `LIMIT` down, use a covering index, or materialize a subquery.

Optimizers are usually **cost-based**: they estimate I/O and CPU from statistics and pick the cheapest plan. SQLite's planner lives in `where.c` (`sqlite3WhereBegin` / `sqlite3WhereEnd`) and can be inspected with `EXPLAIN QUERY PLAN`.

### 0.10 Transactions and ACID

A **transaction** groups several statements so they behave as one atomic unit. The four guarantees are **ACID**:

| Property | Meaning | SQLite mechanism |
|----------|---------|------------------|
| **Atomicity** | All-or-nothing | Rollback journal or WAL |
| **Consistency** | Constraints always hold | Constraint checks + triggers |
| **Isolation** | Concurrent transactions don't interfere | File locks + snapshots |
| **Durability** | Committed data survives a crash | `fsync` of journal + database |

Transactions are controlled with `BEGIN` / `COMMIT` / `ROLLBACK`, and nested units via `SAVEPOINT`. The `pager.c` layer implements all of this.

### 0.11 Concurrency control and isolation

Because multiple readers/writers may touch the same data, engines need **concurrency control**. Two broad strategies:

1. **Locking** — readers take shared locks, writers take exclusive locks. SQLite uses OS file locks (POSIX advisory locks or Windows `LockFileEx`) and its own lock states: UNLOCKED, SHARED, RESERVED, PENDING, EXCLUSIVE.
2. **MVCC (multi-version concurrency control)** — writers create new versions instead of overwriting, so readers see a consistent snapshot. SQLite approximates this in **WAL mode**: readers read the last committed snapshot while a writer appends to the `-wal` file.

**Isolation levels** (from weakest to strongest): Read Uncommitted → Read Committed → Repeatable Read → Serializable. SQLite effectively provides **serializable** isolation for the classic rollback-journal mode; WAL gives snapshot isolation for readers with a single writer.

### 0.12 Durability, journals and WAL

To make a commit crash-safe, an engine must be able to undo or redo partial writes. Two families:

- **Rollback (undo) journal** — before modifying a page, write the *old* image to a journal; on crash, replay the journal to restore. SQLite's default (`DELETE`/`PERSIST`/`TRUNCATE` journal modes).
- **Write-Ahead Log (WAL)** — append *new* page images to a log and only later move them into the main database. Readers read the main file plus the WAL; a **checkpoint** periodically folds the WAL back. Faster commits, better concurrency.

The directory/file conventions you will see:

```
app.db            the database
app.db-journal    rollback journal (delete mode)
app.db-wal        write-ahead log
app.db-shm        shared-memory index for the WAL
```

### 0.13 On-disk format and the system catalog

Every database needs a **catalog** (system tables) describing its own schema. SQLite keeps it in a table named **`sqlite_schema`** (historically `sqlite_master`) stored at the root of the file:

```sql
SELECT type, name, tbl_name, rootpage, sql FROM sqlite_schema;
```

Each row names an object (`table`, `index`, `view`, `trigger`) and, for tables/indexes, the **root page** of its B-tree. A **schema cookie** in the file header is bumped on every schema change so connections can detect staleness.

The physical file header is fixed and self-describing (magic string `"SQLite format 3\000"`, page size, encoding, etc.) — the full layout is detailed in §11.1.

### 0.14 Execution models: interpreters vs. compilers

Two ways to run a query plan:

- **Tree-walking interpreter** — walk the plan/AST directly. Simple, slower.
- **Compile to bytecode, run a virtual machine** — translate the plan into a compact instruction program, then execute it on a small VM. Faster (the plan is analyzed once), and it cleanly separates planning from execution.

SQLite takes the second route: it compiles SQL into **VDBE bytecode** (`opcodes.h`, ~150 opcodes) and executes it with a **register-based virtual machine** (`vdbe.c`). This is why the VDBE is described as the "heart" of SQLite in §6.

The **Volcano / iterator model** is the classic execution abstraction: each operator (`Scan`, `Filter`, `Join`, `Sort`, `Aggregate`) exposes `open() / next() / close()`, and a parent operator pulls rows from its children one at a time. SQLite's cursors and VDBE loops follow this pull-based pattern.

### 0.15 Data types: static vs. dynamic typing

Textbook SQL uses **static typing**: a column's declared type is enforced and values are converted on insert. SQLite uses **dynamic typing** with a twist:

- Each *value* has one of five **storage classes**: `INTEGER`, `REAL`, `TEXT`, `BLOB`, `NULL`.
- Each *column* has a **type affinity** that nudges values toward a preferred class, but SQLite will happily store any type in any column.

Details are in §11.3. The practical consequence is that `STRICT` tables (introduced later than 3.36.0) exist precisely to opt back into static typing.

### 0.16 What makes SQLite different (and worth studying)

- **Zero administration** — no server, no config files, no DBA.
- **One file** — a database is a single cross-platform file; copying it is a valid backup.
- **Fully embedded** — a few hundred KB linked into the host program.
- **Transactional even on a single file** — crash-safe without a separate server.
- **Extensible in-process** — user functions, collations, virtual tables, and loadable extensions all run in the caller's address space.

The trade-off is concurrency and scale: one writer at a time on one machine. That is the design point, not a bug — and understanding it explains many of the choices you will see in `pager.c`, `btree.c`, and the lock-state machine.

### 0.17 Reading checklist

With the above in mind, these are the concepts to keep handy while reading the source:

- [ ] Layers: parser → optimizer → code generator → VM → B-tree → pager → VFS
- [ ] Tables and indexes are both B-trees; rowid is the key of a table's B-tree
- [ ] Pages are the unit of I/O, caching, and locking
- [ ] ACID is delivered by the pager using a journal or WAL
- [ ] Concurrency is file-lock based; WAL adds snapshot reads
- [ ] The catalog is `sqlite_schema`; a schema cookie detects changes
- [ ] SQL is compiled to VDBE bytecode and executed on a register VM
- [ ] Types are dynamic: five storage classes, six affinities

---

## 1. What is in the directory

| File | Size | Lines | Role |
|------|------|-------|------|
| `sqlite3.h`      | ~586 KB | ~6 500 | Public C API — what application code `#include`s. Every exported symbol, constant, and result code is declared here, with extensive Doxygen-style comments (`CAPI3REF`, `METHOD`, `KEYWORDS`). |
| `sqlite3ext.h`  |  ~36 KB |  ~500  | Header used by **loadable extensions**. It declares the giant `sqlite3_api_routines` function-pointer table that SQLite fills in for extensions (kept append-only for ABI compatibility). |
| `sqlite3.c`     | ~8.5 MB | 235 517 | The **amalgamation** — every `.c` source file from the canonical source tree concatenated into one translation unit. The build banner reads `version 3.36.0`. |

This is the classic "single-file build" of SQLite. The amalgamation is what most projects compile directly, while the Fossil source tree keeps the same code split into ~100 files for editing.

The file's top declares:

```c
#define SQLITE_CORE 1
#define SQLITE_AMALGAMATION 1
#ifndef SQLITE_PRIVATE
# define SQLITE_PRIVATE static
#endif
```

`SQLITE_PRIVATE` makes everything that is not in the public header `static`, preventing internal symbols from leaking.

---

## 2. Version and source identity

```c
#define SQLITE_VERSION        "3.36.0"
#define SQLITE_VERSION_NUMBER 3036000
#define SQLITE_SOURCE_ID      "2021-06-18 18:36:39 5c9a6c06871cb9fe42814af9c039eb6da5427a6ec28f187af7ebfb62eafa66e5"
```

`SQLITE_SOURCE_ID` is a Fossil check-in identifier (date + SHA3-256 of the tree). If you build from an edited copy, the SHA3-256's last four hex characters change so consumers can detect a mismatched build. Three runtime helpers expose the same info for executables that don't see the macros:

```c
const char *sqlite3_libversion(void);
const char *sqlite3_sourceid(void);
int         sqlite3_libversion_number(void);
```

---

## 3. The C API surface (`sqlite3.h`)

The header is structured as a long sequence of documented sections, each beginning with a `CAPI3REF:` block comment. The high-level groups are:

### 3.1 Version and configuration
- `sqlite3_version`, `sqlite3_libversion*()`, `sqlite3_sourceid()`
- `sqlite3_threadsafe()` — reports the compile-time `SQLITE_THREADSAFE` value
- `sqlite3_compileoption_used()`, `sqlite3_compileoption_get()` — introspect build flags
- `sqlite3_config()` / `sqlite3_db_config()` — global / per-connection tuning (mutex mode, lookaside, page size, encoding, …)
- `sqlite3_initialize()` / `sqlite3_shutdown()` — explicit lifecycle for non-embedded hosts

### 3.2 Connections (`sqlite3`)
```c
int sqlite3_open (const char *filename, sqlite3 **ppDb);
int sqlite3_open16(const void *filename, sqlite3 **ppDb);
int sqlite3_open_v2(const char *filename, sqlite3 **ppDb,
                    int flags, const char *zVfs);
int sqlite3_close (sqlite3*);
int sqlite3_close_v2(sqlite3*);   /* leaves finalize to a background helper */
```

`sqlite3` is an **opaque** struct (`typedef struct sqlite3 sqlite3;`). Constructor / destructor pairs throughout the API follow this style:
- `sqlite3_open*` / `sqlite3_close*`
- `sqlite3_prepare*` / `sqlite3_finalize`
- `sqlite3_backup_init` / `sqlite3_backup_finish`

### 3.3 Compiling SQL — the prepare/step/finalize protocol
```c
int sqlite3_prepare_v2(sqlite3 *db, const char *zSql, int nByte,
                       sqlite3_stmt **ppStmt, const char **pzTail);
int sqlite3_step     (sqlite3_stmt*);
int sqlite3_reset    (sqlite3_stmt*);
int sqlite3_finalize (sqlite3_stmt*);
```
Plus the `sqlite3_exec()` convenience wrapper that runs one or more semicolon-separated statements and invokes a row callback.

The pattern: one prepare, many steps, one finalize.

### 3.4 Reading and binding values
- `sqlite3_bind_blob`/`_text`/`_int`/`_int64`/`_double`/`_null`/`_value`/`_pointer`/`_zeroblob` and the `_64`/`_16` variants
- `sqlite3_column_blob`/`_text`/`_int`/`_int64`/`_double`/`_bytes`/`_type`/`_value`/`_count`/`_name`/`_decltype`
- `sqlite3_value_*` mirror functions for raw values used inside user functions

### 3.5 User-defined functions, collations, modules
- `sqlite3_create_function` / `_v2` — scalar and aggregate callbacks (`xFunc`, `xStep`, `xFinal`)
- `sqlite3_create_window_function` (added recently for window functions)
- `sqlite3_create_collation` / `sqlite3_collation_needed`
- `sqlite3_create_module` — define virtual tables

### 3.6 Hooks (event callbacks)
- `sqlite3_commit_hook`, `sqlite3_rollback_hook`, `sqlite3_update_hook`
- `sqlite3_preupdate_hook` (gives access to old/new row in update triggers)
- `sqlite3_busy_handler`, `sqlite3_busy_timeout`
- `sqlite3_progress_handler`
- `sqlite3_authorizer` / `sqlite3_set_authorizer`
- `sqlite3_trace_v2` (events: STMT, PROFILE, ROW, CLOSE)

### 3.7 VFS and extensions
- `sqlite3_vfs_register` / `sqlite3_vfs_find` / `sqlite3_vfs_unregister`
- `sqlite3_load_extension` / `sqlite3_enable_load_extension`
- `sqlite3_auto_extension`

### 3.8 Backup and blob I/O
- `sqlite3_backup_init` / `sqlite3_backup_step` / `sqlite3_backup_finish` / `sqlite3_backup_remaining` / `sqlite3_backup_pagecount`
- `sqlite3_blob_open` / `sqlite3_blob_read` / `sqlite3_blob_write` / `sqlite3_blob_close`

---

## 4. Result codes

Two layers, defined in `sqlite3.h`:

```c
/* Primary codes (subset) */
#define SQLITE_OK           0
#define SQLITE_ERROR        1
#define SQLITE_INTERNAL     2
#define SQLITE_PERM         3
#define SQLITE_ABORT        4
#define SQLITE_BUSY         5
#define SQLITE_LOCKED       6
#define SQLITE_NOMEM        7
#define SQLITE_READONLY     8
#define SQLITE_INTERRUPT    9
#define SQLITE_IOERR       10
#define SQLITE_CORRUPT     11
#define SQLITE_NOTFOUND    12
#define SQLITE_FULL        13
#define SQLITE_CANTOPEN    14
#define SQLITE_PROTOCOL    15
#define SQLITE_EMPTY       16
#define SQLITE_SCHEMA      17
#define SQLITE_TOOBIG      18
#define SQLITE_CONSTRAINT  19
#define SQLITE_MISMATCH    20
#define SQLITE_MISUSE      21
#define SQLITE_NOLFS       22
#define SQLITE_AUTH        23
#define SQLITE_FORMAT      24
#define SQLITE_RANGE       25
#define SQLITE_NOTADB      26
#define SQLITE_NOTICE      27
#define SQLITE_WARNING     28
#define SQLITE_ROW       100   /* sqlite3_step has a row */
#define SQLITE_DONE      101   /* sqlite3_step is done  */
```

Extended codes are formed as `(primary | (subcode << 8))`, e.g. `SQLITE_IOERR_READ = SQLITE_IOERR | (1<<8)`. They can be turned on per connection with `sqlite3_extended_result_codes()`.

---

## 5. Internal architecture

SQLite is built as a strict **layered pipeline**. From top to bottom in `sqlite3.c`:

```
┌─────────────────────────────────────────────────────────┐
│ Public C API  (sqlite3_open / prepare / step / finalize) │
└─────────────────────────┬───────────────────────────────┘
                          │
   ┌──────────────────────┼───────────────────────────┐
   │                      │                           │
┌──▼─────────┐   ┌────────▼────────┐    ┌────────────▼────────┐
│ Tokenizer  │──▶│   Parser (LALR) │──▶ │ Code Generator (VDBE │
│ tokenize.c │   │ Lemon-generated │    │  program emission)  │
└─────────────┘   └─────────────────┘    └──────────┬─────────┘
                                                   │
                       ┌───────────────────────────▼──────────┐
                       │  WHERE analyzer  (where.c/whereexpr.c│
                       │  + whereInt.h) — query optimizer     │
                       └─────────────────────┬────────────────┘
                                             │
                       ┌─────────────────────▼───────────────┐
                       │ VDBE  (vdbe.c / vdbeapi.c / vdbeaux)│
                       │ Register-based virtual machine      │
                       └────────────┬───────────────┬────────┘
                                    │               │
                       ┌────────────▼────┐ ┌────────▼─────────┐
                       │  B-Tree (btree.c)│ │   Sort/Merge     │
                       │                 │ │ (vdbesort.c)     │
                       └────────┬────────┘ └──────────────────┘
                                │
                       ┌────────▼────────┐
                       │     Pager       │  — page-level cache,
                       │   (pager.c)     │    WAL/journal, locking
                       └────────┬────────┘
                                │
                       ┌────────▼────────┐
                       │   VFS (os_unix.c│   virtual file system;
                       │   / os_win.c)   │   everything OS-specific
                       └─────────────────┘
```

Cross-cutting modules live alongside:

- Memory: `mem0.c` + `mem1.c` / `mem2.c` / `mem3.c` / `mem5.c` (different allocator strategies)
- Mutex: `mutex_noop.c`, `mutex_unix.c`, `mutex_w32.c`
- Utilities: `util.c`, `hash.c`, `printf.c`, `utf.c`, `date.c`, `random.c`, `treeview.c`
- Extension points: `json1.c`, `fts3*.c`, `fts5*.c`, `rtree*.c`, `sqlite3session.c`, `sqlite3rbu.c`
- Page cache: `pcache.c` / `pcache.h`

### 5.1 Walk through the .c files (line numbers from this amalgamation)

| Line  | File | What it does |
|------:|------|--------------|
|  812  | `sqliteInt.h`     | The single internal master header. Every internal subsystem `#include`s this. |
| 13450 | `sqliteLimit.h`   | Compile-time resource caps (`SQLITE_MAX_LENGTH`, `SQLITE_MAX_COLUMN`, …). |
| 14051 | `hash.h`          | String/byte hash table used everywhere. |
| 14152 | `parse.h`         | Symbols emitted by the Lemon parser generator. |
| 14981 | `pager.h`         | Page-level cache + journal API. |
| 15228 | `btree.h`         | B-tree cursors, seeks, put/delete. |
| 15641 | `vdbe.h`          | VDBE program header: `Vdbe`, `VdbeOp`, `Mem` (memory cell). |
| 15811 | `opcodes.h`       | **Auto-generated opcode table** (150+ opcodes, see §6). |
| 16271 | `pcache.h`        | Page-cache interface (pluggable strategy). |
| 16466 | `os.h` / `os_setup.h` | OS abstraction setup. |
| 16750 | `mutex.h`         | Mutex interface. |
| 20794 | `global.c`        | Process-wide state. |
| 21146 | `status.c`        | `sqlite3_status_*` runtime counters. |
| 21164 | `vdbeInt.h`       | VDBE internal types and helpers. |
| 23425 | `os.c`            | Common OS interface glue. |
| 23868 | `fault.c`         | `sqlite3_test_control(SIMPLEFAULT, …)`. |
| 23958+ | `mem*.c`          | Memory allocator strategies. |
| 26114 | `mutex.c`         | Mutex core. |
| 26478+ | `mutex_unix.c` / `mutex_w32.c` | OS-specific mutex implementations. |
| 27785 | `malloc.c`        | `sqlite3_malloc`, `sqlite3_realloc`, `sqlite3_free`. |
| 28615 | `printf.c`        | SQLite's own `printf` family. |
| 30724 | `random.c`        | `sqlite3_randomness`. |
| 31143 | `utf.c`           | UTF-8/UTF-16 conversion. |
| 31681 | `util.c`          | Misc helpers (`sqlite3_strglob`, `sqlite3_getdigitcount`, …). |
| 33389 | `hash.c`          | Hash table implementation. |
| 33661 | `opcodes.c`       | Names + help text for every opcode. |
| 33860 | `os_unix.c`       | VFS for POSIX (file, mmap, flock/posix advisory locks, …). |
| 42165 | `os_win.c`        | VFS for Windows. |
| 48520 | `memdb.c`         | In-memory VFS (`file::memory:`). |
| 52484 | `pager.c`         | Pager implementation. |
| 65452 | `btree.c`         | B-tree implementation. |
| 76223 | `backup.c`        | Online backup (file-to-file copy). |
| 76997 | `vdbemem.c`       | `Mem` cell (register) management. |
| 78909 | `vdbeaux.c`       | VDBE helpers: emit, list, resolve labels. |
| 84157 | `vdbeapi.c`       | Public functions over `sqlite3_stmt` (`sqlite3_bind_*`, `sqlite3_column_*`, …). |
| 86214 | `vdbetrace.c`     | Tracing/debugging printers. |
| 86411 | `vdbe.c`          | The VDBE interpreter. |
| 94805 | `vdbeblob.c`      | Incremental BLOB read/write. |
| 95317 | `vdbesort.c`      | External merge sorter (`Sorter` object). |
| 98084 | `vdbevtab.c`      | Virtual-table cursor primitives. |
| 99210 | `resolve.c`       | Name resolution (columns, tables, functions). |
| 101227| `expr.c`          | Expression tree construction and analysis. |
| 109597| `analyze.c`       | `ANALYZE` command — builds `sqlite_stat1/4`. |
| 111543| `attach.c`        | `ATTACH DATABASE`. |
| 112143| `auth.c`          | Authorizer callback. |
| 117801| `callback.c`      | `sqlite3_exec` wrapper. |
| 118340| `delete.c`        | DELETE code generation. |
| 119314| `func.c`          | Built-in SQL functions (math, string, …). |
| 121603| `fkey.c`          | Foreign-key implementation. |
| 123058| `insert.c`        | INSERT code generation. |
| 126267| `loadext.c`       | Extension loading. |
| 127801| `pragma.c`        | `PRAGMA` command implementation. |
| 131049| `prepare.c`       | Tokenize + parse + code-generate. |
| 132071| `select.c`        | SELECT (and compound selects) generation. |
| 139471| `table.c`         | `CREATE TABLE` / `DROP TABLE` / … |
| 139672| `trigger.c`       | Triggers. |
| 141093| `update.c`        | UPDATE generation. |
| 142742| `vacuum.c`        | `VACUUM` / incremental vacuum. |
| 144472| `wherecode.c`     | Generates code for a chosen WHERE plan. |
| 147601| `whereexpr.c`     | WHERE expression analysis. |
| 149227| `where.c`         | Query planner: best-index search (`sqlite3WhereBegin/End`). |
| 154911| `window.c`        | SQL window functions (`OVER`). |
| 163338| `tokenize.c`      | SQL tokenizer. |
| 165000+| `fts3*.c`, `fts5*.c` | Full-text search modules. |
| 170000+| `rtree*.c`        | R-Tree index module. |
| 189634| `json1.c`         | JSON1 extension. |
| 164985| `main.c`          | Database connection lifecycle (`sqlite3_open`, `sqlite3_close`, …). |
| 98511 | `memjournal.c`    | In-memory journal for small transactions. |

---

## 6. The VDBE — SQLite's virtual machine

The single most interesting piece in the amalgamation is `vdbe.c`. It executes a small register-based bytecode program.

### 6.1 Opcodes (excerpt from `opcodes.h`)

Categories visible in the file:

- **Control flow** — `OP_Goto`, `OP_Gosub`, `OP_Return`, `OP_Init`, `OP_Once`, `OP_If`, `OP_IfNot`, `OP_ElseEq`, `OP_IfNullRow`, `OP_Halt`, `OP_HaltIfNull`, `OP_Program`, `OP_Jump`, `OP_Yield`, `OP_EndCoroutine`, `OP_InitCoroutine`
- **Comparisons** — `OP_Eq`, `OP_Ne`, `OP_Lt`, `OP_Le`, `OP_Gt`, `OP_Ge`, `OP_IsTrue`, `OP_IsNull`, `OP_NotNull`, `OP_And`, `OP_Or`, `OP_Not`
- **Cursor seeks** — `OP_SeekLT`, `OP_SeekLE`, `OP_SeekGE`, `OP_SeekGT`, `OP_IfNoHope`, `OP_NoConflict`, `OP_NotFound`, `OP_Found`, `OP_SeekRowid`, `OP_NotExists`, `OP_Last`, `OP_Rewind`, `OP_Next`, `OP_Prev`, `OP_IfNotOpen`
- **Index probes** — `OP_IdxLE`, `OP_IdxGT`, `OP_IdxLT`, `OP_IdxGE`, `OP_RowSetRead`, `OP_RowSetTest`
- **Records & values** — `OP_Column`, `OP_MakeRecord`, `OP_Affinity`, `OP_Cast`, `OP_RealAffinity`, `OP_Compare`, `OP_Permutation`, `OP_Offset`
- **Constants** — `OP_Integer`, `OP_Int64`, `OP_String`, `OP_Null`, `OP_SoftNull`, `OP_Blob`, `OP_Variable`, `OP_ChngCntRow`
- **Register ops** — `OP_Move`, `OP_Copy`, `OP_SCopy`, `OP_IntCopy`, `OP_AddImm`, `OP_ZeroOrNull`
- **Aggregation / sorting** — `OP_Count`, `OP_Sort`, `OP_SorterSort`, `OP_SorterNext`, `OP_CollSeq`
- **Functions** — `OP_Function`, `OP_PureFunc`
- **Output** — `OP_ResultRow`, `OP_Return`
- **Schema & transactions** — `OP_Transaction`, `OP_AutoCommit`, `OP_Savepoint`, `OP_Checkpoint`, `OP_Vacuum`, `OP_IncrVacuum`, `OP_ReadCookie`, `OP_SetCookie`, `OP_JournalMode`, `OP_PageCount`, `OP_MaxPgcnt`, `OP_Update`, `OP_Insert`, `OP_Delete`, `OP_ResetCount`
- **Virtual tables** — `OP_VOpen`, `OP_VFilter`, `OP_VColumn`, `OP_VNext`, `OP_VUpdate`, `OP_VRename`
- **FTS / misc** — `OP_FkIfZero`, `OP_FkCheck`, `OP_IfPos`, `OP_IfNotZero`, `OP_DecrJumpZero`, `OP_IfSmaller`

Each opcode is documented with a one-line synopsis (e.g. `/* jump, synopsis: key=r[P3@P4] */`). The pattern is `r[P1@P2] = …` — registers, with optional count.

### 6.2 The interpreter loop
`vdbe.c::sqlite3VdbeExec()` is a giant `for(;;)` switch on `pOp->opcode`. Each case consumes its operands (P1..P5) from the `VdbeOp` and operates on `aMem[]` registers and a `VdbeCursor[]` array of B-tree cursors. Branch opcodes simply adjust `pc`.

### 6.3 Memory cells (`vdbemem.c`)
A register is a tagged union — `Mem` holds an integer, real, text, blob, or NULL plus an affinity flag. `Mem` is the same struct used for `sqlite3_value` and `sqlite3_context` results, which is why the API is so uniform.

---

## 7. Query processing pipeline

```c
sqlite3_prepare_v2()  ──▶  sqlite3Prepare()       (prepare.c)
                          ├─ sqlite3RunParser()   (uses Lemon parser in parse.y,
                          │                        compiled into sqlite3.c)
                          ├─ sqlite3ExprAnalyze() (whereexpr.c)
                          └─ Code-gen for the chosen WHERE layout (wherecode.c)
sqlite3_step()          ──▶  sqlite3VdbeExec()     (vdbe.c) loops until SQLITE_ROW/SQLITE_DONE.
sqlite3_finalize()      ──▶  free Vdbe, cursors, registers.
```

`sqlite3_open_v2()` itself is a 7-line wrapper that calls `openDatabase()` in `main.c`, which in turn:

1. Calls `sqlite3Malloc` for `sqlite3`.
2. Selects a VFS (named, or default).
3. Sets up mutex, lookaside, error codes.
4. Calls `sqlite3BtreeOpen` to open the B-tree.
5. Calls `sqlite3HeaderSizeBtree` to initialize file format.

`sqlite3_close()` → `sqlite3Close()` (in `main.c` line ~166 362) waits for the journal to flush, then frees all `sqlite3_stmt`s, all `sqlite3_backup`s, every schema (`sqlite3SchemaRelease`), the lookaside, the mutex, and finally `sqlite3_free(db)`.

---

## 8. Storage layer in detail

### 8.1 B-tree (`btree.c`)
SQLite stores every table and every index as a separate B-tree, identified by root page numbers in `sqlite_schema`. There is no heap-organized table — table B-trees use a 64-bit integer "rowid" as key. Index B-trees use a serialized key. Operations: open, close, begin transaction, cursor: first/last/next/prev/seek/insert/delete.

### 8.2 Pager (`pager.c`)
The pager owns a fixed-size page cache, a journal file, and (optionally) a WAL file. Each `sqlite3Btree` has one pager per file. The pager exposes page reference counts, savepoints, transactions, checkpoints, and locking. It also implements the locking model — POSIX advisory or Windows `LockFileEx` depending on the VFS.

### 8.3 Page cache (`pcache.h`)
A separate page cache is layered on top of the pager so that multiple pagers can share memory through `SQLITE_CONFIG_PAGECACHE`.

### 8.4 VFS (`os_unix.c`, `os_win.c`)
The VFS is a struct of function pointers — `xOpen`, `xClose`, `xRead`, `xWrite`, `xTruncate`, `xSync`, `xFileSize`, `xLock`, `xUnlock`, `xFileControl`, `xSectorSize`, `xDeviceCharacteristics`, plus `xOpen`/`xDelete`/`xAccess`/`xFullPathname`/`xRandomness`/`xSleep`/`xGetLastError`/`xCurrentTime`/`xGetSystemTime`/`xCurrentTimeInt64` at the parent level.

You can substitute your own VFS to:

- run SQLite on top of a custom storage device,
- add tracing / fault injection,
- encrypt pages transparently,
- restrict paths.

`sqlite3_open_v2(filename, ppDb, flags, "winname")` selects the VFS at open time.

---

## 9. Memory management

Four built-in strategies behind `sqlite3_malloc`:

| File | Strategy |
|------|----------|
| `mem1.c` | Wraps the system `malloc`/`free` with optional scratch buffers. Default. |
| `mem2.c` | Allocator with debug headers for leak / overrun detection. |
| `mem3.c` | Lookaside slab allocator. |
| `mem5.c` | Another debug-aware allocator. |

Plus `lookaside` slabs (per-connection `sqlite3_db_config(SQLITE_DBCONFIG_LOOKASIDE, …)`) that reduce malloc churn.

All allocations go through `sqlite3Malloc`/`sqlite3_free`, which in `OOM-safe` code paths are checked; on `NULL` they call `db->mallocFailed = 1`, which the public API turns into `SQLITE_NOMEM`.

---

## 10. Concurrency

- Default `SQLITE_THREADSAFE=1` (serialized): a single mutex per connection.
- `SQLITE_CONFIG_MULTITHREAD`: per-connection mutex but no per-statement mutex — you may not share a connection across threads.
- `SQLITE_CONFIG_SINGLETHREAD`: no mutexes.
- WAL mode (`PRAGMA journal_mode=WAL`) lets readers not block writers and vice versa, with per-process file locking.

---

## 11. Compile-time knobs (a tiny sample)

`ctime.c` reports every compile option:

```
SQLITE_THREADSAFE
SQLITE_ENABLE_FTS3 / FTS4 / FTS5
SQLITE_ENABLE_RTREE
SQLITE_ENABLE_JSON1
SQLITE_OMIT_FOREIGN_KEY
SQLITE_OMIT_COMPILEOPTION_DIAGS
SQLITE_DEFAULT_CKPTFULLFSYNC
SQLITE_DQS
SQLITE_ENABLE_COLUMN_METADATA
SQLITE_ENABLE_UPDATE_DELETE_LIMIT
…
```

Removing options shrinks the binary drastically — that's why "amalgamation" projects often redefine `SQLITE_OMIT_*` macros before `#include "sqlite3.c"`.

---

## 11.1 On-disk file format

Every SQLite database is one file. Page 1 contains a 100-byte file header that doubles as a magic string:

```c
#define SQLITE_FILE_HEADER "SQLite format 3"   /* sqlite3.c line ~64675 */
```

The full layout (comment block at line 64480):

| Offset | Size | Field |
|-------:|-----:|-------|
|  0 | 16 | `"SQLite format 3\000"` magic |
| 16 |  2 | Page size in bytes (`1` ⇒ 65536) |
| 18 |  1 | File format write version |
| 19 |  1 | File format read version |
| 20 |  1 | Reserved bytes at end of each page |
| 21 |  1 | Max embedded payload fraction (default 64) |
| 22 |  1 | Min embedded payload fraction (default 32) |
| 23 |  1 | Min leaf payload fraction (default 32) |
| 24 |  4 | File change counter (increment on commit) |
| 28 |  4 | Reserved |
| 32 |  4 | First freelist trunk page |
| 36 |  4 | Freelist page count |
| 40 | 60 | 15 × 4-byte meta values (schema cookie, page cache size, text encoding, user version, incr-vacuum mode, application-ID, `SQLITE_VERSION_NUMBER`, …) |

All integers are **big-endian**.

A B-tree page (after the file header) is split into three regions:

```
+------------------+
| file header (100)|   page 1 only
+------------------+
| page header (8+4)|   flags, freeblock ptr, cell count, ...
+------------------+
| cell ptr array   |   sorted offsets, grows downward
|       v          |
+------------------+
| unallocated space|
+------------------+
|       ^          |   grows upward
| cell content area|   freeblocks + cells
+------------------+
```

**Take-away**: knowing this layout is enough to write tools that scan the file directly without using SQLite (recovery, forensic, custom importers).

## 11.2 Journal modes (`PAGER_JOURNALMODE_*`)

The pager can commit a transaction several ways (constants in `pager.h`, line ~15058):

| Value | Name | Behaviour |
|------:|------|-----------|
|  0 | `DELETE`     | Rollback journal file is deleted at commit (default). |
|  1 | `PERSIST`    | Journal header is zeroed but the file is kept. |
|  2 | `OFF`        | No journal at all (no atomicity!). |
|  3 | `TRUNCATE`   | Journal file is truncated at commit. |
|  4 | `MEMORY`     | Journal is a memory-only ring buffer (`memjournal.c`). |
|  5 | `WAL`        | Write-Ahead Log: readers do not block writers and vice versa. |

Setting via SQL: `PRAGMA journal_mode = WAL;`. Internally the pager exposes this through `OP_JournalMode`.

## 11.3 Type system: storage classes vs type affinity

SQLite is **dynamically typed**. Each stored value has one of five *storage classes* (declared at `sqlite3.h` line 4789):

```c
#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_TEXT     3   /* (was the v2 name; redefined here) */
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
```

Column declarations carry a *type affinity* (one of six, declared at `sqlite3.c` line 17673):

```c
#define SQLITE_AFF_NONE     0x40  /* '@' */
#define SQLITE_AFF_BLOB     0x41  /* 'A' */
#define SQLITE_AFF_TEXT     0x42  /* 'B' */
#define SQLITE_AFF_NUMERIC  0x43  /* 'C' */
#define SQLITE_AFF_INTEGER  0x44  /* 'D' */
#define SQLITE_AFF_REAL     0x45  /* 'E' */
#define SQLITE_AFF_MASK     0x47
```

When a value is inserted, the `OP_Affinity` and `OP_Cast` opcodes coerce it toward the column's affinity. The VDBE works in these six affinities, which is why the magic characters are `'A'`–`'E'` — they show up in `EXPLAIN` listings.

## 11.4 Lookaside memory (`LuaCache`, `malloc.c`, `mem3.c`)

Every connection keeps a slab of pre-allocated buffers (`Lookaside` struct, line 16977):

```
struct Lookaside {
  u32 bDisable;
  u32 nSlot;
  LookasideSlot *pInit;
  LookasideSlot *pFree;
  /* two-size lookaside (default since 3.31) */
  LookasideSlot *pSmallInit;
  LookasideSlot *pSmallFree;
};
```

Default is **100 slots of 1200 bytes** plus 100 slots of a smaller size for short-lived small objects (`SQLITE_OMIT_TWOSIZE_LOOKASIDE` removes this). Tunable via:

- Global: `sqlite3_config(SQLITE_CONFIG_LOOKASIDE, slotSize, nSlot)`
- Per connection: `sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, buf, slotSize, nSlot)`

Counter-introspection through `sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_*)`:

```
SQLITE_DBSTATUS_LOOKASIDE_USED
SQLITE_DBSTATUS_LOOKASIDE_HIT
SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE
SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL
```

In shared-cache mode the lookaside is disabled while reading schema, to avoid duplicating it per connection.

## 11.5 `sqlite3_open_v2` flag bits

`sqlite3.h` line 554:

```c
#define SQLITE_OPEN_READONLY         0x00000001   /* OK for sqlite3_open_v2 */
#define SQLITE_OPEN_READWRITE        0x00000002
#define SQLITE_OPEN_CREATE           0x00000004
#define SQLITE_OPEN_DELETEONCLOSE    0x00000008   /* VFS-only */
#define SQLITE_OPEN_EXCLUSIVE        0x00000010   /* VFS-only */
#define SQLITE_OPEN_AUTOPROXY        0x00000020   /* VFS-only */
#define SQLITE_OPEN_URI              0x00000040   /* enables file:...?mode=ro */
#define SQLITE_OPEN_MEMORY           0x00000080   /* ":memory:" */
#define SQLITE_OPEN_MAIN_DB          0x00000100   /* VFS-only */
#define SQLITE_OPEN_TEMP_DB          0x00000200   /* VFS-only */
#define SQLITE_OPEN_TRANSIENT_DB     0x00000400   /* VFS-only */
#define SQLITE_OPEN_MAIN_JOURNAL     0x00000800   /* VFS-only */
#define SQLITE_OPEN_TEMP_JOURNAL     0x00001000   /* VFS-only */
#define SQLITE_OPEN_SUBJOURNAL       0x00002000   /* VFS-only */
#define SQLITE_OPEN_SUPER_JOURNAL    0x00004000   /* VFS-only */
#define SQLITE_OPEN_NOMUTEX          0x00008000
#define SQLITE_OPEN_FULLMUTEX        0x00010000
#define SQLITE_OPEN_SHAREDCACHE      0x00020000
#define SQLITE_OPEN_PRIVATECACHE     0x00040000
#define SQLITE_OPEN_WAL              0x00080000   /* VFS-only */
#define SQLITE_OPEN_NOFOLLOW         0x01000000
```

Common combinations: `sqlite3_open_v2("file::memory:?cache=shared", …, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY, NULL)`.

## 11.6 `sqlite3_config()` verbs (global)

| Verb | Args | Effect |
|------|------|--------|
| `SQLITE_CONFIG_SINGLETHREAD` `(1)` | – | Disable all mutexing. |
| `SQLITE_CONFIG_MULTITHREAD` `(2)` | – | Per-connection mutex only (no statement mutex). |
| `SQLITE_CONFIG_SERIALIZED` `(3)` | – | Default for `SQLITE_THREADSAFE=1`. |
| `SQLITE_CONFIG_MALLOC` `(4)` | `sqlite3_mem_methods*` | Install a custom allocator. |
| `SQLITE_CONFIG_GETMALLOC` `(5)` | `sqlite3_mem_methods*` | Read the current allocator. |
| `SQLITE_CONFIG_SMALL_MALLOC` `(6)` | bool | Drop allocations <8 bytes. |
| `SQLITE_CONFIG_PAGECACHE` `(7)` | `void*, int sz, int N` | Static page-cache memory pool. |
| `SQLITE_CONFIG_HEAP` `(8)` | `void*, int nByte, int min` | Bounded heap region (back-end). |
| `SQLITE_CONFIG_MEMSTATUS` `(9)` | bool | Toggle memory-status accounting. |
| `SQLITE_CONFIG_MUTEX` `(10)` | `sqlite3_mutex_methods*` | Custom mutex implementation. |
| `SQLITE_CONFIG_GETMUTEX` `(11)` | `sqlite3_mutex_methods*` | Read it back. |
| `SQLITE_CONFIG_LOOKASIDE` `(13)` | int, int | Default lookaside size. |
| `SQLITE_CONFIG_PCACHE2` `(18)` | `sqlite3_pcache_methods2*` | Custom page-cache. |
| `SQLITE_CONFIG_URI` `(17)` | int | Enable URI filenames globally. |

## 11.7 `sqlite3_db_config()` verbs (per-connection)

| Verb | Meaning |
|------|---------|
| `SQLITE_DBCONFIG_LOOKASIDE` (1001) | Replace lookaside buffers. |
| `SQLITE_DBCONFIG_ENABLE_FKEY` | Toggle FK enforcement (default on). |
| `SQLITE_DBCONFIG_ENABLE_TRIGGER` | Toggle triggers. |
| `SQLITE_DBCONFIG_ENABLE_VIEW` | Toggle views. |
| `SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER` | Toggle two-arg `fts3_tokenizer()`. |
| `SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION` | Toggle `sqlite3_load_extension`. |
| `SQLITE_DBCONFIG_MAINDBNAME` | Rename the default schema. |
| `SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE` | Skip WAL checkpoint on detach. |
| `SQLITE_DBCONFIG_ENABLE_QPSG` | Query Planner Stability Guarantee. |
| `SQLITE_DBCONFIG_TRIGGER_EQP` | Include trigger ops in EXPLAIN QUERY PLAN. |
| `SQLITE_DBCONFIG_RESET_DATABASE` | Reset on next VACUUM (irreversible). |
| `SQLITE_DBCONFIG_DEFENSIVE` | Block SQL that looks like an injection attack. |
| `SQLITE_DBCONFIG_WRITABLE_SCHEMA` | Mirror `PRAGMA writable_schema`. |
| `SQLITE_DBCONFIG_LEGACY_ALTER_TABLE` | Pre-3.24 `ALTER TABLE RENAME` semantics. |

## 11.8 `sqlite3_limit()` categories

```c
int sqlite3_limit(sqlite3*, int id, int newVal);
```

| Id | Name | Default | Meaning |
|---:|------|--------:|---------|
| 0 | `SQLITE_LIMIT_LENGTH` | 1 MB | Max string/BLOB length. |
| 1 | `SQLITE_LIMIT_SQL_LENGTH` | 1 MB | Max text of one SQL statement. |
| 2 | `SQLITE_LIMIT_COLUMN` | 2 000 | Columns in a table or SELECT result. |
| 3 | `SQLITE_LIMIT_EXPR_DEPTH` | 1 000 | Recursion depth in expressions. |
| 4 | `SQLITE_LIMIT_COMPOUND_SELECT` | 500 | Terms in a compound SELECT. |
| 5 | `SQLITE_LIMIT_VDBE_OP` | 250 M | VDBE opcodes per statement. |
| 6 | `SQLITE_LIMIT_FUNCTION_ARG` | 127 | Args to a user function. |
| 7 | `SQLITE_LIMIT_ATTACHED` | 10 | Attached databases. |
| 8 | `SQLITE_LIMIT_LIKE_PATTERN_LENGTH` | 50 000 | LIKE/GLOB pattern. |
| 9 | `SQLITE_LIMIT_VARIABLE_NUMBER` | 32 766 | Bound parameters per statement. |
| 10 | `SQLITE_LIMIT_TRIGGER_DEPTH` | 1 000 | Trigger recursion. |
| 11 | `SQLITE_LIMIT_WORKER_THREADS` | 0 | Reserved for `sqlite3_malloc64`? (off by default) |

Setting `-1` returns the current value without changing it.

## 11.9 File-control opcodes (`SQLITE_FCNTL_*`)

`sqlite3_file_control(db, zDbName, op, pArg)` lets applications (and the VFS itself) send private commands down to a `sqlite3_file`. Major opcodes:

| Opcode | Purpose |
|--------|---------|
| `SQLITE_FCNTL_LOCKSTATE` | Debug: report OS lock state. |
| `SQLITE_FCNTL_SIZE_HINT` | Hint: expected file growth. |
| `SQLITE_FCNTL_SIZE_LIMIT` | Cap file size (in-memory VFS). |
| `SQLITE_FCNTL_CHUNK_SIZE` | Adjust the chunked file growth step. |
| `SQLITE_FCNTL_FILE_POINTER` | Get the `sqlite3_file*` for the DB. |
| `SQLITE_FCNTL_JOURNAL_POINTER` | Get the `sqlite3_file*` for the journal. |
| `SQLITE_FCNTL_SYNC_OMITTED` / `SQLITE_FCNTL_SYNC` | Internal: ask the VFS to flush. |
| `SQLITE_FCNTL_COMMIT_PHASETWO` | Internal: second commit phase. |
| `SQLITE_FCNTL_WIN32_AV_RETRY` | Windows AV-retry counter. |
| `SQLITE_FCNTL_PERSIST_WAL` | Keep `-wal` file after the last close. |
| `SQLITE_FCNTL_POWERSAFE_OVERWRITE` | fsync semantics on macOS / BSD. |
| `SQLITE_FCNTL_OVERWRITE` | Hint that file will be fully overwritten. |
| `SQLITE_FCNTL_VFSNAME` | Retrieve the VFS name for a handle. |
| `SQLITE_FCNTL_VFS_POINTER` | Retrieve the VFS struct pointer. |
| `SQLITE_FCNTL_PRAGMA` | Hook for custom PRAGMA implementations in the VFS. |
| `SQLITE_FCNTL_BUSYHANDLER` | Plug a custom busy handler. |
| `SQLITE_FCNTL_TEMPFILENAME` | Ask the VFS for a unique temp path. |
| `SQLITE_FCNTL_MMAP_SIZE` | Tune mmap I/O size. |
| `SQLITE_FCNTL_TRACE` | Notification trace. |
| `SQLITE_FCNTL_HAS_MOVED` | Did the file move (rename) under us? |
| `SQLITE_FCNTL_WIN32_GET_HANDLE` / `..._SET_HANDLE` | Windows HANDLE bridge. |
| `SQLITE_FCNTL_WAL_BLOCK` | Tell WAL VFS that we may block. |
| `SQLITE_FCNTL_ZIPVFS` | Compression-extension entry point. |
| `SQLITE_FCNTL_RBU` | RBU update entry point. |
| `SQLITE_FCNTL_BEGIN_ATOMIC_WRITE` / `_COMMIT_…` / `_ROLLBACK_…` | Atomic-batch write protocol (for filesystems that support it). |

## 11.10 Memory pressure and shutdown

```c
int        sqlite3_release_memory(int N);              /* try to free N bytes */
sqlite3_int64 sqlite3_soft_heap_limit64(sqlite3_int64); /* aspirational cap */
sqlite3_int64 sqlite3_hard_heap_limit64(sqlite3_int64); /* hard ceiling */
void       sqlite3_soft_heap_limit(int);               /* deprecated */
```

`sqlite3_release_memory(N)` asks the pager and other caches to drop pages; returns the actual number of bytes freed. `sqlite3_soft_heap_limit64(N)` causes the allocator to start returning `SQLITE_NOMEM` once persistent usage grows past `N`, but transient peaks are allowed. `sqlite3_hard_heap_limit64` is an upper bound that even single allocations cannot exceed.

`sqlite3_shutdown()` undoes `sqlite3_initialize()`. The library is auto-initialized on first use and auto-shuts down at process exit (`atexit`), but you can drive it manually for embedded hosts.

## 11.11 `sqlite3_log()` and error logging

```c
SQLITE_API void sqlite3_log(int iErrCode, const char *zFormat, ...);
```

- Sends a printf-style message to the global logger.
- The default logger writes to `stderr` for `SQLITE_WARNING` / `SQLITE_NOTICE`.
- Set your own logger with `sqlite3_config(SQLITE_CONFIG_LOG, myLogCb, myArg)`.
- Inside user-defined functions, prefer `sqlite3_result_error(...)` so errors flow back through the result code.

## 11.12 Text encoding (`SQLITE_UTF*`)

`sqlite3_create_function`, `sqlite3_create_collation`, `sqlite3_bind_text16`, etc. take an encoding:

```c
#define SQLITE_UTF8           1
#define SQLITE_UTF16LE        2
#define SQLITE_UTF16BE        3
#define SQLITE_UTF16          4   /* native byte order */
#define SQLITE_UTF16_ALIGNED  8   /* sqlite3_create_collation only */
```

The DB itself stores one of UTF-8, UTF-16le, or UTF-16be (file-header meta #4). Mismatch on a CREATE/ATTACH causes `SQLITE_MISMATCH`.

## 11.13 Subtypes (`sqlite3_value_subtype`, `sqlite3_result_subtype`)

```c
SQLITE_API unsigned int sqlite3_value_subtype  (sqlite3_value*);
SQLITE_API void          sqlite3_result_subtype(sqlite3_context*, unsigned int);
```

A user function can attach an opaque 16-bit sub-type to a returned `sqlite3_value`. JSON1 uses this to remember whether a value is JSON `null` vs SQL `NULL`, and to remember JSONB vs JSON text. Other extensions use it to round-trip their own data inside the SQL value.

## 11.14 Status counters

```c
int sqlite3_status   (int op, int *pCurrent, int *pHighWater, int resetFlag);
int sqlite3_db_status(sqlite3*, int op, int *pCurrent, int *pHighWater, int resetFlag);
```

Global (`sqlite3_status`): `SQLITE_STATUS_MEMORY_USED`, `SQLITE_STATUS_MALLOC_COUNT`, `SQLITE_STATUS_PAGECACHE_USED`, `SQLITE_STATUS_PAGECACHE_OVERFLOW`, `SQLITE_STATUS_SCRATCH_USED`, `SQLITE_STATUS_SCRATCH_OVERFLOW`, `SQLITE_STATUS_PARSER_STACK`, …

Per-connection (`sqlite3_db_status`): `SQLITE_DBSTATUS_LOOKASIDE_USED/HIT/MISS_SIZE/MISS_FULL`, `SQLITE_DBSTATUS_CACHE_USED/SHARED/HIT/MISS_*`, `SQLITE_DBSTATUS_SCHEMA_USED`, `SQLITE_DBSTATUS_STMT_USED`, `SQLITE_DBSTATUS_TOTAL_DEAD_VTAB`, `SQLITE_DBSTATUS_CACHE_USED_SHARED`, …

Useful for "is the cache helping?" diagnostics.

## 11.15 SQL features mapped to source files

| Feature | Files |
|---------|-------|
| SELECT and compound selects | `select.c` |
| INSERT (incl. UPSERT/ON CONFLICT) | `insert.c` |
| UPDATE | `update.c` |
| DELETE | `delete.c` |
| CREATE/ALTER/DROP TABLE | `table.c` |
| CREATE/DROP TRIGGER, `INSTEAD OF` | `trigger.c` |
| CREATE/DROP VIEW | `select.c` / `attach.c` |
| Foreign keys | `fkey.c` |
| Common Table Expressions (`WITH … AS`) | parsed by Lemon, materialized in `select.c` |
| Window functions (`OVER (…)`) | `window.c` |
| Generated columns (`AS (…) STORED`/`VIRTUAL`) | `expr.c`, schema cookie bump |
| UPSERT (`ON CONFLICT … DO UPDATE`) | `insert.c` |
| `ANALYZE` (statistics) | `analyze.c` |
| `VACUUM` / incremental vacuum | `vacuum.c` |
| `PRAGMA …` | `pragma.c` |
| `ATTACH DATABASE` | `attach.c` |
| `EXPLAIN` / `EXPLAIN QUERY PLAN` | `vdbeaux.c` / `wherecode.c` |
| Tokenizer | `tokenize.c` |
| Built-in functions (math, string, date) | `func.c`, `date.c`, `utf.c` |
| JSON1 extension | `json1.c` |
| Full-text search | `fts3*.c`, `fts5*.c` |
| R-Tree | `rtree*.c` |
| Session / change-set capture | `sqlite3session.c` |
| RBU (Resumable Bulk Update) | `sqlite3rbu.c` |

---

## 12. How to use SQLite from C (the canonical pattern)

```c
#include "sqlite3.h"

sqlite3 *db;
sqlite3_stmt *stmt;
char *errmsg = NULL;

if (sqlite3_open("app.db", &db) != SQLITE_OK) {
    fprintf(stderr, "open: %s\n", sqlite3_errmsg(db));
    return 1;
}

const char *sql = "SELECT id, name FROM users WHERE age > ?;";
if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "prepare: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
}

sqlite3_bind_int(stmt, 1, 18);          /* ? */

while (sqlite3_step(stmt) == SQLITE_ROW) {
    int    id   = sqlite3_column_int (stmt, 0);
    const unsigned char *name =
        sqlite3_column_text(stmt, 1);
    printf("%d %s\n", id, (const char *)name);
}

sqlite3_finalize(stmt);
sqlite3_close(db);
```

The companion convenience wrapper `sqlite3_exec()` does the prepare/step/finalize loop in one call, with a row callback.

---

## 13. Useful diagnostics you can plug in

```c
/* Trace every SQL statement as it runs */
sqlite3_trace_v2(db,
    SQLITE_TRACE_STMT,        /* and/or PROFILE, ROW, CLOSE */
    myTraceCb, NULL);

/* Block writes that don't touch any table */
sqlite3_set_authorizer(db, myAuthCb, NULL);

/* Get a progress callback every N VDBE ops */
sqlite3_progress_handler(db, 1000, myProgressCb, NULL);

/* Compile options for support tickets */
printf("%s\n", sqlite3_sourceid());
for (int i = 0; ; i++) {
    const char *opt = sqlite3_compileoption_get(i);
    if (!opt) break;
    printf("opt: %s\n", opt);
}
```

---

## 14. Things I learned / takeaways

1. **One translation unit is a deliberate choice.** SQLite's authors report 5–10% speed-up from amalgamating because the compiler can inline across module boundaries (`sqlite3.c` line 6–8). That comes at the cost of a 235 k-line file; the trade-off is documented up front.

2. **Strict layering pays off.** Tokenizer → Parser → Code Generator → WHERE planner → VDBE → B-tree → Pager → VFS. Every layer calls only into the layer beneath through a struct of function pointers or a small API surface, which is exactly why VFS substitution, custom page caches, and debug builds "just work."

3. **The VDBE is the heart.** It is a tiny register machine with ~150 opcodes. Each opcode carries a one-line synopsis in `opcodes.h`, which doubles as the documentation. Studying `vdbe.c::sqlite3VdbeExec` is the fastest way to understand what SQLite actually does.

4. **Prepare/step/finalize is the API contract.** Every statement goes through this lifecycle. Binding and reading columns are the same operations viewed from C/V as from inside a user function (`sqlite3_value`, `sqlite3_context`).

5. **Result codes are two-tiered.** Primary codes (`SQLITE_OK`…`SQLITE_WARNING`) plus extended codes (`primary | (subcode << 8)`). The SQLITE_ROW/SQLITE_DONE duo (`100`/`101`) is separate from the error range so callers can branch on `rc` directly.

6. **The VFS layer is genuinely portable.** Every OS interaction goes through `sqlite3_vfs` / `sqlite3_file`. That is why the same `sqlite3.c` builds on Windows, POSIX, and embedded targets with no `#ifdef` soup in the engine.

7. **Modules are pluggable.** JSON1, FTS3/4/5, R-Tree, sessions, RBU are all bundled but isolated. They hook into the same code generator and VDBE that core SQL uses — there is no "second SQL engine" inside extensions.

8. **`SQLITE_PRIVATE = static`** (line 23) prevents symbol leakage from the amalgamation even when every file is in one translation unit. The public surface lives entirely in `sqlite3.h`.

9. **`SQLITE_SOURCE_ID` is a content identifier.** It is the Fossil check-in SHA3-256 plus date. Editing the amalgamation changes the last four hex digits, so mismatched libraries and headers are detectable at runtime.

11. **Two header surfaces.** `sqlite3.h` is for embedding applications; `sqlite3ext.h` is for extensions that load via `sqlite3_load_extension`. The latter defines `sqlite3_api_routines`, a giant function-pointer table that SQLite builds per-load. This is how a `.so` extension can call any SQLite API without linking against the engine.

12. **Build flags matter.** Compile options change the API surface, the size of the binary, and even which functions exist (`SQLITE_OMIT_FOREIGN_KEY`, `SQLITE_OMIT_JSON`, …). `sqlite3_compileoption_used` / `sqlite3_compileoption_get` let you verify what was actually compiled in.

---

## 15. Where to go in `sqlite3.c` for a particular topic

| If you want to understand… | Read… |
|----------------------------|-------|
| Public API entry points | `main.c` (`sqlite3_open`, `sqlite3_close`), `prepare.c`, `vdbeapi.c` |
| SQL parsing | `tokenize.c` + `parse.h` (Lemon parser generated into `sqlite3.c`) |
| Code generation | `select.c`, `insert.c`, `update.c`, `delete.c`, `expr.c` |
| Query planning | `where.c` (`sqlite3WhereBegin` / `sqlite3WhereEnd`), `whereexpr.c`, `whereInt.h`, `wherecode.c` |
| Window functions | `window.c` |
| Index design and cursors | `btree.c` + `btreeInt.h` |
| Transactions, journaling, WAL | `pager.c` + `pager.h` |
| Page caching | `pcache.c` + `pcache.h` |
| OS-specific behaviour | `os_unix.c`, `os_win.c`, `mutex_unix.c`, `mutex_w32.c` |
| Memory allocation | `mem0.c` + `mem1/2/3/5.c`, `malloc.c` |
| Built-in SQL functions | `func.c`, `date.c`, `utf.c`, `json1.c` |
| Foreign keys / triggers / views | `fkey.c`, `trigger.c`, `view.c` (within `attach.c`/`select.c`) |
| Virtual tables | `vdbevtab.c`, `sqlite3rtree.h`, FTS code |
| Extensions | `loadext.c`, `sqlite3ext.h`, `sqlite3rbu.c`, `sqlite3session.c` |
| Backup / incremental BLOB | `backup.c`, `vdbeblob.c` |
| Tracing / debugging | `vdbetrace.c`, `treeview.c`, `fault.c` |

---

## 16. Build command

The simplest possible use of this amalgamation:

```sh
cc -O2 -c sqlite3.c                  # produces sqlite3.o
cc  -O2   app.c sqlite3.o -lpthread  # link with the thread library on POSIX
```

To strip features:

```sh
cc -O2 -DSQLITE_OMIT_LOAD_EXTENSION=1 -DSQLITE_ENABLE_FTS5=1 \
    -c sqlite3.c
```

To embed statically into a single program, just `#include "sqlite3.c"` once after `#include "sqlite3.h"` (the amalgamation is designed for that).

---

## 17. References

### Inside this code base
- The official docs are embedded in `sqlite3.h` — every section starts with a `CAPI3REF:` block.
- The VDBE opcodes are self-documenting in `opcodes.h` / `opcodes.c`.
- For deep dives, the Fossil source tree (`sqlite/src/...`) maps one-to-one onto the `Begin file` markers in `sqlite3.c`.

### For the database background (§0)
- *Database System Concepts* (Silberschatz, Korth, Sudarshan) — the standard textbook for the layered architecture, ACID, concurrency, and recovery.
- *Architecture of a Database System* (Hellerstein, Stonebraker, Hamilton) — a compact paper on how engines are actually built; matches the §0.1 diagram closely.
- *SQLite: Past, Present, and Future* (Gaffney et al., VLDB 2022) — design rationale straight from the SQLite team.
- SQLite's own **"Architecture of SQLite"** document and **"Atomic Commit in SQLite"** essay — short, authoritative, and directly tied to `pager.c`.
- *Designing Data-Intensive Applications* (Kleppmann) — chapters on storage engines (B-trees vs. LSM-trees), replication, and transactions for a broader context.

Happy hacking.