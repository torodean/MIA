#!/bin/bash

# This script is used for installing dependencies and performing
# various setup which is needed for the build and install.

# Load shared platform detection and directory variables.
source "$(cd "$(dirname "$0")" && pwd)/common.sh"

# Enable the repo git hooks (e.g. the commit-msg hook that stamps version bumps).
git config core.hooksPath .githooks

if [[ "$uname_out" == "Linux" ]]; then
    echo "Running Linux setup..."
	
	sudo apt-get update
	sudo apt-get install -y cmake
	sudo apt-get install -y libxdo-dev
	sudo apt-get install -y libmysqlcppconn-dev
	sudo apt-get install libx11-dev libxtst-dev
	sudo apt install nlohmann-json3-dev
	sudo apt install python3-dev
	sudo apt install python3-numpy
	sudo apt install python3-matplotlib
	sudo apt install python3-tk

	# Setup gtest for testing
	bash "$script_dir/setup-gtest.sh"

	# Return to the original directory
	cd "$original_dir" || exit 1

elif [[ "$uname_out" == CYGWIN* ]]; then
    echo "Running Cygwin setup..."

	# Setup gtest for testing
	bash "$script_dir/setup-gtest.sh"

	# Return to the original directory
	cd "$original_dir" || exit 1

elif [[ "$uname_out" == MINGW* ]]; then
    echo "Running MinGW setup..."
	echo "ERROR: Not yet supported!"
else
    echo "Unknown platform: $uname_out"
fi
