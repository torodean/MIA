#!/bin/python3
"""
@file    comment_cheecker.py
@author  Antonius Torode
@date    2025-07-13

A script to verify and correct file headers and method docstrings in C++ source/header files.

Features:
- Ensures headers use @file, @author, @date tags
- Converts legacy 'File:', 'Author:', and date formats to standard tags
- Checks method declarations for proper docstrings including @param and @return tags
- Supports .cpp and .hpp files
- Prints modifications in colored output for easy identification

Usage:
    python3 comment_cheecker.py [options] <directory>

Options:
    --help      Show this help message and exit
"""
import os
import re
import termcolor
import argparse
from pathlib import Path

def check_file_header(file_path, lines):
    """
    Check if the file has a proper header with @file and @author, and replace 'File:' with 
    '@file', 'Author:' with '@author', and 'Date:' or 'Created on:' with '@date' if found.
    
    This function scans the file for a header comment starting with '/**' and ending with 
    '*/'. It checks for the presence of '@file' followed by the file name and '@author' 
    followed by a name. If 'File:' or 'Author:' are found, they are replaced with '@file' 
    and '@author', respectively, and the changes are saved to the file. Modified lines are 
    printed in green text.
    
    @param file_path[str] - The path to the file being checked.
    @param lines[list] - The list of lines in the file.
    @return [list] - A list of strings describing any issues found with the header.
    """
    issues = []
    header_found = False
    file_name = os.path.basename(file_path)
    modified = False
    modified_lines = lines.copy()  # Create a copy to modify
    
    for i, line in enumerate(lines, 1):
        if line.rstrip('\n').startswith("/**"):
            header_found = True
            header_lines = []
            header_start_idx = i - 1
            while i < len(lines) and not lines[i-1].rstrip('\n').endswith("*/"):
                header_lines.append(lines[i-1].rstrip('\n'))
                i += 1
            header_lines.append(lines[i-1].rstrip('\n'))  # Include the closing '*/'
            
            header_text = " ".join(header_lines)
            original_header = header_lines.copy()
            
            # Check and replace 'File:' with '@file'
            for j, h_line in enumerate(header_lines):
                if re.search(r"File:\s*" + re.escape(file_name), h_line, re.IGNORECASE):
                    header_lines[j] = h_line.replace("File:", "@file")
                    modified_lines[header_start_idx + j] = header_lines[j] + "\n"
                    modified = True
                elif re.search(r"File:", h_line, re.IGNORECASE):
                    issues.append(f"{file_path}:{header_start_idx + j + 1}: Incorrect file name in '@file' (expected '@file {file_name}')")
            
            # Normalize @file:, @author:, @date: → remove colon (with or without space)
            for j, h_line in enumerate(header_lines):
                updated_line = re.sub(r"(@file|@date|@author)\s*:", r"\1", h_line)
                if updated_line != h_line:
                    header_lines[j] = updated_line
                    modified_lines[header_start_idx + j] = updated_line + "\n"
                    modified = True
        
            # Check and replace 'Author:' with '@author'
            for j, h_line in enumerate(header_lines):
                if re.search(r"Author:\s*[\w\s]+", h_line, re.IGNORECASE):
                    header_lines[j] = h_line.replace("Author:", "@author")
                    modified_lines[header_start_idx + j] = header_lines[j] + "\n"
                    modified = True
                    
            date_patterns = [r"Creation date:", r"Date:", r"Created on:"]
            date_regex = re.compile(r"(" + "|".join(date_patterns) + r")\s*(.+)", re.IGNORECASE)

            for j, h_line in enumerate(header_lines):
                if "@date" in h_line:
                    continue  # skip any line that already uses @date
                match = date_regex.search(h_line)
                if match:
                    # Replace the matched prefix with '@date' and keep the rest of the line
                    header_lines[j] = date_regex.sub(r"@date \2", h_line)
                    modified_lines[header_start_idx + j] = header_lines[j] + "\n"
                    modified = True
            
            # Re-check header after modifications
            header_text = " ".join(header_lines)
            if not re.search(r"@file\s+" + re.escape(file_name), header_text, re.IGNORECASE):
                issues.append(f"{file_path}:{header_start_idx + 1}: Missing or incorrect @file in header (expected '@file {file_name}')")
            if not re.search(r"@author\s+[\w\s]+", header_text, re.IGNORECASE):
                issues.append(f"{file_path}:{header_start_idx + 1}: Missing @author in header")
            
            # Print modified lines in green
            if modified:
                print(termcolor.colored(f"Modified {file_path}:", "green"))
                for j, (orig, new) in enumerate(zip(original_header, header_lines)):
                    if orig != new:
                        print(termcolor.colored(f"Line {header_start_idx + j + 1}: {orig} -> {new}", "green"))
                
                # Write modified lines back to the file
                with open(file_path, "w", encoding="utf-8") as f:
                    f.writelines(modified_lines)
            
            break
    
    if not header_found:
        issues.append(f"{file_path}:1: No header comment found")
    
    return issues


