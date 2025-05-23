#!/bin/sh

scriptLocation=$(readlink -f "$0")
rootDirectory=$(dirname "$scriptLocation")
echo "rootDirectory: $rootDirectory"

usage()
{
  echo "updateRelease: updateRelease.sh [options...]"
  echo "  This will update release files and executables based on a build."
  echo ""
  echo "  Options:"
  echo "    -W    Specify Windows."
  echo "    -L    Specify Linux."
}

while getopts "hWL" opt; do
  case $opt in
    h) usage
      ;;
    W) usingWindows=1
      ;;    
    L) usingLinux=1
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



echo "Removing old release download."
rm -vf $rootDirectory/MIA\ release.tar.gz
echo "...done!"
echo "Updating executable(s)."
if [[ $usingWindows ]]; then
  # Don't remove the built Linux executables.
  rm -vf $rootDirectory/release/MIA*.exe
elif [[ $usingLinux ]]; then
  # Don't remove the built Windows executables.
  rm -vf $rootDirectory/release/MIA
  rm -vf $rootDirectory/release/MIATerminal
else
  rm -vf $rootDirectory/release/MIA*
fi
rm -vf $rootDirectory/release/MIAManual.pdf
cp -uv $rootDirectory/build/bin/MIA.exe $rootDirectory/release/MIA.exe
cp -uv $rootDirectory/build/bin/MIA $rootDirectory/release/MIA
cp -uv $rootDirectory/build/bin/terminal/MIATerminal.exe $rootDirectory/release/MIATerminal.exe
cp -uv $rootDirectory/build/bin/terminal/MIATerminal $rootDirectory/release/MIATerminal
echo "...done!"
echo "Updating manual."
rm -vf $rootDirectory/release/MIAManual.pdf
cp $rootDirectory/documentation/MIAManual.pdf $rootDirectory/release/MIAManual.pdf
echo "...done!"
echo "Updating dependencies."
rm -vrf $rootDirectory/release/resources
cp -vr $rootDirectory/bin/resources $rootDirectory/release/resources
cp -vr $rootDirectory/dlls/* $rootDirectory/release/
echo "...done!"
echo "Creating compressed release file."
tar -czvf $rootDirectory/MIA-release.tar.gz $rootDirectory/release/
echo "...done!"