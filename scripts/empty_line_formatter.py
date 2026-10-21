#!/usr/bin/env python3
"""
@file    empty_line_formatter.py
@author  Antonius Torode
@date    10/14/2026
Recursively remove whitespace from otherwise empty lines in files.

Searches every file in the specified directory and its subdirectories.
Lines containing only spaces or tabs are converted to empty lines.
Line endings are preserved, and a message is printed for each modified file.

Usage:
    python3 remove_blank_line_whitespace.py <folder>
    OR
    ./remove_blank_line_whitespace.py <folder>
"""

import argparse
import os


folders_to_ignore = [
    "CARA"
]

valid_file_ext = [
    ".c", ".h", ".cpp", ".hpp",
    ".py",
    ".java",
    ".js",
    ".sh", ".bash",
    ".html", ".htm", ".css",
    ".xml",
    ".json", ".yaml", ".yml",
    ".md", ".rst", ".txt", ".text",
    ".tex", ".bib",
    ".cmake", ".make",
    ".ini", ".cfg", ".conf",
    ".dockerfile", ".gitignore", ".gitattributes",
    ".csv",
    ".lua",
    ".MIA"
]


def process_file(file_path: str) -> None:
    """
    Remove whitespace from lines containing only spaces or tabs.

    Args:
        file_path: Path to the file to process.

    Prints:
        A message identifying the file and the number of lines modified.
    """
    try:
        if not any(file_path.endswith(ext) for ext in valid_file_ext):
            return

        with open(file_path, "r", newline="") as f:
            lines = f.readlines()

        changed_lines = 0

        for i, line in enumerate(lines):
            if line.strip(" \t\r\n") == "" and line.strip(" \t\r\n") != line.rstrip("\r\n"):
                lines[i] = line.lstrip(" \t")
                changed_lines += 1

        if changed_lines > 0:
            with open(file_path, "w", newline="") as f:
                f.writelines(lines)

            print(f"Modified {file_path}: cleared whitespace from {changed_lines} line(s)!")

    except (UnicodeDecodeError, PermissionError, OSError) as e:
        print(f"Skipped {file_path}: {e}")


def process_directory(directory: str) -> None:
    """
    Recursively process every file in a directory and its subdirectories.

    Args:
        directory: Root directory to search.
    """
    for root, dirs, files in os.walk(directory):
        # Prevent os.walk() from descending into ignored directories.
        dirs[:] = [d for d in dirs if d not in folders_to_ignore]

        for file in files:
            process_file(os.path.join(root, file))


def main() -> None:
    """
    Parse command-line arguments and process the specified directory.
    """
    parser = argparse.ArgumentParser(
        description="Remove whitespace from otherwise empty lines in files recursively."
    )
    parser.add_argument(
        "folder",
        help="Root directory to process."
    )
    args = parser.parse_args()

    if not os.path.isdir(args.folder):
        parser.error(f"'{args.folder}' is not a valid directory.")

    process_directory(args.folder)


if __name__ == "__main__":
    main()