def is_docstring_start(line):
    """
    Check if a line starts a docstring.
    
    @param line[str] - The line to check.
    @return [bool] - True if the line starts a docstring, False otherwise.
    """
    return line.strip().startswith("/**")


def is_docstring_end(line):
    """
    Check if a line ends a docstring.
    
    @param line[str] - The line to check.
    @return [bool] - True if the line ends a docstring, False otherwise.
    """
    return line.strip().endswith("*/")


def is_method_declaration(line):
    """
    Check if a line is a method declaration.
    
    Identifies method signatures by looking for a word followed by '()'. The word(s) 
    before the method name are assumed to be the return type (if present). Parameters 
    are parsed as pairs of words (type, name) separated by commas, handling multi-word 
    types like char*[]. Supports constructors, destructors, and methods with const, 
    override, final, or = default qualifiers.
    
    @param line[str] - The line to check.
    @return [tuple] - (bool, str, str) indicating if it's a method, the return type (or 
        empty for constructors/destructors), and the full signature.
    """
    stripped = line.strip()
    
    # Split the line into words, preserving parentheses and commas
    words = stripped.replace('(', ' ( ').replace(')', ' ) ').replace(',', ' , ').split()
    
    # Find the method name: a word followed by ( or ()
    method_name_idx = -1
    for i, word in enumerate(words):
        if word == '(' and i > 0:
            method_name_idx = i - 1
            break
    
    if method_name_idx == -1 or words[method_name_idx + 1] != '(':
        return False, "", ""
    
    method_name = words[method_name_idx]
    
    # Check if it's a destructor or constructor (no return type)
    is_destructor = method_name.startswith('~')
    is_constructor = method_name_idx > 0 and words[method_name_idx - 1].endswith('::') and words[method_name_idx - 1].replace('::', '') == method_name
    
    # Extract return type (word(s) before method name, if not constructor/destructor)
    return_type = ""
    if not (is_destructor or is_constructor):
        return_type_words = []
        for i in range(method_name_idx - 1, -1, -1):
            if words[i] in ('inline', 'const', 'override', 'final', '= default'):
                break
            return_type_words.insert(0, words[i])
        return_type = ' '.join(return_type_words) if return_type_words else ""
    
    # Extract parameters between ( and )
    param_start_idx = method_name_idx + 1
    param_end_idx = words.index(')', param_start_idx) if ')' in words[param_start_idx:] else -1
    if param_end_idx == -1:
        return False, "", ""
    
    params = []
    i = param_start_idx + 1
    while i < param_end_idx:
        if i + 1 < param_end_idx:
            param_type_words = [words[i]]
            i += 1
            while i < param_end_idx and words[i] not in (',', words[i + 1] if i + 1 < param_end_idx else ''):
                param_type_words.append(words[i])
                i += 1
            if i < param_end_idx:
                param_name = words[i]
                params.append((' '.join(param_type_words), param_name))
                i += 2 if i + 1 < param_end_idx and words[i + 1] == ',' else 1
            else:
                break
        else:
            break
    
    # Reconstruct the signature up to the end of the declaration
    signature_end_idx = param_end_idx + 1
    while signature_end_idx < len(words) and words[signature_end_idx] in ('const', 'override', 'final', '= default'):
        signature_end_idx += 1
    signature = ' '.join(words[:signature_end_idx]).replace(' , ', ', ').replace(' ( ', '(').replace(' ) ', ')')
    if stripped.endswith(';') or stripped.endswith('}'):
        signature += stripped[-1]
    
    # Verify it's a method (has () and reasonable structure)
    if '(' not in signature or ')' not in signature:
        return False, "", ""
    
    return True, return_type, signature


