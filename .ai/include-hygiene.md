# Include Hygiene

This is a task file.
It directs an AI to check that every source file includes the headers it actually needs, and only those.
A file should name its own dependencies rather than compile by accident because some other header happened to include them for it.

The AI's job here is to scan and report, not to rewrite includes unprompted.
Bring findings to the user and let them decide what to change.

## Why

When a file gets a declaration transitively (e.g. it uses `std::string` but only includes a header which itself includes `<string>`), the file is silently coupled to the middle header's contents.
If the middle header drops that include, the file stops compiling in a way that points at the wrong place.
Reused through a target's PUBLIC link visibility, the same coupling hides at the CMake layer: a target compiles because of a link it never named.
Naming each dependency in the file that uses it makes the dependency graph honest and keeps headers self-contained.

## When to run this

Run this check when reading or editing a C++ file for any reason, and especially when:

- Adding a new function, class, or member which introduces a new type.
- Moving code between files, since moves are the most common way a file ends up using a symbol it no longer includes.
- Reviewing a diff which changes what a file depends on.
- Changing `target_link_libraries` visibility (PUBLIC to PRIVATE or the reverse), since that is when transitive-include coupling surfaces as a build break.

## What to scan for

### Missing includes: symbols used but not directly included

For each file, check that the types, functions, and macros it uses come from an include in that file, or from the file's own header when reading a .cpp.

Highest-signal cases:

- A .cpp uses a type whose declaration only arrives through its associated header's includes.
- A public header (one included by other targets) uses a type from another module but does not include that module's header itself.
- A file uses standard library types (`std::string`, `std::vector`, `std::atomic`, stream types) with no direct include for them.
- A file was recently edited and gained a new type without gaining the include for it.

When checking, prefer the declaration site over guesswork: find where the symbol is declared and confirm the including file names it.

### Unused includes: symbols included but not used

Flag includes whose declarations are not used anywhere in the file.
Common sources of drift:

- Code was removed or moved out, and the include stayed.
- An include was added defensively "just in case" and never needed.
- An include was added to be used while developing but no longer used by the time development finished.
- The file includes its own dependencies' dependencies (e.g. including `MIAException.hpp` when only `Error.hpp`'s error codes are used).

### Forward declarations vs includes

A forward declaration is preferable to an include when the file only uses the type by pointer or reference and never needs its definition.
Flag heavy includes in headers which a forward declaration would replace.

### CMake visibility coupling

Include coupling shows up at the CMake layer too.
When a target's files include a module's headers but its `target_link_libraries` call does not name that module's library, the target is picking the library up through some other target's PUBLIC link.
Flag the missing link and suggest naming it explicitly.
In the other direction, a PUBLIC link is only justified when the target's public headers include that library or its consumers are meant to depend on it; otherwise the link belongs PRIVATE.

## How to report

For each finding, give enough detail to act on:

- The file (and line, when useful) with the problem.
- Whether it is a missing include, an unused include, a forward-declaration candidate, or a CMake visibility coupling.
- What the file uses versus what it includes, and where the symbol actually comes from today.
- For a missing include, the specific header to add; for an unused include, the specific line to remove.

When the fix is unambiguous, propose the exact edit for the user to confirm.
When removing an include could break a hidden dependent (other files which rely on that include transitively through this header), say so and check those files before proposing.

Let the user decide; do not rewrite a file's include block unprompted.

## Scope

This is forward-going.
Apply it to files you are reading or editing, and to diffs you review.
Do not sweep untouched files beyond flagging what you happen to notice.
If a file's includes are already correct, say nothing and move on.
