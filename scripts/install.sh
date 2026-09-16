#!/bin/bash

# This script is in charge of performing install options beyong what is done during the
# cmake build. This can include adding configuration files or changing config values.

# Load shared platform detection and directory variables.
source "$(cd "$(dirname "$0")" && pwd)/common.sh"

if [[ "$uname_out" == "Linux" ]]; then
    echo "Running Linux install..."
	sudo mkdir -p /var/log/mia
	sudo touch /var/log/mia/MIA.log
	sudo chmod a+rw /var/log/mia/MIA.log
	
elif [[ "$uname_out" == CYGWIN* ]]; then
    echo "Running Cygwin install..."

elif [[ "$uname_out" == MINGW* ]]; then
    echo "Running MinGW install..."
	echo "ERROR: Not yet supported!"
else
    echo "Unknown platform: $uname_out"
fi