def extract_parameter_types(signature):
    """
    Extract parameter types from a method signature.
    
    @param signature[str] - The method signature.
    @return [list] - A list of parameter types.
    """
    param_types = []
    param_match = re.search(r"\(([^)]*)\)", signature)
    if param_match:
        param_str = param_match.group(1).strip()
        if param_str:
            param_list = []
            current_param = ""
            bracket_count = 0
            for char in param_str + ",":
                if char == "," and bracket_count == 0:
                    param_list.append(current_param.strip())
                    current_param = ""
                else:
                    current_param += char
                    if char in "<[":
                        bracket_count += 1
                    elif char in ">]":
                        bracket_count -= 1
            for param in param_list:
                param_parts = re.match(r"^(.*?[\w*]+(?:\s*\[\])?)\s+\w+\s*(?:=.*)?$", param.strip())
                if param_parts:
                    param_types.append(param_parts.group(1).strip())
    return param_types


def format_colored_signature(signature, return_type, param_types):
    """
    Format a method signature with line number in blue and types in red.
    
    @param signature[str] - The method signature.
    @param return_type[str] - The return type of the method.
    @param param_types[list] - List of parameter types.
    @return [str] - The formatted signature string.
    """
    colored_signature = signature
    if return_type:
        colored_signature = colored_signature.replace(return_type, termcolor.colored(return_type, "red"))
    for p_type in param_types:
        if p_type:
            colored_signature = colored_signature.replace(p_type, termcolor.colored(p_type, "red"))
    return colored_signature


def extract_methods_and_docstrings(lines):
    """
    Extract methods and their preceding docstrings from the lines.
    
    This function parses the input lines to identify method declarations and their associated 
    docstrings. It prints each method's line number in blue and parameter/return types in red 
    as it processes them. Method declarations include class methods, inline methods, destructors, 
    and methods with override, final, or default qualifiers.
    
    @param lines[list] - The list of lines from the file to parse.
    @return [list] - A list of dictionaries containing method info (docstring, method signature, 
        and line numbers).
    """
    methods = []
    current_docstring = []
    in_docstring = False
    in_method = False
    method_lines = []
    docstring_start_line = 0
    method_start_line = 0

    for i, line in enumerate(lines, 1):
        stripped = line.strip()

        if is_docstring_start(stripped) and not in_docstring and not in_method:
            in_docstring = True
            docstring_start_line = i
            current_docstring = [stripped]
            continue

        if in_docstring:
            current_docstring.append(stripped)
            if is_docstring_end(stripped):
                in_docstring = False
            continue

        if not in_docstring and not in_method:
            is_method, return_type, signature = is_method_declaration(stripped)
            if is_method:
                in_method = True
                method_start_line = i
                method_lines = [stripped]
                
                # Print line number in blue, types in red
                param_types = extract_parameter_types(signature)
                colored_signature = format_colored_signature(signature, return_type, param_types)
                print(termcolor.colored(f"Line {i}", "blue") + f": {colored_signature}")
                continue

        if in_method:
            method_lines.append(stripped)
            if stripped.endswith("}") or stripped.endswith(";"):
                in_method = False
                method_signature = " ".join(method_lines).strip()
                methods.append({
                    "docstring": current_docstring if current_docstring else [],
                    "method": method_signature,
                    "docstring_line": docstring_start_line,
                    "method_line": method_start_line
                })
                current_docstring = []
                method_lines = []
            continue

    return methods


