
#!/bin/bash

root_test_dir="test"
run_test_sh="run_test.sh"

cd ..
cwd=$(pwd)

if [ "$#" -ge 1 ]; then
  ancestor_test_dir="${root_test_dir}/${1}"
else
  ancestor_test_dir="${root_test_dir}"
fi

if [ ! -d "$ancestor_test_dir" ]; then
  echo "Error: The path '${ancestor_test_dir}' does not exist."
  exit 1
fi

test_paths=$(find "$ancestor_test_dir" -type f -name "$run_test_sh" -exec dirname {} \;)

echo ""
echo "Setting up test reference output and errors files ..."
echo "––––––––––––––––––––––––––––––––"

total_updated_tests=0

for test_path in $test_paths; do

  cd "${test_path}"

  correct_stdout_file="correct_output.txt"
  correct_stderr_file="expected_errors.txt"

  echo "${test_path}/${run_test_sh}:"
  printf "Putting output into ${correct_stdout_file} and errors into ${correct_stderr_file}...\r"
  "./$run_test_sh" > "$correct_stdout_file" 2> "$correct_stderr_file"
  if [ $? -eq 0 ]; then
    ((total_updated_tests++))
    printf "Finished putting output into ${correct_stdout_file} and errors into ${correct_stderr_file}.\n"
  else
    printf "Failed in putting output into ${correct_stdout_file} and errors into ${correct_stderr_file}.\n"
  fi
  echo "––––––––––––––––––––––––––––––––"

  cd "${cwd}"

done

echo "Summary:\n--  Tests updated: ${total_updated_tests}  --\n"
