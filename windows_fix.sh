##################################################################
# This file is meant to fix the file endings for all of the shell files in Cygwin (when running on windows).
# This is needed because of the limitations and inefficiencies of Windows systems.
# When running the build script, if you see the following errors or related errors, this command needs ran.
# $ ./build.sh -CvT
# ./build.sh: line 2: $'\r': command not found
# rootDirectory: /cygdrive/s/projects/git/MIA
# ./build.sh: line 7: $'\r': command not found
# ./build.sh: line 9: syntax error near unexpected token `$'\r''
# '/build.sh: line 9: `usage()
##################################################################

find . -type f -name '*.sh' -exec sed -i 's/\r$//' {} +