def extract_parameters(method):
    """
    Extract parameters from a method signature.
    
    Parses the method signature to extract parameter names and types, handling complex types 
    like pointers and arrays.
    
    @param method[str] - The method signature to parse.
    @return [list] - A list of tuples (param_type, param_name) for each parameter.
    """
    params = []
    param_match = re.search(r"\(([^)]*)\)", method)
    if not param_match:
        return params
    
    param_str = param_match.group(1).strip()
    if not param_str:
        return params
    
    # Split parameters, handling types with spaces, pointers, and arrays
    param_list = []
    current_param = ""
    bracket_count = 0
    for char in param_str + ",":
        if char == "," and bracket_count == 0:
            param_list.append(current_param.strip())
            current_param = ""
        else:
            current_param += char
            if char in "<[":
                bracket_count += 1
            elif char in ">]":
                bracket_count -= 1
    params = [p.strip() for p in param_list if p.strip()]
    
    result = []
    for param in params:
        param_parts = re.match(r"^(.*?[\w*]+(?:\s*\[\])?)\s+(\w+)\s*(?:=.*)?$", param)
        if param_parts:
            param_type, param_name = param_parts.groups()
            result.append((param_type.strip(), param_name))
    
    return result


def is_constructor(method, file_path):
    """
    Determine if a method is a constructor.
    
    Checks if the method name matches the class name (ClassName::ClassName) or the file name 
    (for .hpp files).
    
    @param method[str] - The method signature to check.
    @param file_path[str] - The path to the file containing the method.
    @return [bool] - True if the method is a constructor, False otherwise.
    """
    method_name_match = re.search(r"(\w+)\s*\([^)]*\)", method)
    if not method_name_match:
        return False
    method_name = method_name_match.group(1)
    
    # Check for ClassName::ClassName
    if re.search(r"(\w+)::\1\s*\(", method):
        return True
    
    # Check if method name matches file name (for .hpp files, parameterless or not)
    file_name = file_path.split('/')[-1].replace('.hpp', '')
    param_match = re.search(r"\(([^)]*)\)", method)
    is_parameterless = param_match and not param_match.group(1).strip()
    return method_name == file_name and file_path.endswith('.hpp')


def check_parameters(file_path, method, docstring_text, docstring_line, params):
    """
    Check if parameters are properly documented in the docstring.
    
    Verifies that each parameter has a '@param name[type] - Description' entry matching the 
    declared type.
    
    @param file_path[str] - The path to the file being checked.
    @param method[str] - The method signature.
    @param docstring_text[str] - The joined docstring text.
    @param docstring_line[int] - The line number of the docstring.
    @param params[list] - List of (param_type, param_name) tuples.
    @return [list] - A list of strings describing parameter-related issues.
    """
    issues = []
    for param_type, param_name in params:
        escaped_param_type = re.escape(param_type)
        expected_param_doc = f"@param\\s+{param_name}\\[{escaped_param_type}\\]\\s*-"
        if not re.search(expected_param_doc, docstring_text):
            issues.append(f"{file_path}:{docstring_line}: Missing or incorrect @param for '{param_name}' in method '{method}' (expected '@param {param_name}[{param_type}] - ...')")
    return issues


def check_return_type(file_path, method, docstring_text, docstring_line):
    """
    Check if a non-void, non-constructor method has a proper @return tag.
    
    Verifies that non-void methods have '@return [returnType] - Description' in the docstring.
    
    @param file_path[str] - The path to the file being checked.
    @param method[str] - The method signature.
    @param docstring_text[str] - The joined docstring text.
    @param docstring_line[int] - The line number of the docstring.
    @return [list] - A list of strings describing return-type-related issues.
    """
    issues = []
    return_type_match = re.match(r"^(inline\s+)?(?:([\w*:]+(?:<[\w\s,]+>)?(?:\s*\[\])?)\s+)?\w+\s*\([^)]*\)\s*(const)?\s*(override)?", method)
    is_void = return_type_match and return_type_match.group(2) == "void"
    if not is_void and not re.search(r"@return\s+\[\w[\w\s:<>\*]*\]\s*-", docstring_text):
        issues.append(f"{file_path}:{docstring_line}: Missing @return with type for non-void method '{method}' (expected '@return [returnType] - ...')")
    return issues


