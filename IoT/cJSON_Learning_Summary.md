# cJSON Source Code Learning Summary

> Notes from reading `cJSON.h` / `cJSON.c` (version 1.7.12).
> cJSON is an ultra-lightweight JSON parser/generator written by Dave Gamble. It ships as just two files (`.c` + `.h`) that can be compiled into a single translation unit.
>
> **Source Repository**: [https://github.com/DaveGamble/cJSON](https://github.com/DaveGamble/cJSON)
> - License: MIT
> - Author: Dave Gamble and cJSON contributors
> - Latest stable at time of writing: v1.7.18 (this code corresponds to v1.7.12)

---

## 1. File Overview

| File | Lines (approx.) | Role |
|------|----------------|------|
| `cJSON.h` | 285 | Header: type definitions, macros, public API declarations, platform compatibility macros |
| `cJSON.c` | 2995 | Implementation: parsing, serialization, memory management, linked-list operations |

Design philosophy: **all public APIs are prefixed with `cJSON_`**; **internal helpers use `static`**; **cross-platform details are fully encapsulated in macros and hooks**.

---

## 2. Core Data Structures

### 2.1 `cJSON` Node (Doubly-Linked List + Tree)

```c
typedef struct cJSON {
    struct cJSON *next;        // next sibling (same level)
    struct cJSON *prev;        // previous sibling (same level)
    struct cJSON *child;       // head of children chain (elements of array/object)

    int type;                  // type + flag bits

    char *valuestring;         // string value / raw JSON text
    int   valueint;            // integer value (direct writes are deprecated; use cJSON_SetNumberValue)
    double valuedouble;        // floating-point value

    char *string;              // key name when the item is inside an object
} cJSON;
```

Key points:
- The **same struct represents both objects and arrays**: arrays ignore `string`, objects use `string` to hold the key.
- Siblings are maintained as a **doubly-linked chain via `child` (head) + `next`/`prev`**.
- Clever optimization in `add_item_to_array()`: `array->child->prev` points to the **last element**, giving `O(1)` tail append instead of `O(n)` traversal.

### 2.2 Types and Flag Bits

```c
#define cJSON_Invalid (0)
#define cJSON_False   (1 << 0)   // 1
#define cJSON_True    (1 << 1)   // 2
#define cJSON_NULL    (1 << 2)   // 4
#define cJSON_Number  (1 << 3)   // 8
#define cJSON_String  (1 << 4)   // 16
#define cJSON_Array   (1 << 5)   // 32
#define cJSON_Object  (1 << 6)   // 64
#define cJSON_Raw     (1 << 7)   // 128

#define cJSON_IsReference   256  // reference type (does not free children)
#define cJSON_StringIsConst 512  // key is a constant string (not copied, not freed)
```

- The **base type uses only the low 8 bits** (`item->type & 0xFF`).
- Higher bits are **flags** that can be OR-ed with the base type, e.g. `cJSON_String | cJSON_IsReference`.
- Type-checking helpers (e.g. `cJSON_IsString`) all mask out flags with `& 0xFF`.

### 2.3 `error` / `parse_buffer` / `printbuffer`

```c
typedef struct {                          // parse-error backtrack
    const unsigned char *json;
    size_t position;
} error;

typedef struct {                          // parsing cursor
    const unsigned char *content;
    size_t length, offset, depth;         // depth guards against stack overflow
    internal_hooks hooks;
} parse_buffer;

typedef struct {                          // serialization buffer
    unsigned char *buffer;
    size_t length, offset, depth;
    cJSON_bool noalloc;                   // external-buffer mode
    cJSON_bool format;                    // pretty-print flag
    internal_hooks hooks;
} printbuffer;
```

`CJSON_NESTING_LIMIT` (default 1000) prevents malicious deeply-nested input (e.g. `[[[[[...]]]]]`) from overflowing the recursion stack.

---

## 3. Cross-Platform and Export Control

### 3.1 Calling Convention (Windows)

```c
#define CJSON_CDECL    __cdecl
#define CJSON_STDCALL  __stdcall

#if defined(CJSON_HIDE_SYMBOLS)
#define CJSON_PUBLIC(type)   type CJSON_STDCALL
#elif defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_PUBLIC(type)   __declspec(dllexport) type CJSON_STDCALL
#elif defined(CJSON_IMPORT_SYMBOLS)
#define CJSON_PUBLIC(type)   __declspec(dllimport) type CJSON_STDCALL
#endif
```

Three macros control DLL import/export and force `__stdcall`, avoiding crashes when the caller uses a different default calling convention.

### 3.2 GNU Visibility (Non-Windows)

```c
#if (defined(__GNUC__) || ...) && defined(CJSON_API_VISIBILITY)
#define CJSON_PUBLIC(type) __attribute__((visibility("default"))) type
#else
#define CJSON_PUBLIC(type) type
#endif
```

Combined with `-fvisibility=hidden`, this gives fine-grained symbol hiding for `.so` builds.

### 3.3 MSVC Workaround

```c
#if defined(_MSC_VER)
static void * CJSON_CDECL internal_malloc(size_t size) { return malloc(size); }
...
#else
#define internal_malloc  malloc
...
#endif
```

Reason: MSVC errors out (C2322) when taking the address of a `dllimport`-ed function. Static wrapper functions sidestep that.

---

## 4. Memory Management and Hooks

### 4.1 Hook Abstraction

Users can inject custom `malloc`/`free` through `cJSON_InitHooks()`:

```c
typedef struct cJSON_Hooks {
    void *(*malloc_fn)(size_t);
    void  (*free_fn)(void*);
} cJSON_Hooks;

typedef struct internal_hooks {
    void *(*allocate)(size_t);
    void  (*deallocate)(void*);
    void *(*reallocate)(void *ptr, size_t size);   // only when malloc==free==libc
} internal_hooks;
```

Internally all modules use `hooks->allocate / deallocate` — never `malloc` directly — so the library fits cleanly into embedded systems, memory pools, instrumentation, etc.

### 4.2 Recursive Release in `cJSON_Delete`

```c
void cJSON_Delete(cJSON *item) {
    while (item != NULL) {
        next = item->next;
        if (!(item->type & cJSON_IsReference) && item->child)
            cJSON_Delete(item->child);          // recursion
        if (!(item->type & cJSON_IsReference) && item->valuestring)
            hooks->deallocate(item->valuestring);
        if (!(item->type & cJSON_StringIsConst) && item->string)
            hooks->deallocate(item->string);
        hooks->deallocate(item);
        item = next;
    }
}
```

Key rules:
- **`cJSON_IsReference` does not free children or `valuestring`** because they are borrowed (e.g. from `CreateStringReference`).
- **`cJSON_StringIsConst` does not free `string`** because it points to a constant string.

---

## 5. Parser Implementation

### 5.1 Entry Point and BOM Handling

```c
cJSON_ParseWithOpts(...) {
    parse_buffer buffer = {0};
    buffer.content = (const unsigned char*)value;
    buffer.length  = strlen(value) + 1;
    buffer.hooks   = global_hooks;

    if (!parse_value(item, buffer_skip_whitespace(skip_utf8_bom(&buffer))))
        goto fail;
    ...
}
```

- Automatically skips a UTF-8 BOM (`\xEF\xBB\xBF`).
- `require_null_terminated` enforces no trailing garbage after the JSON document.
- On failure, `global_error` is set so `cJSON_GetErrorPtr()` can report the error location.

### 5.2 Recursive Descent: `parse_value`

The first character dispatches to the appropriate parser:

```c
if (can_read(input_buffer, 4) && strncmp(.., "null", 4) == 0) ...
if (can_read(input_buffer, 5) && strncmp(.., "false", 5) == 0) ...
if (can_read(input_buffer, 4) && strncmp(.., "true", 4) == 0) ...
if ('"' == buf[0])   parse_string(...)
if ('-' == buf[0] || isdigit(buf[0]))  parse_number(...)
if ('[' == buf[0])   parse_array(...)
if ('{' == buf[0])   parse_object(...)
```

Plain but effective recursive descent. **No separate lexer** — just one-token lookahead via the first character.

### 5.3 Number Parsing

`parse_number()`:
1. Copy valid characters (`0-9 + - e E .`) into a temp buffer, substituting `.` with the **current locale's decimal point** (via `get_decimal_point()`) so `strtod` is happy.
2. Convert with `strtod()` in one shot.
3. `valueint` uses **saturated truncation** (clamped to `INT_MAX` / `INT_MIN`) to avoid undefined behaviour.
4. Advance `offset` by `after_end - number_c_string` bytes.

### 5.4 String Parsing and Escapes

`parse_string()` does a **two-pass scan**:
1. **Pre-scan**: estimate output size (one `\\` reduces output by 1 byte; UTF-16 placeholders are not expanded here).
2. **Actual copy**: handles `\b \f \n \r \t \" \\ \/` and `\uXXXX` (converted to UTF-8).

#### 5.4.1 UTF-16 → UTF-8

`utf16_literal_to_utf8()`:
- A single `\uXXXX` (codepoint ≤ 0xFFFF) is encoded as 1–3 UTF-8 bytes.
- A high surrogate (`0xD800–0xDBFF`) must be followed by a low surrogate (`0xDC00–0xDFFF`); together they form a full `0x10000+` codepoint encoded as 4 UTF-8 bytes.
- Strict validation: lone surrogates and codepoints above `0x10FFFF` return 0 → triggers `fail`.

`parse_hex4()` builds the 4-digit hex value with `<< 4` shifts plus range checks — **accumulation without multiplication**.

### 5.5 Array / Object Parsing

`parse_array()`:
1. On entry, `depth++`; if it exceeds `CJSON_NESTING_LIMIT` the parse fails immediately.
2. Use `do { ... } while (next char == ',')` to handle `[]`, `[v]`, and `[v, v, v]` uniformly.
3. Maintain a `head` chain stitched together via `next` / `prev`.
4. The `success` label attaches `head` to `item->child` — **ensuring the empty array `[]` parses successfully too**.

`parse_object()` adds one extra step: **after parsing the key string, swap `valuestring` and `string`**, so the key lands in `string` and `valuestring` is reset to NULL (keys carry no value).

### 5.6 Parse-Buffer Macros

```c
#define can_read(buffer, size)             ((buffer)->offset + size <= (buffer)->length)
#define can_access_at_index(buffer, i)     ((buffer)->offset + i < (buffer)->length)
#define buffer_at_offset(buffer)           ((buffer)->content + (buffer)->offset)
```

**Every pointer access is guarded by `can_access_*`**, virtually eliminating out-of-bounds reads. The fail paths heavily rely on `goto fail;` to centralize cleanup.

---

## 6. Serialization (Print) Implementation

### 6.1 `print()` Main Entry

```c
static unsigned char *print(const cJSON *item, cJSON_bool format, const internal_hooks *hooks) {
    printbuffer buffer[1];
    buffer->buffer = hooks->allocate(256);          // initial 256 bytes
    buffer->length = 256;
    buffer->format = format;
    buffer->hooks  = *hooks;

    if (!print_value(item, buffer)) goto fail;
    update_offset(buffer);

    // Finalize: shrink to exact size
    if (hooks->reallocate) {
        printed = hooks->reallocate(buffer->buffer, buffer->offset + 1);
    } else {
        printed = hooks->allocate(buffer->offset + 1);
        memcpy(printed, buffer->buffer, ...);
        hooks->deallocate(buffer->buffer);
    }
    return printed;
}
```

Four output styles:
- `cJSON_Print()` — formatted (with indentation / newlines, **tab indentation**).
- `cJSON_PrintUnformatted()` — compact, no whitespace.
- `cJSON_PrintBuffered(item, prebuffer, fmt)` — caller-controlled preallocation (key for performance).
- `cJSON_PrintPreallocated(item, buf, len, fmt)` — caller-provided buffer (`noalloc=1`); ideal for memory-constrained or zero-copy scenarios.

### 6.2 `ensure()` — Auto-Grow

```c
static unsigned char *ensure(printbuffer *p, size_t needed) {
    needed += p->offset + 1;
    if (needed <= p->length) return p->buffer + p->offset;

    if (p->noalloc) return NULL;                  // external buffer cannot grow

    // Pick new capacity: needed * 2, capped at INT_MAX
    size_t newsize = needed * 2;

    if (p->hooks.reallocate) {
        newbuffer = p->hooks.reallocate(p->buffer, newsize);
        if (!newbuffer) {                          // realloc failed
            p->hooks.deallocate(p->buffer);
            ...
            return NULL;
        }
    } else {
        // Fallback: malloc + memcpy + free
        ...
    }
    p->length  = newsize;
    p->buffer  = newbuffer;
    return newbuffer + p->offset;
}
```

Highlights:
- **Exponential growth** (×2) with an **INT_MAX ceiling** and a **realloc → malloc/memcpy fallback** that still yields a valid result or NULL.
- Every `print_*` function `ensure()`s space for the trailing `\0` up front.

### 6.3 `print_string_ptr()` — Escaping Output

Special characters (`\" \\ \b \f \n \r \t`) and ASCII control characters (`< 32`) are escaped:

```c
sprintf((char*)output_pointer, "u%04x", *input_pointer);
```

Other characters are copied verbatim. The function does **one full scan first** to count escape characters (`escape_characters++`), then a single `ensure()` avoids repeated reallocation.

### 6.4 `print_number()` — Number Formatting

```c
if ((d * 0) != 0)                 // NaN / Infinity
    length = sprintf(buf, "null");
else {
    length = sprintf(buf, "%1.15g", d);          // try 15 significant digits
    if (sscanf(buf, "%lg", &test) != 1 || !compare_double(test, d))
        length = sprintf(buf, "%1.17g", d);      // round-trip mismatch → upgrade to 17 digits (full double precision)
}
```

After formatting, the locale-specific decimal point is rewritten back to `.`, keeping the output spec-compliant.

`compare_double()` uses `CJSON_DOUBLE_PRECISION` (default `1e-16`) for safe float comparison.

### 6.5 Pretty-Printing Objects

```c
*output_pointer++ = '{';
if (format) *output_pointer++ = '\n';
output_buffer->depth++;
...                                 // write \t according to depth
*output_pointer++ = ':';
if (format) *output_pointer++ = '\t';
...                                 // iterate children, comma+newline
for (i = 0; i < output_buffer->depth - 1; i++)
    *output_pointer++ = '\t';
*output_pointer++ = '}';
```

Indentation uses **tab characters**; the level equals `depth`, and the closing brace indents one level less than the opening one.

---

## 7. Public API Quick Reference

### 7.1 Creation

```c
cJSON_CreateNull / CreateTrue / CreateFalse / CreateBool(b)
cJSON_CreateNumber(double)
cJSON_CreateString(const char*)
cJSON_CreateRaw(const char*)                  // embed a raw JSON snippet
cJSON_CreateArray()
cJSON_CreateObject()

cJSON_CreateStringReference(string)           // borrowed, not copied
cJSON_CreateObjectReference(child)
cJSON_CreateArrayReference(child)

cJSON_CreateIntArray / FloatArray / DoubleArray / StringArray
```

### 7.2 Add / Remove / Replace

```c
cJSON_AddItemToArray(array, item)
cJSON_AddItemToObject(obj, "key", item)
cJSON_AddItemToObjectCS(obj, "key", item)     // key is not copied
cJSON_AddItemReferenceToArray/Object(...)

cJSON_DetachItemViaPointer(parent, item)
cJSON_DetachItemFromArray(array, idx)
cJSON_DetachItemFromObject(obj, "key")        // case-insensitive
cJSON_DetachItemFromObjectCaseSensitive(...)
cJSON_DeleteItemFromArray(...)                // detach + delete in one call
cJSON_DeleteItemFromObject(...)

cJSON_InsertItemInArray(array, idx, item)     // shifts subsequent items right
cJSON_ReplaceItemViaPointer(parent, item, newitem)
cJSON_ReplaceItemInArray(array, idx, newitem)
cJSON_ReplaceItemInObject(obj, "key", newitem)
```

Convenience macros:
```c
cJSON_AddStringToObject(obj, "name", "x");    // create + attach in one shot
cJSON_AddNumberToObject(obj, "n", 1.0);
cJSON_AddBoolToObject(obj, "b", 1);
cJSON_AddNullToObject / AddTrueToObject / AddFalseToObject / ...
```

### 7.3 Lookup

```c
cJSON_GetArraySize(array)
cJSON_GetArrayItem(array, idx)
cJSON_GetObjectItem(obj, "key")               // case-insensitive
cJSON_GetObjectItemCaseSensitive(obj, "key")
cJSON_HasObjectItem(obj, "key")

cJSON_IsInvalid / IsFalse / IsTrue / IsBool / IsNull / IsNumber / IsString
cJSON_IsArray / IsObject / IsRaw

cJSON_ArrayForEach(el, array)                 // iteration macro
    for (el = (array) ? (array)->child : NULL; el != NULL; el = el->next)
```

### 7.4 Other Utilities

```c
cJSON_Duplicate(item, recurse)                // shallow/deep copy
cJSON_Compare(a, b, case_sensitive)
cJSON_Minify(char *json)                      // in-place compression (strips whitespace and comments)
cJSON_GetStringValue(item)
cJSON_SetNumberValue(obj, n)                  // updates both valuedouble and valueint
cJSON_GetErrorPtr()                           // last parse-failure position
```

---

## 8. Implementation Techniques Worth Borrowing

| Technique | Location |
|-----------|----------|
| Base types encoded as `1 << n` bits, easy to combine with flag bits | `cJSON*.h` |
| Low 8 bits serve as the type mask (`& 0xFF`); flags live in upper bits | `cJSON_Is*` family |
| `goto fail;` for centralized cleanup, avoiding deeply-nested `if`s | almost every parser |
| `parse_buffer` + explicit `offset` instead of raw pointer bumping; checks bounds | `parse_*` everywhere |
| Two-pass scan to size the output first, then a single `allocate` | `parse_string` |
| `printbuffer` + `ensure()` + exponential growth (×2) + INT_MAX ceiling | `ensure()` |
| Prefer `realloc`; fall back to malloc/memcpy/free when unavailable | `ensure()` |
| `array->child->prev` caches the tail of the list for `O(1)` append | `add_item_to_array` |
| `array->child->prev` always points to the last item | `add_item_to_array` |
| Number values use **saturated truncation** (INT_MAX / INT_MIN) instead of direct casts | `parse_number / cJSON_SetNumberHelper` |
| Float output tries `%.15g`; on round-trip mismatch upgrades to `%.17g` to preserve precision | `print_number` |
| Locale decimal-point `.` rewrite, so JSON never becomes `,` under different locales | `get_decimal_point` |
| Cross-platform and visibility options all encapsulated in the `CJSON_PUBLIC` macro | `cJSON.h` |
| MSVC cannot take address of `dllimport` → static wrapper functions sidestep it | `internal_malloc/free/realloc` |
| Hook abstraction: internal modules use `internal_hooks`; user-visible API exposes only `malloc_fn/free_fn` | `cJSON_InitHooks` |
| `cJSON_IsReference` / `cJSON_StringIsConst` flags decide whether `Delete` frees children / strings | `cJSON_Delete` |
| `parse_object` swaps `valuestring ↔ string` to reuse `parse_string` for keys | `parse_object` |
| `Minify` strips whitespace, `//` and `/* */` comments in-place | `cJSON_Minify` |

---

## 9. Limitations and Gotchas

1. **Lenient validation**: out-of-range numbers, surrogate pairs, invalid `\uXXXX` escapes are handled by simply returning failure — it is not a diagnostic-grade validator.
2. **`cJSON_GetArraySize` returns `int`** — the source itself flags this with `FIXME: Can overflow here. Cannot be fixed without breaking the API`. Arrays exceeding `INT_MAX` elements will wrap.
3. **`cJSON_Compare` on objects is `O(n²)`** (the source comment says: `TODO This has O(n^2) runtime, which is horrible!`).
4. **`cJSON_Minify` only strips ASCII whitespace and `//` / `/* */`** — and it requires **writable memory**, so do not pass string literals.
5. **`global_error` and `global_hooks` are singletons**, which means **cJSON is not thread-safe**. In multi-threaded use, complete each parse lifecycle in one thread.
6. **Numbers are parsed as `double`**: precision is limited to `double`. Not suitable for high-precision financial calculations.
7. **`CJSON_HIDE_SYMBOLS` plus `__stdcall` only takes effect on Windows**; on Linux `CJSON_PUBLIC(type) type` is the plain default calling convention and imposes no ABI constraint.

---

## 10. Practical Usage Template

```c
#include "cJSON.h"
#include <stdio.h>

int main(void) {
    const char *json = "{\"name\":\"kylin\",\"nums\":[1,2,3],\"ok\":true}";

    cJSON *root = cJSON_Parse(json);                       // parse
    if (!root) {
        const char *err = cJSON_GetErrorPtr();
        if (err) fprintf(stderr, "Parse error near: %s\n", err);
        return 1;
    }

    cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (cJSON_IsString(name) && name->valuestring)
        printf("name = %s\n", name->valuestring);

    cJSON *nums = cJSON_GetObjectItemCaseSensitive(root, "nums");
    int n = cJSON_GetArraySize(nums);
    for (int i = 0; i < n; ++i) {
        cJSON *v = cJSON_GetArrayItem(nums, i);
        printf("nums[%d] = %.0f\n", i, v->valuedouble);
    }

    // Build dynamically
    cJSON *out = cJSON_CreateObject();
    cJSON_AddStringToObject(out, "device", "camera01");
    cJSON_AddNumberToObject(out, "fps", 30.0);
    cJSON *arr = cJSON_AddArrayToObject(out, "modes");
    cJSON_AddItemToArray(arr, cJSON_CreateString("HDR"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("SDR"));

    char *printed = cJSON_PrintUnformatted(out);
    puts(printed);

    free(printed);
    cJSON_Delete(root);
    cJSON_Delete(out);
    return 0;
}
```

---

## 11. Takeaways

- **Code organization**: collapsing "type + flags + list node" into a single `cJSON` struct lets the same code path serve arrays, objects, strings, numbers, and null — a very clean "data-structure-oriented programming" style.
- **Robustness**: every cross-boundary access is guarded by a macro; every error path funnels through `goto fail`. You rarely see sequences like `if (x) return -1; if (y) return -1;` — a textbook example of idiomatic embedded C.
- **Portability**: all platform differences are isolated in the first ~30 lines of `cJSON.h`; the main code stays fully portable.
- **Extensibility**: hooks let users plug in memory pools, instrumentation, and sanitizers purely by configuration — no library changes needed.
- **Reusable core toolkit**: **bitfield types + singly-linked lists + recursive descent + exponentially-growing buffers** — these four tricks are the universal template for hand-written C parsers.

After walking through this code, reading any other C JSON library (`jansson`, `parson`, `yajl`, etc.) becomes much easier because you can focus on each library's distinguishing design choices.
