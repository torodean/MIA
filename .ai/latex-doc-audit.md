# LaTeX Doc Audit

This is a task file.
The MIA manual is built from [../docs/MIAManual.tex](../docs/MIAManual.tex), which includes the chapter fragments in [../docs/TeX_files/](../docs/TeX_files/).
Those fragments make factual claims about the code, commands, configuration, behavior, and the build, and they drift as the code changes.
This file directs an AI to parse the LaTeX docs and find inaccuracies, deficiencies, and other things worth fixing.

The AI's job here is to scan and report, not to rewrite the manual unprompted.
Bring findings to the user and let them decide what to fix.
When the fix is a clear one-line correction, propose the specific edit for the user to confirm.

## When to run this

Run this check when the AI has reason to believe the docs and the code may have drifted.

In particular:

- After adding, renaming, removing, or substantially changing a command, config key, sequencer action, utility, library, or build step that the manual describes.
- When editing a `.tex` fragment for any reason, audit that fragment while it is open.
- When reviewing a diff or set of changes that touches a feature documented in the manual.
- When the user asks for a doc pass over a chapter or the whole manual.

Do not rewrite untouched chapters beyond flagging drift you happen to notice while reading them.

## What to scan for

### Inaccuracies: the doc says something the code contradicts

Read the doc, then check the code (or build, or config) it refers to.
Flag any claim that no longer holds.

Common kinds and examples:

- A command listed in [MIAcommands.tex](../docs/TeX_files/MIAcommands.tex) that no longer exists in `MIAOriginal`, or a command that exists in code but is missing from the list.
- A command's described behavior, syntax, flags, or accepted limits (e.g. the `2147483647` cap, the `XXdYY` dice format) that differ from what the code actually does.
- A config key, default value, or access method in [MIAConfig.tex](../docs/TeX_files/MIAConfig.tex) that does not match the `MIAConfig` class or the example `MIAConfig.MIA` file.
- A sequencer action, input type, or enum value in [MIASequencer.tex](../docs/TeX_files/MIASequencer.tex) that does not match the sequencer code or its `SpecialButton` enum.
- A library name, `*_LIB` target, or linking example in [Libraries.tex](../docs/TeX_files/Libraries.tex) that does not match the CMake library definitions.
- A utility or library described in [Utilities.tex](../docs/TeX_files/Utilities.tex) or [Developers.tex](../docs/TeX_files/Developers.tex) that has been moved, renamed, or removed.
- A build step, flag, or script behavior in [Developers.tex](../docs/TeX_files/Developers.tex) that does not match `build.sh` or the CMake setup.
- Version numbers, dates, or "at the time of writing" statements that are now stale or false.
- A class, function, namespace, or file name in the doc that no longer matches the code (check the actual header or source rather than guessing from the name).

When a doc claim references a specific identifier, open the code and confirm it.
Do not trust the doc to verify the doc.

### Deficiencies: things missing that should be added

Flag content gaps where the manual is silent about something a user or contributor would reasonably need to know.

Common kinds:

- A new app, command, config key, sequencer action, utility, or library that was added to the code but has no doc entry.
- A chapter that exists as a stub or design note (e.g. [Maple.tex](../docs/TeX_files/Maple.tex) says no code is implemented yet) where code has since landed; flag that the doc should describe the real implementation.
- A flag, option, or edge case of an existing feature that the doc omits.
- A figure or screenshot referenced in prose but never included, or an image file in [../docs/images/](../docs/images/) that is never referenced.
- A section the table of contents or a cross-reference points to but that does not exist.

### LaTeX-level problems

Flag structural issues in the source, separate from the prose content:

- A `\ref{...}` whose target `\label{...}` does not exist, or a `\label{...}` that is never referenced.
  A `\ref` to an undefined label renders as `??` and produces a LaTeX warning.
- A `\ref{...}` used where `\label{...}` was meant, or vice versa (e.g. `figure \label{fishbot increments}` in running prose is a mistake; it should be `\ref`).
- A `\ref{...}` key that does not match the `\label{...}` key exactly, including spaces (e.g. `\ref{hardwar cursor}` vs a label spelled `hardware cursor`).
- An `\includegraphics{...}` whose image file is missing from [../docs/images/](../docs/images/).
- A chapter fragment that is not `\include`d by [MIAManual.tex](../docs/MIAManual.tex), or an `\include` whose file does not exist.
- A label that is duplicated across fragments, which makes `\ref` ambiguous.
- Broken or mis-nested list, table, or `lstlisting` environments that would fail to compile.

## How to respond

For each finding, give enough detail to act on:

- Which file and line (use the `.tex` source location).
- What kind of finding it is: inaccuracy, deficiency, or LaTeX-level.
- What the doc currently says, and what is actually true (cite the code, file, or image you checked).
- For a clear one-line correction, the exact replacement text, offered for the user to confirm.

Group findings by chapter so the user can work through them in order.

### When the fix is clear

If the correct wording is obvious and you have verified it against the code, propose the specific edit.
Show the exact lines to change and let the user confirm before it is written.
Follow the wording rules in [writing-style.md](writing-style.md) and the TeX comment conventions in [comment-formatting.md](comment-formatting.md).

Match the tone and format of the surrounding doc rather than introducing a new style.

### When the fix is not clear

Often the right description depends on intent the doc does not state, or on a feature that is still in flux.
In that case, do not guess at the wording.

Flag it instead.
Tell the user:

- Which file and line is out of sync.
- What the doc says versus what the code does.
- What you would need to know to write a correct entry.

Let the user decide whether to write it now, wait, or leave it.

## Cross-references and authority

- [CodingStandards.tex](../docs/TeX_files/CodingStandards.tex) is the canonical code-level standard.
  Where a doc fragment and the coding standard disagree, the standard wins; raise the discrepancy.
- [comment-formatting.md](comment-formatting.md) covers how to write comments in `.tex` source (`%` line comments, no Doxygen headers).
- [writing-style.md](writing-style.md) covers the wording of any prose you propose (plain, terse, no em dashes, no signposting).
- [readme-sync.md](readme-sync.md) covers folder READMEs; this file covers the LaTeX manual.
  If a finding is about the `docs/` README rather than the manual, hand it to that task.

## Scope

This is forward-going.
Audit chapters you are reading or that are affected by a change you made.
Do not rewrite untouched chapters beyond flagging drift you happen to notice.
If a chapter is already accurate, say so and move on.
