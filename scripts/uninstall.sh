#!/bin/bash

# This script is in charge of performing an uninstall of MIA. This will delete the
# necessary files to ininstall.

# Path to this script
PARENT_DIR="$(dirname "$(dirname "$(readlink -f "$0")")")"

# remove system installed files
rm -vrf /usr/local/mia
rm -vrf /var/log/mia
rm -vrf /etc/mia

# remove build directory
echo "Removing build directory: ${PARENT_DIR}/build"
rm -rf ${PARENT_DIR}/build

rm -vrf ${PARENT_DIR}/release
rm -vrf ${PARENT_DIR}/Testing
