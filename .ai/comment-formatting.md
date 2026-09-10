# Comment Formatting

How to format comments across the MIA codebase.
This is a task file: an AI following it should parse files for correct comment structure and content, and write new comments in the forms below.

The MIA codebase is **not** yet standardized.
Many existing files use older or drifted formats (see [Current drift](#current-drift) below).
The formats in this file are the target.
When editing a file, bring its comments toward these forms; do not replicate the drifted forms just to match neighbors.

## Authority and references

- The canonical code-level standard is [docs/tex_files/CodingStandards.tex](../docs/tex_files/CodingStandards.tex), section "Comments and Documentation".
  Where this file and the .tex disagree, the .tex wins; treat this file as a quick reference and raise the discrepancy rather than guessing.
- `scripts/comment_checker.py` enforces a *legacy* param/return format (`@param name[type] - ...`).
  It is out of date with the Doxygen-standard direction below.
  Do not follow its bracketed format when writing new comments.
  (The script is expected to be updated to match.)
- For the *wording* inside comments (tone, em-dashes, e.g./i.e., plain language), see [writing-style.md](writing-style.md).
  This file covers structure and tags only.

## Line length

Wrap comments at roughly **120 characters**.
This is the lower end of the 120-140 range in CodingStandards.tex.
Do not enforce a hard cap; just do not run far past 120.
Do not wrap at 80.

## C++ (.hpp / .cpp)

### Doxygen is the standard

Use Doxygen `/** ... */` blocks to document files, classes, namespaces, functions, typedefs, and enumerations.
Place the authoritative Doxygen comments in **header files** (.hpp); they serve as the main documentation source.
For methods implemented in .cpp files that are not declared in a header, add the Doxygen comment in the .cpp.

Use standard Doxygen tag forms:

- `@brief` - one-line summary.
- `@param name description` - one per parameter, in order.
- `@return description` - for non-void, non-constructor functions.
- `@throws type description` - for any function that may throw.
  Use `@throws`, not `@throw`.
  The newer code (bin/libs/rpg) uses `@throws`; some older utils headers use the legacy `@throw [type] - ...` form.
  Both are being standardized on `@throws type description`.
- `@note`, `@see`, `@code`/`@endcode` as needed.

Do **not** use the legacy bracketed forms `@param name[type] - ...` or `@return [type] - ...`.
Those are being phased out.

### Block comment shape

For multi-line Doxygen and plain block comments, the `/**` (or `/*`) and `*/` delimiters each go on their own line.
Body lines are prefixed with ` * ` (space, asterisk, space):

```cpp
/**
 * @brief Sleeps for up to the given number of milliseconds.
 *
 * Returns early if the stop predicate returns true.
 *
 * @param ms Maximum number of milliseconds to sleep.
 * @param stopRequested Functor returning true when the sleep should abort.
 * @throws std::runtime_error if ms is negative.
 */
```

Not `/** body` on the same line, and not `body */` on the same line.
The open delimiter is alone; the close delimiter is alone.
This is the consistent house style and applies to plain `/* ... */` blocks too.

The `///` vs `/** ... */` block choice depends on what is being documented, not just line count:

- **Methods and tests** prefer the `/** ... */` block form, even when the brief is a single sentence that would fit on one line. These comments tend to grow a body paragraph or tags later, so the block form is the preferred choice. A single-line `///` is acceptable for single line comments, but the block form is preferred.
- **Trivial methods** such as simple getters and setters may use `///` if they fit on a single line; there is no strong preference between `///` and the `/** ... */` block form for these.
- **Member variables** use `///` for a single-line doc (on its own line above the declaration, or `///<` inline on the same line). Use the `/** ... */` block form when the description spans multiple lines. See [Member variables](#member-variables).
- **Other single-line Doxygen** (e.g. a one-off declaration doc that is not a method or test) uses `///`.

Never write `/** ... */` crammed onto a single physical line; a one-line Doxygen comment is `///`, and a multi-line one is the block form with delimiters on their own lines.

For **non-Doxygen** single-line comments, always use `//`, never `/* ... */` on one line. Reserve `/* ... */` blocks for multi-line non-Doxygen prose.

### File header

Every .cpp and .hpp file begins with a Doxygen header block.
Required tags: `@file`, `@author`, `@date`.
Use `@brief` for the one-line summary (not the plain `Description:` line that appears in many older files).

```cpp
/**
 * @file Timing.hpp
 * @author Antonius Torode
 * @date 03/09/2021
 * @brief Declares system-level timing utilities for sleeping and waiting.
 */
```

- `@file` must be followed by the file's basename.
- `@author` is `Antonius Torode` for this repo.
- `@date` is `MM/DD/YYYY` (match the existing date format in the file when editing; do not reformat dates that are already correct).
- A `@copyright` line is optional.
  Only the older encryption files carry one (`@copyright This file can be used under the conditions of Antonius' General Purpose License (AGPL).`).
  Do not add it to files that lack it unless asked.
- After the tags, an optional detailed-description paragraph is allowed for genuinely non-obvious files.
  Keep it to one or two sentences.

### Functions and methods

Each public function or method gets a Doxygen block with `@brief`, then `@param` (one per parameter), then `@return` (if non-void), then `@throws` (if it can throw).
Example:

```cpp
/**
 * @brief Simulates pressing a number key (0-9) using virtual key codes.
 *
 * Sends a key press and release event for the specified number key.
 *
 * @param num The number key to press (must be in range 0-9).
 * @param holdTime Duration in milliseconds to hold the key before releasing.
 * @param verboseMode If true, prints the pressed number to standard output.
 * @return True if the key event was sent successfully.
 * @throws std::out_of_range if num is not in [0, 9].
 */
```

Constructors and destructors have no `@return`.
Document their parameters with `@param` and any exceptions with `@throws`.

Trivial methods such as simple getters and setters are an exception: `///` is acceptable for these, with no strong preference between `///` and the `/** ... */` block form.
Use `///` when the method is a one-liner whose behavior is obvious from its signature.

### Member variables

Document a member variable with `///` on its own line above the declaration, or with `///<` inline on the same line.
Both are accepted; pick one and be consistent within a file:

```cpp
/// The type of currency.
CurrencyType type;

bool tradeable{true}; ///< Whether this currency can be traded to other players.
```

The comment describes the variable's purpose or semantics, not its type (which is clear from the code).

### Enumerations

Each enum gets a Doxygen block above it explaining its purpose.
Each enum value uses `///<` inline on the same line:

```cpp
/**
 * The various actions the sequencer supports.
 */
enum SequenceActionType
{
    UNKNOWN,     ///< Unknown action, do nothing.
    TYPE,        ///< Type a sequence of characters.
    SLEEP,       ///< Wait or pause for some time.
    DELAY,       ///< Time to wait between each action.
    MOVEMOUSE,   ///< Move the mouse to a specific coordinate.
    CLICK        ///< Perform a mouse click.
};
```

Always use `///<` on the same line as the enum value, not on a line above it.

### Inline and section comments in .cpp

Use `//` for brief explanatory comments inside function bodies.
Place a standalone comment on its own line above the code it describes; use trailing inline comments sparingly and only when the code is not self-explanatory.

```cpp
// Sleep in small slices so a stop request is observed promptly.
while (elapsed < ms)
{
    sleepMilliseconds(slice);
    elapsed += slice;
}
```

Describe *why*, not *what*.
Do not restate what the code obviously does.

### Closing-brace comments

Follow the closing brace of a namespace, class, or long code block with a trailing comment indicating its scope:

```cpp
} // namespace mio
```

This helps match braces without scrolling.
Use it for namespaces and long blocks; short blocks (a few lines) do not need it.

### Header guards

Header files use `#pragma once`.
Do not add traditional `#ifndef` include guards.
(`comment_checker.py` flags headers missing `#pragma once`.)

### Test files (*_T.cpp)

Google Test files use the same file-header format as other .cpp files.

Each `TEST(...)` gets a `/** ... */` block comment describing what the test verifies, even when the brief is a single sentence.
No `@test` tag (it duplicates the TEST name and Doxygen does not resolve gtest macros).
One terse line is usually enough; add a body paragraph only when the test is genuinely non-obvious.

```cpp
/**
 * @brief Verifies sleepMilliseconds blocks for approximately the requested duration.
 */
TEST(TimingTest, SleepMilliseconds)
```

gtest assertion failure messages (`<< "..."`) go on their own line, indented under the assertion, even when short:

```cpp
EXPECT_TRUE(container.has(coin, 50))
    << "has(coin, 50) should return true.";
```

### @TODO comments

Write TODOs as `// @TODO - <description>.` on their own line.
They are acceptable in headers and source; do not let them accumulate silently.

## CMake (.txt / .cmake)

Use `#` line comments.
Add a comment above any block of CMake that performs a distinct task.
Keep comments brief and in plain language.

```cmake
# Create the MIATemplate executable.
set(MIATemplate_SRC MIATemplate.cpp MIATemplate_main.cpp)
set(MIATemplate_INC MIATemplate.hpp)
add_executable(MIATemplate ${MIATemplate_SRC} ${MIATemplate_INC})
target_link_libraries(MIATemplate PRIVATE Core_LIB)
```

No file-level header block is required for CMakeLists.txt files.

## Shell (.sh)

Start each script with `#!/bin/bash` (or `#!/usr/bin/env bash`).
Follow with a brief comment block describing the script's purpose and usage:

```bash
#!/bin/bash

# Fixes Unix line endings for all shell files when running under Cygwin
# on Windows. Run this when build.sh fails with $'\r' errors.
```

Use `#` comments to clarify non-obvious logic.
Inline `#` comments explaining a variable assignment are fine where the assignment is non-obvious.

## Python (.py)

Use triple-quoted docstrings (`"""..."""`) for modules, functions, and classes.
The module docstring carries `@file`, `@author`, `@date` tags mirroring the C++ header convention (this repo's `comment_checker.py` and `increment_version.py` do this):

```python
#!/bin/python3
"""
@file    increment_version.py
@author  Antonius Torode
@date    2025-07-13

Increment the minor part of a version string like '2.005' to '2.006'.
"""
```

Function docstrings describe purpose, parameters, and return value in prose.
Standard `Args:` / `Returns:` sections are acceptable for Python (see `scripts/increment_version.py`).
Use `#` for inline comments.

## TeX (.tex)

Use `%` for line comments.
Comment out a line by prefixing `%`; use `%%` or `% ---` for visual section dividers inside .tex source where helpful.

Doxygen-style headers do not apply to .tex files.
A `.tex` chapter or file begins with its `\chapter{...}` or `\documentclass{...}` line; a leading comment block is optional and used only for notes to maintainers.
\label{...}` keys use the long-form convention `\label{chapter:name}`, `\label{section:name}`, or `\label{table:name}`, with underscores (not hyphens) as word separators; see [latex-doc-audit.md](latex-doc-audit.md) for the full rule.

Data/sequence files referenced from .tex (e.g. `MIASequences.txt`) use `#` line comments and may carry a `# Name / # Author / # Date / # Description` block at the top; match the existing format when editing those data files.

## Markdown (.md)

No comment syntax in the rendered sense.
Source-level HTML comments `<!-- ... -->` are allowed for maintainer notes that should not render, but use them sparingly.

For prose wording and whitespace (one sentence per line), see [writing-style.md](writing-style.md).

## Current drift

The formats above are the target.
The existing codebase drifts in these ways; recognize the drift but do not copy it into new work:

- **`Description:` instead of `@brief`** in many file headers (Timing, MIASequencer, VirtualKeyStrokes, most test files). Target: `@brief`.
- **Bracketed param/return** `@param name[type] - ...` and `@return [type] - ...` across most headers, and enforced by `comment_checker.py`. Target: standard Doxygen `@param name ...` and `@return ...`.
- **`.cpp` files often lack Doxygen on functions** that are declared in the header. This is acceptable when the header carries the docs; the .cpp needs its own Doxygen only for functions not declared in a header.
- **`///` and `//` mixed for section comments** in some headers (e.g. VirtualKeyStrokes.hpp uses `///` above includes). Use `//` for implementation/section comments; reserve `///` for documenting a declaration.
- **`@TODO` / `@todo` / `TODO`** variants. Target: `// @TODO - ...`.
- **Block comments with `/*` or `*/` on the same line as body** appear in a few places. Target: delimiters on their own lines.
- **`@throws` vs `@throw`**: newer code uses `@throws`; some older utils headers use `@throw [type] - ...`. Target: `@throws type description`.

When you encounter a drifted comment you are not editing, leave it.
Rewrite comments only in the files you are actively changing, and bring them toward the target forms above.

## Checklist for an AI editing comments

1. Header has `@file <basename>`, `@author Antonius Torode`, `@date`, and `@brief` (not `Description:`).
2. Public functions have `@brief`, `@param` per parameter, `@return` if non-void, `@throws` if it can throw. Standard Doxygen form, no brackets.
3. Member variables use `///` above or `///<` inline, consistently within the file.
4. Enum values use `///<` on the same line.
5. Multi-line `/* */` and `/** */` blocks have `/*`/`/**` and `*/` on their own lines; body lines prefixed ` * `.
6. Comments wrap near 120 chars, not 80.
7. Namespace and long-block closing braces have a trailing `} // namespace ...` comment.
8. Headers use `#pragma once`.
9. Wording follows [writing-style.md](writing-style.md): plain, terse, no em dashes, no signposting, accurate.
10. No private/local-resource references in any committed comment or doc.
