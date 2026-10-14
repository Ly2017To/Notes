# Learning Notes on `nlohmann/json` (`json.hpp`)

> Personal learning summary of [nlohmann/json](https://github.com/nlohmann/json), a modern C++ JSON library.
> Version reviewed: **3.12.0** (single-header release).
> Audience: C++ developers who are new to `nlohmann/json` and need a quick, opinionated reference.

---

## 1. What Is `nlohmann/json`?

* A modern C++ JSON library, **header-only**, MIT-licensed, widely considered the de-facto standard for JSON in C++.
* Ships in a single header `json.hpp` (~960 KB) — drop it into your project and `#include` it. No linking, no `find_package`, no sub-dependencies.
* Implements **strict** JSON parsing per **RFC 8259**. By default it does **not** accept comments, trailing commas, single quotes, unquoted keys, or any other JSON5 / "relaxed JSON" extensions (a few can be enabled with parser callbacks).
* Provides a familiar STL-like API: `is_object()`, `is_array()`, `is_string()`, `value()`, `at()`, `contains()`, iterator-based traversal, and direct `get<T>()` conversion.

## 2. When to Reach for It (and When Not To)

| Option | Pick this when… | Avoid when… |
| --- | --- | --- |
| **`nlohmann/json`** ✅ | You want a clean, STL-like API; project is small to medium; build time is not critical; you need precise parse errors. | You parse > 1 GB of JSON, or you need SIMD / DOM-less streaming. |
| **Hand-rolled parser** | You have a very small, well-defined subset of JSON and zero dependencies is mandatory. | You are parsing a non-trivial document — you'll end up re-implementing bug-for-bug what `nlohmann/json` already does correctly. |
| **RapidJSON** | You need raw throughput (header size and compile time aside). | You want a clean, allocation-leak-free API; RapidJSON is famously easy to misuse. |
| **Boost.JSON** | You are on Boost ≥ 1.75 and you want a standards-track library. | You are pinned to Boost 1.71–1.74 where `boost::json` is still experimental. |
| **simdjson** | Maximum parsing speed on huge documents; you can accept a non-DOM view. | You want a mutating DOM or a single-header include. |

In short: **for typical application code reading or writing JSON, `nlohmann/json` is the default answer.** Reach for the others only when you can measure a real problem.

## 3. Minimal Working Example

```cpp
#include "json.hpp"          // vendored copy, header-only
#include <fstream>
#include <sstream>

using json = nlohmann::json;

std::ifstream in("config.json");
std::stringstream ss; ss << in.rdbuf();

json root;
try {
    root = json::parse(ss.str());
} catch (const json::parse_error& e) {
    // e.byte    : byte offset
    // e.what()  : human-readable message including line/column
    std::cerr << "parse error at byte " << e.byte << ": " << e.what() << "\n";
    return;
}

if (!root.is_object()) {
    std::cerr << "root must be an object\n";
    return;
}

std::string name   = root.value("name",   "default-name");
int         port   = root.value("port",   8080);
bool        enable = root.value("enable", true);
```

## 4. The `value()` / `at()` / `contains()` Trinity

These three are the core of safe access:

| Method | Behavior on missing key | Behavior on type mismatch | Throws? |
| --- | --- | --- | --- |
| `j.value("k", default)` | Returns `default` | Returns `default` | No |
| `j.at("k")` | Throws `json::out_of_range` | Throws if used as wrong type | Yes |
| `j.contains("k")` + `j["k"]` | Caller checks first | Caller decides | Indirect |

**Rule of thumb:** Prefer `j.value("k", default)` for read-mostly code where a missing or differently-typed field should just fall back. Combine with `is_string()` / `is_number_integer()` / `is_boolean()` predicates when you need a guaranteed type. Use `at()` and `get<T>()` only when an absent field is a *programmer error* and should fail loudly.

A common helper pattern in production code:

```cpp
std::string getStr(const json& j, const char* key, const std::string& def = {}) {
    if (j.contains(key) && j.at(key).is_string())
        return j.at(key).get<std::string>();
    return def;
}

int getInt(const json& j, const char* key, int def) {
    if (j.contains(key) && j.at(key).is_number_integer())
        return j.at(key).get<int>();
    return def;
}

bool getBool(const json& j, const char* key, bool def) {
    if (j.contains(key) && j.at(key).is_boolean())
        return j.at(key).get<bool>();
    return def;
}
```

This is the simplest way to keep configuration files forward-compatible.

## 5. Type Predicates (Read This First)

`nlohmann/json` stores values as a tagged union. Before converting, check the type:

```cpp
j.is_null()
j.is_boolean()
j.is_number_integer()    // 64-bit integer
j.is_number_unsigned()
j.is_number_float()
j.is_string()
j.is_array()
j.is_object()
```

`j.is_number()` is `true` for all numeric subtypes but does **not** include booleans (booleans are not numbers in JSON). Use the more specific predicates when you care about exact types.

A default-constructed `json` value is `is_null() == true`. This matters when you write `json v;` and later check `if (v.is_null())`.

## 6. Error Handling: `parse_error` and `type_error`

Two exceptions you'll see most often:

* **`json::parse_error`** — input is not valid JSON.
  * Carries `byte` (offset), and `what()` includes `[json.exception.parse_error.XXX] parse error at line N, column M: ...`.
  * **This is one of the library's biggest quality-of-life wins**: the error message tells the user exactly where the syntax is wrong, which is what you want for a config file edited by ops engineers.
* **`json::type_error`** — type mismatch (`j.at("port").get<std::string>()` on a number, etc.).

**Pattern:** always `try { ... } catch (const json::exception& e)` and surface `e.what()` to the log. The base class `nlohmann::json::exception` covers both.

## 7. Iterating Objects and Arrays

```cpp
// Array of objects
for (const auto& item : root.at("items")) {
    if (!item.is_object()) continue;
    auto name = item.value("name", "");
    // ...
}

// Object
for (auto it = root.begin(); it != root.end(); ++it) {
    std::cout << it.key() << " = " << it.value() << "\n";
}
```

`it.key()` is `std::string`, `it.value()` is `json&`. Object iteration order is **insertion order** (the library uses an ordered map internally).

## 8. Building JSON

Beyond reading, you can build and emit:

```cpp
json j;
j["name"]  = "Camera11";
j["enable"] = true;
j["port"]   = 8000;
j["cameras"] = json::array();
j["cameras"].push_back({{"name", "C1"}, {"ip", "192.168.0.1"}});

std::string pretty  = j.dump(4);   // pretty-print with 4-space indent
std::string compact = j.dump();    // compact
```

`dump()` returns a `std::string` (no exceptions on normal use). It is the right method to serialize, **not** stream operators.

## 9. CMake Integration

Because it's a single header, the simplest approach is to vendor it:

```cmake
target_include_directories(my_target PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nlohmann   # json.hpp lives here
)
```

And in source:

```cpp
#include "json.hpp"   // picked up via the include path above
```

That's it. No `find_package`, no `target_link_libraries`, no install rules for the header.

If you prefer to install the library system-wide you can use its bundled `CMakeLists.txt` and link the imported target `nlohmann_json::nlohmann_json`, but for a self-contained repo the vendored approach is simpler and keeps the dependency explicit.

## 10. Build-Time Cost

The header is ~960 KB. First-time preprocessing of a translation unit that includes it takes roughly 1–3 extra seconds on a typical developer laptop. If you include it in a single `.cpp` the cost is negligible. **Lesson:** don't include `json.hpp` from widely-included headers — keep it scoped to the translation unit that actually parses JSON.

## 11. Gotchas

1. **Strict parser surprises.** People sometimes expect JSON parsers to accept comments, unquoted keys, or `=` as a separator. `nlohmann/json` does not. **The error is the right answer** — fix the input instead of teaching the parser to be lenient.
2. **`j.value()` does not convert strings to numbers.** `root.value("port", 0)` returns `0` if `"port"` is a JSON string like `"8000"`. Use explicit type predicates or `j.get<int>()` after a type check.
3. **`contains()` vs `find()`.** `contains(key)` is the right way to test for an object key. `find(key)` returns an iterator and is mainly useful when you also want to remove or modify the entry.
4. **Stream extraction vs `parse()`.** `in >> j` works but errors surface as `std::ios::failure` / end-of-stream; for clear diagnostics, read the whole stream into a string and call `json::parse()`.
5. **Default value semantics.** Default-constructed `json` is `null`, **not** an empty object. `json j; j.is_object()` is `false`. `json j{};` is the same.
6. **Ordered map cost.** `nlohmann/json` uses an ordered map (effectively `std::map`) for objects, not a hash map. For a small config this is irrelevant; for very large objects it can be measurable. The library also exposes `nlohmann::ordered_map` and `nlohmann::json` can be specialized with a different underlying container.
7. **UTF-8 input is assumed.** `json::parse` expects UTF-8 by default. If your input might be in a different encoding, convert first.
8. **Floating-point parsing follows IEEE 754 rules.** `1.0` parses as `double`. If you need exact decimals (e.g. money), do not use JSON numbers; use strings.

## 12. Hand-Rolled vs `nlohmann/json`

| Aspect | Hand-rolled tolerant parser | `nlohmann/json` |
| --- | --- | --- |
| Lines of parser code | Hundreds, depending on corner cases | Negligible (you only write the glue) |
| External dependency | None | None (header-only, vendored) |
| Standard compliance | Accepts whatever the author remembered to handle | Strict RFC 8259 |
| Error diagnostics | Usually `syntax error near offset N` | `parse error at line X, column Y` |
| API ergonomics | Custom DOM, manual type checks | Standard STL-style API |
| Type safety | Manual | `is_string()` / `get<T>()` |
| Test coverage | Self-written | Backed by extensive upstream tests |
| Verdict | Acceptable only if the input format *must* stay loose | **Recommended** — fix the input instead |

## 13. Reading Order

If you are new to the library, read the docs in this order:

1. [README — "JSON as first-class data type"](https://json.nlohmann.me/) — gives the right mental model in 5 minutes.
2. [Examples — read / write / iterate](https://json.nlohmann.me/features/) — covers the core API.
3. [API reference — `basic_json` class](https://json.nlohmann.me/api/basic_json/) — bookmark, not read end-to-end.
4. [FAQ — "How do I parse JSON with comments?"](https://json.nlohmann.me/faq/) — teaches you that strict parsing is the right default and `parser_callback_t` is the right escape hatch.

## 14. Takeaways

* **Use the standard library for standards-compliance work.** A multi-hundred-line custom parser cannot beat the readability, test coverage, and diagnostics of `nlohmann/json`. Spend the saved effort on the application logic.
* **Strict parsing is a feature, not a limitation.** When an engineer on call edits a config at 2 a.m., the difference between "syntax error at offset 412" and "parse error at line 27, column 8: expected `:`" is the difference between a 30-minute outage and a 30-second fix.
* **Header-only is a deployment asset.** Vendoring `json.hpp` removes one whole class of "what version of the lib is installed on the target?" problems — particularly relevant for air-gapped or locked-down environments.
* **`value("k", default)` is your friend** for config-like data; `at()` / `get<T>()` is the right tool when an absent field is a programmer error.
* **Read errors at construction time.** Don't bury parse errors in `if`-checks; the library will tell you the byte offset and the line/column for free.
* **Keep `json.hpp` out of your public headers.** Its compile cost should be paid by exactly one translation unit per binary.

## 15. References

* Project home: <https://github.com/nlohmann/json>
* Documentation: <https://json.nlohmann.me/>
* RFC 8259 — The JavaScript Object Notation (JSON) Data Interchange Format: <https://datatracker.ietf.org/doc/html/rfc8259>
* License: MIT
