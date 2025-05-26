#!/bin/bash

# This script is used for isntalling dependencies and performing
# various setup which is needed for the build and install.

sudo apt-get update
sudo apt-get install -y cmake
sudo apt-get install -y libxdo-dev
sudo apt-get install libmysqlcppconn-dev


# Setup gtest for testing
sudo apt install libgtest-dev
cd /usr/src/googletest
sudo cmake .
sudo make
sudo cp lib/libgtest*.a /usr/lib
sudo mkdir -p /usr/include/gtest
sudo cp -r googletest/include/gtest /usr/include/

