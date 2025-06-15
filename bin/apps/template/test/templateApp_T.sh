#!/bin/bash

set -e

# Use the first argument as the path to the built executable
test_executable_name="$1"

# Test 1: Help message
echo "Running help message test..."
output=$(${test_executable_name} -h)
[[ "$output" == *"Usage:"* ]] || { echo "Help message test failed"; exit 1; }

# Test 2: Custom log file
echo "Running log file test..."
log_file="$(pwd)/test_log.txt" # Use absolute path.
rm -f "$log_file"
${test_executable_name} -l "$log_file" -t
[[ -f "$log_file" ]] || { echo "Log file was not created"; exit 1; }
grep -q "run()" "$log_file" || { echo "Expected log contents not found"; exit 1; }
grep -q "loadConfig()" "$log_file" || { echo "Expected log contents not found"; exit 1; }

# Test 3: Custom config file
echo "Running config file test..."
config_file="MIATemplate.MIA"
echo "optionA=value1" > "$config_file"
echo "optionB=value2" >> "$config_file"
output=$(${test_executable_name} -c "$config_file" -t)
# These are the expected values set in the config file. 
[[ "$output" == *"boolValue: true"* ]] || { echo "Bool value not printed correctly"; exit 1; }
[[ "$output" == *"intValue: 314"* ]] || { echo "Int value not printed correctly"; exit 1; }
[[ "$output" == *"doubleValue: 3.141592"* ]] || { echo "Double value not printed correctly"; exit 1; }
[[ "$output" == *"stringValue: Hello World!"* ]] || { echo "String value not printed correctly"; exit 1; }
[[ "$output" == *"listValue: 1 2 3 a b c"* ]] || { echo "List value not printed correctly"; exit 1; }

# Test 4: Verbose mode
echo "Running verbose mode test..."
verbose_output=$(${test_executable_name} -v  -t)
[[ "$verbose_output" == *"Verbose mode enabled"* ]] || { echo "Verbose output missing"; exit 1; }

# Test 5: Ensure verbose output is suppressed without -v
echo "Running non-verbose mode test..."
non_verbose_output=$(${test_executable_name} -t)
[[ "$non_verbose_output" != *"Verbose mode enabled"* ]] || { echo "Unexpected verbose output"; exit 1; }

echo "All tests passed."

