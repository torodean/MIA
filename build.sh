#!/bin/bash

scriptLocation=$(readlink -f "$0")         # Get the absolute path of the currently running script
rootDirectory=$(dirname "$scriptLocation") # Extract the directory path from the script's full path
echo "rootDirectory: $rootDirectory"       # Print the root directory path to stdout
cd $rootDirectory || exit                  # Change to the root directory, or exit if it fails

# Create the usage output.
usage()
{
  echo "build: build.sh [options...]"
  echo "  This build script will automate the build and install of MIA."
  echo ""
  echo "  Options:"
  echo "    -h    Display this help message."
  echo "    -C    Perform a clean build by removing the build directory first."  
  echo "    -v    Enable verbose output during build process."
  echo "    -D    Attempt to Install dependencies."
  echo "    -u    Update release files on successful build."
  echo "    -I    Install MIA after building."
}

# Define the build script options and create variables from options.
while getopts "hCvDuI" opt; do
  case $opt in
    h) usage
      exit 1
      ;;
    C) cleanBuild=1
      ;;
    v) verboseMode=1
      ;;
    D) installDependencies=1
      ;;
    u) updateReleaseFiles=1
      ;;
    I) installMIA=1
      updateReleaseFiles=1
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

# Place OS specific CMake here.
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  # Linux-GNU
  if [[ $installDependencies ]]; then
    echo "...Installing dependencies for Linux-GNU."
    sudo apt-get update
  fi
elif [[ "$OSTYPE" == "darwin"* ]]; then 
  # Mac OSX.
  echo "Installing dependencies for darwin not supported yet."
elif [[ "$OSTYPE" == "cygwin" ]]; then
  # POSIX compatibility layer and Linux environment emulation for Windows.
  echo "Installing dependencies for cygwin not supported yet."
elif [[ "$OSTYPE" == "msys" ]]; then
  # Lightweight shell and GNU utilities compiled for Windows (part of MinGW).
  echo "Installing dependencies for msys not supported yet."
elif [[ "$OSTYPE" == "win32" ]]; then
  # I'm not sure this can happen.
  echo "Installing dependencies for win32 not supported yet."
elif [[ "$OSTYPE" == "freebsd"* ]]; then
  # FreeBSD OS
  echo "Installing dependencies for freebsd not supported yet."
else
  echo "WARNING: Undetected OS! Please install dependencies manually."
  # Unknown.
fi

if [[ $cleanBuild ]]; then
    echo "...Cleaning build directories."
    rm -rf "$rootDirectory"/build
fi

echo "...Beginning MIA Build!"

mkdir -p "$rootDirectory"/build
cmake -G "Unix Makefiles" -B"$rootDirectory"/build
cd "$rootDirectory"/build || exit
make -j16 || exit

echo "...MIA Build done!"

# Update Release files if specified.
if [[ $updateReleaseFiles ]]; then
  echo "...Updating release files!"
  $rootDirectory/scripts/updateRelease.sh
fi

# Install MIA if specified
if [[ $installMIA ]]; then
  echo "...Installing MIA files!"
  $rootDirectory/scripts/install.sh
fi


