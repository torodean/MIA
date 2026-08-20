# Library and Utility Placement

This is a task file.
Whenever an AI looks at a source file for any reason, it should scan for the opportunities below and bring them to the user's attention.
The AI's job here is to scan and propose, not to do the work.
Do not make these changes automatically, even when editing the file for another reason.
Point them out and let the user decide.

This applies to C++ files in `bin/apps/`, `bin/utils/`, and `bin/libs/`.

## What to scan for

### Methods that belong in a utility or library

Look at every method and free function in the file, with extra attention to anything in an anonymous or internal namespace.

For each one, ask whether it is general-purpose logic that this file happens to need, rather than logic specific to this app or module.

General-purpose logic that commonly belongs elsewhere:

- String parsing, splitting, trimming, case conversion.
- Numeric helpers, formatting, unit conversion.
- Repeated small algorithms (filter, find, accumulate) that are not specific to the domain.

Internal-namespace helpers are the strongest signal.
They are often written because a utility did not exist or was not found, and they are usually the first thing worth relocating.

### Existing utilities or libraries that could be reused

Look for logic in the file that duplicates something a utility or library already provides.

Check the relevant modules and read the headers rather than guessing from names.
The READMEs in [../bin/utils/README.md](../bin/utils/README.md) and [../bin/libs/README.md](../bin/libs/README.md) outline what each utility and library module covers, so consult them to find the right home for a move or the right existing method to reuse.

Also flag the reverse case: a utility that almost fits but not exactly, where extending it would be better than the app re-implementing it.

## How to report

When you find a candidate, surface it to the user with enough detail to act on.

For each one, give:

- The file and method (or free function) in question.
- Whether it is a move-to-library candidate or a reuse candidate.
- For a move: the utility or library module it could move to, or that a new utility method is warranted.
- For a reuse: the existing method or module that already does this.

Offer the options as a choice.
Do not pick one and proceed unless the user asks.

Typical options to offer:

- Move the method to an existing utility or library.
- Add a new utility method for it (only if nothing existing fits).
- Replace it with an existing utility method.
- Leave it as-is (app-specific after all).

Keep the scan lightweight.
This is a pass over the file to flag opportunities, not a refactor.
If there is nothing worth flagging, say so and move on.

## Scope

This is forward-going.
Apply it to files you are reading or editing.
Do not rewrite untouched files beyond flagging them.
