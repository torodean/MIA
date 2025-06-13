#!/bin/bash

# This script is used for installing dependencies and performing
# various setup which is needed for the build and install.

uname_out="$(uname -s)"
case "${uname_out}" in
    Linux*)     platform="linux";;
    CYGWIN*)    platform="cygwin";;
    MINGW*)     platform="mingw";;  # MinGW or Git Bash on Windows
    *)          platform="unknown";;
esac
echo "Platform: $platform"

original_dir="$(pwd)"

if [[ "$uname_out" == "Linux" ]]; then
    echo "Running Linux setup..."
	
	sudo apt-get update
	sudo apt-get install -y cmake
	sudo apt-get install -y libxdo-dev
	sudo apt-get install -y libmysqlcppconn-dev
	sudo apt-get install libx11-dev libxtst-dev

	# Setup gtest for testing
	echo "Installing gtest..."
	sudo apt install libgtest-dev
	cd /usr/src/googletest
	sudo cmake .
	sudo make
	sudo cp lib/libgtest*.a /usr/lib
	sudo mkdir -p /usr/include/gtest
	sudo cp -r googletest/include/gtest /usr/include/
	
	# Return to the original directory
	cd "$original_dir" || exit 1
	
elif [[ "$uname_out" == CYGWIN* ]]; then
    echo "Running Cygwin setup..."
	
	echo "Installing gtest..."	
	cd /tmp
	git clone https://github.com/google/googletest.git
	cd googletest
	mkdir build
	cd build
	cmake ..
	make install
	
	# Return to the original directory
	cd "$original_dir" || exit 1
	
elif [[ "$uname_out" == MINGW* ]]; then
    echo "Running MinGW setup..."
	echo "ERROR: Not yet supported!"
else
    echo "Unknown platform: $uname_out"
fi



