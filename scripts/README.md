# Scripts Directory

This folder contains utility scripts used for managing, deploying, and maintaining the project environment and release lifecycle.

## Script Descriptions

### `increment_version.py`
Automatically increments the documentation and application's version number. Typically used as part of the release pipeline to ensure versioning consistency. This script is called when the commitAll.sh script in the top level directory is ran.

### `install.sh`
Handles extra installation features not handled via cmake.

### `setup.sh`
This script handles installing some dependancies used by the MIA project and other initial setup steps.

### `uninstall.sh`
Reverses the actions of `install.sh` by removing installed files or dependencies. Also performs various other uninstall processes to delete files installed via cmake.

### `updateRelease.sh`
Handles updating release-related files and artifacts.

### `CARA`
This is the CARA project used for changelog generation. This is included as a submodule so that the latest version of the file(s) can be used.

---

These scripts streamline the development and deployment workflow, reducing manual effort and ensuring repeatability.
