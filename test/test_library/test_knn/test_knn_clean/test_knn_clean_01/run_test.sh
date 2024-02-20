
#!/bin/bash

dir_name=$(basename "$PWD")
test_exec="./${dir_name}_executable"

$test_exec

leaks_output=$(mktemp)
leaks --atExit -- $test_exec > $leaks_output 2>&1
if grep -q "0 leaks for 0 total leaked bytes." $leaks_output; then
	echo "Program had no memory leak."
else
	echo "Program had memory leak."
fi
rm $leaks_output

