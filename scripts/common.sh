#!/bin/bash

# This file holds setup shared by the scripts in this directory. It is not
# meant to be run directly; other scripts source it and use its variables.

# Detect the platform from the operating system name.
uname_out="$(uname -s)"
case "${uname_out}" in
    Linux*)     platform="linux";;
    CYGWIN*)    platform="cygwin";;
    MINGW*)     platform="mingw";;  # MinGW or Git Bash on Windows
    *)          platform="unknown";;
esac
echo "Platform: $platform"

# The directory the script was started from, and the directory this file is in.
original_dir="$(pwd)"
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
