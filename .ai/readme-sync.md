# README Sync

This is a task file.
The project keeps a README in most folders to explain what that folder contains.
When files or features are added or changed, the README is often forgotten, and the folder description drifts from what is actually there.
This file directs an AI to catch that drift.

The AI's job is to scan for drift and either propose a specific README edit for the user to confirm, or flag the gap when the change is not clear enough to write.
Do not leave a README out of sync silently.

## When to run this

Run this check whenever the AI looks at a folder that has a README, and especially when it has just added, renamed, removed, or substantially changed files in that folder.
Also run it when reviewing a diff or a set of changes that touches a folder with a README.

This applies to any folder with a README: top-level, `bin/`, `bin/apps/<app>/`, `bin/utils/<module>/`, `bin/libs/<lib>/`, `docs/`, `scripts/`, `resources/`, and so on.

## What to scan for

Compare the folder's actual contents against what its README describes.

Look for:

- A file or subfolder present in the folder but not mentioned in the README.
- A README entry for something that no longer exists in the folder.
- A description that no longer matches what the file or subfolder does, because its purpose changed.
- A new feature or capability added to an existing file that the README's description of that file does not reflect.
- A folder with no README at all, where one would help.

The READMEs describe folders at a "what is here and what does it do" level, not a line-by-line changelog.
Match that level when judging drift.

## How to respond

### When the change is clear

If it is obvious what the README should say, propose the specific edit.

Show the user the exact lines to add, remove, or change, and let them confirm before it is written.
Follow the wording rules in [writing-style.md](writing-style.md) and the Markdown one-sentence-per-line rule.

Prefer to match the tone and format of the surrounding README entries rather than introducing a new style.

### When the change is not clear

Often a new file or feature is only partially developed at the time it is committed, and its final purpose is not yet settled.
In that case, do not guess at a description.

Flag it instead.
Tell the user:

- Which folder's README is out of sync.
- What was added, removed, or changed.
- That the purpose is not clear enough to write a confident description.
- What you would need to know to write it.

Let the user decide whether to write the entry now, wait until the feature is further along, or leave it out.

## Scope

This is forward-going.
Apply it to folders you are reading or changing.
Do not rewrite untouched READMEs beyond flagging drift you happen to notice.
If a README is already in sync, say so and move on.
