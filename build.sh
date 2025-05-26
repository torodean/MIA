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
  echo "    -S    Run the initial setup to install dependancies and such."
  echo "    -C    Perform a clean build by removing the build directory first."  
  echo "    -v    Enable verbose output during build process."
  echo "    -D    Attempt to Install dependencies."
  echo "    -I    Install MIA after building (requires admin). Use a clean build if errors occur."
  echo "    -R    Update the release files. Use a clean build if errors occur."
  echo "    -U    Uninstall all MIA files. This will uninstall then quit without other actions."
  echo "    -T    Run all tests."
  echo "    -d    Add flags to cmake for a debug build (useful if gdb is needed)."
}

# Define the build script options and create variables from options.
while getopts "hCvDIRUTd" opt; do
  case $opt in
    h) usage
      exit 1
      ;;
    S) runSetup=1
      ;;
    C) cleanBuild=1
      ;;
    v) verboseMode=1
      ;;
    D) installDependencies=1
      ;;
    I) installMIA=1
      releaseMode=1
      ;;
    R) updateReleaseFiles=1
      releaseMode=1
      ;;
    U) uninstallMIA=1
      ;;
    T) enableTesting=1
      ;;
    d) debugMode=1
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

# Perform an uninstall of MIA then exit.
if [[ $uninstallMIA ]]; then
  echo "...Uninstalling MIA!"
  sudo $rootDirectory/scripts/uninstall.sh
  echo "...Exiting!"
  exit 0
fi

# Run the setup script for installing dependancies.
if [[ $runSetup ]]; then
  echo "Installing dependancies!"
  sudo $rootDirectory/scripts/setup.sh
  echo "...Done!"
fi

# The arguments to pass to cmake.
cmakeArgs=""

# Place OS specific CMake here.
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  # Linux-GNU
  if [[ $installDependencies ]]; then
    echo "...Installing dependencies for Linux-GNU."
    $rootDirectory/scripts/setup.sh
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

# Update Release files if specified.
if [[ $updateReleaseFiles ]]; then
  echo "...Updating release files!"
  cmakeArgs="$cmakeArgs -DRELEASE_BUILD=ON"
elif [[ $installMIA ]]; then
  cmakeArgs="$cmakeArgs -DSYSTEM_INSTALL=ON"
fi

if [[ $debugMode ]]; then
  cmakeArgs="$cmakeArgs -DCMAKE_BUILD_TYPE=Debug"
fi

if [[ $releaseMode ]]; then
  cmakeArgs="$cmakeArgs -DCMAKE_BUILD_TYPE=Release"
fi

echo "...Beginning MIA Build!"

mkdir -p "$rootDirectory"/build
cmake -G "Unix Makefiles" -B "$rootDirectory"/build $cmakeArgs
cd "$rootDirectory"/build || exit
make -j16 || exit

echo "...MIA Build done!"

# Install MIA if specified
if [[ -z $installMIA && -n $updateReleaseFiles ]]; then
  echo "...Updating Release files!"
  cmake --install "$rootDirectory"/build
  $rootDirectory/scripts/install.sh
elif [[ $installMIA ]]; then
  echo "...Installing MIA files!"
  sudo cmake --install "$rootDirectory"/build
  $rootDirectory/scripts/install.sh
fi

#Run tests if specified
if [[ $enableTesting ]]; then
  echo "Running the MIA tests!"
  cd "$rootDirectory"/build
  if [[ $verboseMode ]]; then
    ctest -v --output-on-failure
  else
    ctest -v
  fi
  cd -
  echo "Finished with tests!"
fi



