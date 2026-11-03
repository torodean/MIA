#!/bin/bash

# Quickly opens every file git reports as edited (staged, unstaged, and
# untracked; deleted files are skipped) as tabs in a single gedit window,
# for a final review of your changes right before committing. gedit reuses
# its running window automatically. Pass a different editor command as the
# first argument or in the EDITOR_BIN variable (e.g. EDITOR_BIN="vim -p")
# to use something else. Run from anywhere; it always targets the repository
# the script lives in.

# Enable strict error handling, including failures in the collection pipeline.
set -eu -o pipefail

# The editor command used when none is given on the command line.
EDITOR_CMD="${1:-${EDITOR_BIN:-gedit}}"

# Resolve the repository this script lives in (same readlink idiom as
# uninstall.sh) so the script works no matter where it is invoked from.
script_dir="$(dirname "$(readlink -f "$0")")"

# Fail cleanly when the script does not live inside a git repository.
if ! repo_root="$(git -C "$script_dir" rev-parse --show-toplevel 2>/dev/null)"; then
    echo "Error: this script does not live inside a git repository." >&2
    exit 1
fi

# Move to the repository root so the paths printed by git resolve correctly.
cd "$repo_root"

# Collect changed paths from the index, the working tree, and the untracked
# files, skipping deletions (--diff-filter=d), then collapse duplicates into
# a sorted unique list.
mapfile -t files < <(
    {
        git diff --name-only --diff-filter=d
        git diff --cached --name-only --diff-filter=d
        git ls-files --others --exclude-standard
    } | sort -u
)

if [[ ${#files[@]} -eq 0 ]]; then
    echo "No edited files found."
    exit 0
fi

echo "Opening ${#files[@]} file(s) with: $EDITOR_CMD"

# Unquoted on purpose: lets a multi-word command such as "vim -p" split
# into the executable and its flags.
exec $EDITOR_CMD "${files[@]}"
