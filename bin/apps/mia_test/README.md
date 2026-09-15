# mia_test

This app is a sandbox for testing and experimenting with various in-development features of the MIA project. Many of the tests here are quick scratch work used to verify that new or in-progress features behave as expected, and are not meant to be polished or permanent.

Tests which prove useful may be organized into their own files and kept around for reference when the features they exercise are folded into the main project.

## Usage

The app is run through the standard MIA app arguments, plus the test app specific options:

- `-H` / `--testHelp`
  Prints the list of available test cases.
- `-i` / `--index`
  Runs the test case at the given index. If no index is given, the app prints the list of test cases and exits.
