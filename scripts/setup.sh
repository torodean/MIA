#!/bin/bash

# This script is used for isntalling dependencies and performing
# various setup which is needed for the build and install.

sudo apt-get update
sudo apt-get install -y cmake
sudo apt-get install -y libxdo-dev
sudo apt-get install libmysqlcppconn-dev
