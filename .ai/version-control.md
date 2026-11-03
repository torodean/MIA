# Version control

This file defines one hard rule for any AI working on this repository: the AI never commits.

## The rule

Never create a commit.
Do not run `git commit`, `git commit --amend`, or any command that creates, rewrites, or moves commits (`git rebase`, `git cherry-pick`, `git merge`, `git revert`, and GUI or script equivalents such as `commitAll.sh`).
This holds even when a build or tests pass, even when the task seems finished, and even when an earlier message in the same conversation seemed to allow it.

## What to do instead

Leave every change in the working tree for the user to review, stage, and commit.
Do not stage files with `git add` either; staging is part of the user's commit decision.
When the work is done, summarize the changes and offer a suggested commit message as plain text; the user decides if and when to use it.

## Allowed git use

Read-only inspection is always fine: `git status`, `git diff`, `git log`, `git show`, `git blame`.
Any other git command requires the user's explicit request in the current conversation.

## Why

The user owns the repository history, staging, commit messages, and version-number flow (the `commitAll.sh` scripts bump versions on commit).
An AI commit steals that decision and is much harder to undo than an uncommitted change.