def check_docstrings(file_path, methods):
    """
    Check if methods in .hpp files have proper docstrings with @param and @return.
    
    This function verifies that each method has a docstring, that all parameters are documented 
    with '@param name[type] - Description' where type matches the declared type, and that non-void, 
    non-constructor methods have '@return [returnType] - Description'. It skips docstring checks 
    for .cpp files and ignores constructors when checking for @return.
    
    @param file_path[str] - The path to the file being checked.
    @param methods[list] - A list of dictionaries containing method info (docstring, method signature, 
    and line numbers).
    @return [list] - A list of strings describing any issues found with the docstrings.
    """
    issues = []
    
    for method_info in methods:
        docstring = method_info["docstring"]
        method = method_info["method"]
        docstring_line = method_info["docstring_line"]
        method_line = method_info["method_line"]
        
        if not docstring:
            issues.append(f"{file_path}:{method_line}: Method '{method}' has no docstring")
            continue
        
        # Join docstring lines for easier regex processing
        docstring_text = " ".join(docstring)
        
        # Extract and check parameters
        params = extract_parameters(method)
        issues.extend(check_parameters(file_path, method, docstring_text, docstring_line, params))
        
        # Check return type if not a constructor
        if not is_constructor(method, file_path):
            issues.extend(check_return_type(file_path, method, docstring_text, docstring_line))
    
    return issues


def has_pragma_once(file_path, lines):
    """
    Check if a header file contains '#pragma once'.

    @param file_path[str] - The path to the header file.
    @param lines[list] - The list of lines from the file.
    @return [list] - A list with a string describing the issue if '#pragma once' is missing, empty if present.
    """
    for i, line in enumerate(lines, 1):
        if line.strip() == "#pragma once":
            return []
    
    red_msg = termcolor.colored("Missing '#pragma once' directive", 'red')
    return [f"{file_path}:1: {red_msg}"]



def check_files_in_directory(directory):
    """
    Check .cpp and .hpp files in the directory and subdirectories for header compliance, and .hpp 
    files for docstring compliance.
    
    This function iterates through all .cpp and .hpp files in the specified directory and its 
    subdirectories. It checks file headers for proper '@file' and '@author' tags in all files, 
    and checks method docstrings for proper '@param' and '@return' tags in .hpp files only.
    
    @param directory[str] - The directory to check for .cpp and .hpp files.
    @return [list] - A list of strings describing any issues found with headers or docstrings.
    """
    issues = []
    
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith((".cpp", ".hpp")):
                file_path = os.path.join(root, file)
                with open(file_path, "r", encoding="utf-8") as f:
                    lines = f.readlines()
                
                # Check file header for all files
                issues.extend(check_file_header(file_path, lines))
                
                # TODO - this doesn't quite work yet.
                # Check docstrings only for .hpp files
                if file_path.endswith('.hpp'):
                    issues.extend(has_pragma_once(file_path, lines))
                #    methods = extract_methods_and_docstrings(lines)
                #    issues.extend(check_docstrings(file_path, methods))
    
    return issues


def main():
    """
    Parse command-line arguments and check C++ files for docstring compliance.
    
    This function uses argparse to accept a directory path as a command-line argument,
    then checks all .cpp and .hpp files in that directory and its subdirectories for
    proper docstrings and header comments.
    
    @return [None] - Prints issues or a success message to the console.
    """
    parser = argparse.ArgumentParser(description="Check C++ files for proper docstrings and header comments.")
    parser.add_argument('-d', "--directory", nargs="?", default="./", 
                       help="Directory to check for .cpp and .hpp files (default: current directory)")
    args = parser.parse_args()
    
    issues = check_files_in_directory(args.directory)
    
    if issues:
        print("\nIssues found:")
        print("---------------")
        for issue in issues:
            print(issue)
    else:
        print(termcolor.colored("\nNo issues found. All files comply with docstring standards!", "green"))
        print()


if __name__ == "__main__":
    main()
