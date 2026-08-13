# Writing Style

Preferences for tone, wording, grammar, and punctuation in anything an AI produces for the MIA project: code comments, commit messages, documentation (.md, .tex), README files, and chat replies.

For code-comment *structure* (Doxygen tags, header format, where to place comments), see [comment-formatting.md](comment-formatting.md).
That file points back here for the wording of the text inside those comments.

The repo's own [docs/TeX_files/CodingStandards.tex](../docs/TeX_files/CodingStandards.tex) covers code-level standards (naming, braces, line length, Doxygen usage).
This file covers writing style and does not duplicate the .tex; where a rule below interacts with the .tex, it says so.

## Scope

These rules constrain what the **AI** produces, not what a human author writes.
A human contributor's own voice - including their deliberate use of a sophisticated word - is their call.
The AI should not reach for fancy or formal wording, because it tends to overdo it and sound performative.
Say things plainly; let the human dress up prose when they choose to.

## Tone

Terse and plain.
Say simple things simply.

- First draft short. Add words only when they carry information.
- Cut hedging ("except in special cases", "generally", "usually") when a direct statement is accurate.
- Cut restatements. If a mechanic is defined elsewhere in the same doc or comment block, do not repeat it.
- Prefer plain verbs ("changing `main`") over fancy ones ("churning `main`"). Concrete is better than abstract.
- End on the point. Do not enumerate every case when one example suffices.

## Audience

Write for a broad audience, not for a domain expert.
Name the actual identifier (function, header, variable) concretely rather than gesturing at it.

Avoid framework-specific shorthand and verbs that assume specialist familiarity.
In particular, avoid "expose" and "consumers/producers" when plain words work:

- Prefer "Put each sub-directory on the include path so its headers can be #included by name" over "Expose each subdir on the PUBLIC include path so consumers can include them flat."
- Prefer "code that uses it" over "consumers."

Do not strip necessary precision.
Just do not dress it up in jargon when plain words carry the same meaning.

## Punctuation

### Em dashes: never

Never use an em dash (U+2014, `—`).
En dashes (`–`) are also not used.
Substitute a comma, semicolon, parentheses, hyphen, or reword.
Watch for editors that auto-convert `--` to an em dash.

Before finishing a writing task, grep changed files for U+2014 and replace any hits:

```
grep -nP "\x{2014}" <file>
```

### Spaced hyphen ` - `

A spaced hyphen (space-hyphen-space) is fine at a clause break when the second clause refers back to or restates the first:

- "...check things such as spelling - those would still sensibly remain required." ("those" refers back to "things such as spelling".)

Use a semicolon `;` when the second clause is a distinct, non-referential continuation:

- "append-only and fast-forward; force-push remains blocked."

Avoid ` - ` as a mid-sentence parenthetical aside (interrupting the main clause).
Use parentheses instead:

- Prefer "for `main` (append-only, fast-forward, protected)" over "for `main` - append-only, fast-forward, protected."

True hyphenation of compounds is always fine: `fast-forward`, `append-only`, `comment-only`, `low-friction`, `self-merge`.

### e.g. and i.e.

Write `e.g.,` and `i.e.,` with a following comma in running prose before a noun phrase.

Exception: drop the comma when it is immediately followed by an inline code span (`like this`).
Standardize within a given doc.

### Periods and sentence form

In prose comments and docs, use complete sentences with present tense and consistent punctuation.
Do not leave sentence fragments dangling without a terminal period unless they are label-style (a one-line `@brief` is a sentence and gets a period; a trailing `///<` member comment may be a short phrase and may omit the period to match surrounding style).

## Whitespace and line breaks

### Markdown and AsciiDoc

Write .md and .adoc prose one sentence per line.
End each sentence with its terminator (period, etc.) and put a newline before the next sentence.
Let lines run long; do not wrap at a fixed column.

This is about source formatting.
Rendered output is unaffected because Markdown treats a single newline as a space.
The point is that a change to one sentence shows up as a one-line git diff rather than a re-wrapped paragraph.

Do not break a sentence across lines just to keep lines short.
Do not add trailing whitespace at the end of lines.
Use a blank line to separate paragraphs.
List items and table cells keep one logical unit per line.

This is a forward-going standard.
Existing files (README.md, CHANGELOG.md) do not yet follow it; apply the rule to new prose and to prose you edit.

### Code comments

Code comments are the exception: wrap them at roughly 120 characters (see [comment-formatting.md](comment-formatting.md)).
A comment can hold multiple sentences up to the wrap width.

## Avoid signposting

Do not write sentences whose only job is to announce what the next heading, TOC, or list already says.
Trust the document's structure.

Anti-patterns to cut:

- "The principles behind them are below" under a heading that already conveys this.
- "Two structural ideas apply across tiers:" when the list stands on its own. Replace with a content-bearing lead-in or just end the preceding sentence with a colon.
- A trailing References section that duplicates links already cited inline.
- Summary lines that restate the paragraph immediately above.

A bridge sentence is justified only when it carries real content: an argumentative claim the list then evidences, a clarification that pre-empts a misreading, or a transition distinguishing two adjacent lists.

## Correctness

Comments must stay accurate as code changes.
A wrong comment is worse than no comment.

- When code is changed, verify and update the comments that describe it in the same change.
- If a comment contradicts the code and you cannot tell which is right, flag it rather than silently "fixing" it to match one side.
- Do not write a comment that restates what the code obviously does. Describe intent, preconditions, side-effects, and non-obvious decisions - not the literal statement.

## What not to reference

Never reference private/local resources (personal memory files, local machine paths, non-repo notes) in anything committed to the repo.
Keep references to repo-internal paths and public external resources only.
