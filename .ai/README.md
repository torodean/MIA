# .ai

This folder contains task files for an AI working on the MIA codebase.
Each file is a self-contained set of instructions for one concern.

- [comment-formatting.md](comment-formatting.md) - How to parse files for correct comment structure and content, and how to format new comments across C++, CMake, shell, Python, TeX, and Markdown.
  It references [writing-style.md](writing-style.md) for wording and [../docs/TeX_files/CodingStandards.tex](../docs/TeX_files/CodingStandards.tex) for the canonical code-level standard.
- [writing-style.md](writing-style.md) - Tone, wording, grammar, punctuation, and whitespace preferences for everything an AI produces (comments, commit messages, docs, replies).
  These rules constrain AI output, not a human author's own writing.
- [library-opportunities.md](library-opportunities.md) - When an AI looks at a file for any reason, scan for methods that belong in a utility or library (especially internal-namespace ones) and for logic that duplicates an existing utility, and flag the opportunities to the user rather than making the changes.
- [readme-sync.md](readme-sync.md) - When a folder's files or features change, check whether its README is still in sync and either propose a specific edit for the user to confirm or flag the gap when the change is not clear enough to write.

The MIA codebase is not yet fully standardized; these files describe the target conventions.
When a file here and [../docs/TeX_files/CodingStandards.tex](../docs/TeX_files/CodingStandards.tex) disagree, the .tex is authoritative.
