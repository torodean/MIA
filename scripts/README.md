# Scripts Directory

This folder contains utility scripts used for managing, deploying, and maintaining the project environment and release lifecycle.

## Script Descriptions

### `common.sh`
Shared variables and platform detection sourced by the other scripts in this directory. Not meant to be run directly.

### `empty_line_formatter.py`
Recursively removes whitespace from otherwise empty lines in files. This script will parse a folder and all subfolders for empty lines throughout files. The files are specified by specific file extensions.

### `increment_version.py`
Automatically increments the documentation and application's version number. Typically used as part of the release pipeline to ensure versioning consistency. This script is called when the commitAll.sh script in the top level directory is ran.

### `install.sh`
Handles extra installation features not handled via cmake.

### `open_changes.sh`
Quickly opens all files changed according to git status (staged, unstaged, and untracked) as tabs in a single gedit window, so you can do a final review of your edits right before committing. Can be run from anywhere in the repository.

### `repo_metrics.py`
This is a simple script that gathers some code metrics about the MIA repo. 
It will also optionally updated the README with those metrics, which is automatically done when the `commitAll.sh` script is used.

### `setup.sh`
This script handles installing some dependancies used by the MIA project and other initial setup steps.

### `setup_gtest.sh`
Installs Google Test, which is used by the project's tests. Run on its own or from `setup.sh`.

### `uninstall.sh`
Reverses the actions of `install.sh` by removing installed files or dependencies. Also performs various other uninstall processes to delete files installed via cmake.

### `updateRelease.sh`
Handles updating release-related files and artifacts.

### `CARA`
This is the CARA project used for changelog generation. This is included as a submodule so that the latest version of the file(s) can be used.
