#!/bin/bash

# This script installs Google Test, which is used for the project's tests.
# It can be run on its own or from setup.sh.

# Load shared platform detection and directory variables.
source "$(cd "$(dirname "$0")" && pwd)/common.sh"

if [[ "$platform" == "linux" ]]; then
	echo "Installing gtest..."
	sudo apt install libgtest-dev
	cd /usr/src/googletest || exit 1
	sudo cmake .
	sudo make
	sudo cp lib/libgtest*.a /usr/lib
	sudo mkdir -p /usr/include/gtest
	sudo cp -r googletest/include/gtest /usr/include/
elif [[ "$platform" == "cygwin" ]]; then
	echo "Installing gtest..."
	cd /tmp || exit 1
	git clone https://github.com/google/googletest.git
	cd googletest || exit 1
	mkdir build
	cd build || exit 1
	cmake ..
	make install
elif [[ "$platform" == "mingw" ]]; then
	echo "ERROR: Not yet supported!"
else
	echo "Unknown platform: $uname_out"
fi

# Return to the original directory.
cd "$original_dir" || exit 1
