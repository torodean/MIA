#!/usr/bin/env python3
"""
@file    repo_metrics.py
@author  Antonius Torode
@date    11/1/2026

The script recursively scans a directory, identifying C++, Python, Shell,
and CMake files. It counts the number of files and non-empty lines for each
file type (excluding blank lines). It automatically excludes the .git and
build directories and other specified directories. The results are displayed
in an table showing file counts and line counts by type, along with overall
totals.
"""

import os
import argparse
from collections import defaultdict

FILE_TYPES = {
    ".cpp": "C++ Source",
    ".hpp": "C++ Header",
    ".sh": "Shell Script",
    ".py": "Python",
    "CMakeLists.txt": "CMake"
}


def count_lines(filepath):
    """
    Counts the number of non-empty lines in a file.

    Args:
        filepath: Path to the file to count.

    Returns:
        Number of lines containing non-whitespace characters.
    """
    with open(filepath, "r", encoding="utf-8", errors="ignore") as file:
        return sum(1 for line in file if line.strip())


def scan_repository(root, excluded):
    """
    Recursively scans a directory and gets the number of files and line-counts
    for specific types of files.

    Excluded directories are skipped. Files are organized by their extensions 
    or filename, and all non-empty lines are counted for each file.

    Args:
        root: Root directory of the directory to scan.
        excluded: Set of directory paths to exclude when scanning.

    Returns:
        Dictionary containing file counts and line counts, organized by file type.
    """
    statistics = defaultdict(lambda: {"files": 0, "lines": 0})

    for directory, subdirectories, files in os.walk(root):
        current_directory = os.path.abspath(directory)

        subdirectories[:] = [
            subdirectory
            for subdirectory in subdirectories
            if os.path.join(current_directory, subdirectory) not in excluded
        ]

        for filename in files:
            extension = os.path.splitext(filename)[1]

            if filename == "CMakeLists.txt":
                file_type = "CMakeLists.txt"
            elif extension in FILE_TYPES:
                file_type = extension
            else:
                continue

            filepath = os.path.join(directory, filename)

            statistics[file_type]["files"] += 1
            statistics[file_type]["lines"] += count_lines(filepath)

    return statistics


def print_statistics(statistics):
    """
    Formats and returns directory stats as a table.

    Args:
        statistics: Dictionary containing file and line counts by file type.

    Returns:
        None.
    """
    headers = ("File Type", "Files", "Lines")

    rows = [
        (FILE_TYPES.get(ext, ext), data["files"], data["lines"])
        for ext, data in statistics.items()
    ]

    rows.sort(key=lambda row: row[0])

    total_files = sum(row[1] for row in rows)
    total_lines = sum(row[2] for row in rows)

    rows.append(("TOTAL", total_files, total_lines))

    widths = [
        max(len(headers[0]), max(len(str(row[0])) for row in rows)),
        max(len(headers[1]), max(len(str(row[1])) for row in rows)),
        max(len(headers[2]), max(len(str(row[2])) for row in rows))
    ]

    def format_row(row):
        return (f"| {row[0]:<{widths[0]}} "
                f"| {row[1]:>{widths[1]}} "
                f"| {row[2]:>{widths[2]}} |" )

    separator = (f"+-{'-' * widths[0]}-"
                 f"+-{'-' * widths[1]}-"
                 f"+-{'-' * widths[2]}-+")

    table = [separator, format_row(headers), separator]

    table.extend(format_row(row) for row in rows)
    table.append(separator)

    return "\n".join(table)


def main():
    """
    Parses command-line arguments and generates directory stats.

    Command-line Arguments:
        -f, --folder:  Repository directory to scan.
        -e, --exclude: Additional directories to exclude, specified
                       relative to the repository root.
        -r, --readme:  README file to update with the generated metrics.

    Returns:
        None.
    """
    parser = argparse.ArgumentParser(
        description="Count source files and lines in a repository."
    )
    parser.add_argument(
        "-f", 
        "--folder",
        default=".",
        help="Repository folder to scan."
    )
    parser.add_argument(
        "-e", 
        "--exclude",
        nargs="+",
        default=[],
        help="Folders to exclude, relative to the repository root."
    )
    parser.add_argument(
        "-r", 
        "--readme",
        default=None,
        help="README file to update, relative to the repository root."
    )

    args = parser.parse_args()

    root = os.path.abspath(args.folder)

    excluded = { os.path.join(root, ".git"), os.path.join(root, "build") }

    excluded.update(
        os.path.abspath(os.path.join(root, folder))
        for folder in args.exclude
    )

    statistics = scan_repository(root, excluded)
    metrics = print_statistics(statistics)
    print(metrics)

    if args.readme:
        readme_path = os.path.abspath(os.path.join(root, args.readme))
        update_readme(readme_path, metrics)


def update_readme(readme_path, metrics):
    """
    Updates the Code Metrics section of a file. This will locates the 
    section using HTML comment delimiters and replaces its existing 
    contents with the provided metrics, preserving the rest of the file.

    Args:
        readme_path: Path to the README file to update.
        metrics: Formatted Markdown table containing repository metrics.

    Returns:
        None.
    """
    begin_marker = "<!-- BEGIN AUTO GENERATED CODE METRICS -->"
    end_marker = "<!-- END AUTO GENERATED CODE METRICS -->"

    with open(readme_path, "r", encoding="utf-8") as file:
        content = file.read()

    start = content.index(begin_marker) + len(begin_marker)
    end = content.index(end_marker)

    content = (content[:start] +"\n" + metrics + "\n" +content[end:])

    with open(readme_path, "w", encoding="utf-8") as file:
        file.write(content)


if __name__ == "__main__":
    main()
