
#!/bin/bash



# Before running test(s), build all tests via make

cd ../make
echo "\nBuilding all tests:\n"
make test
if [ $? -ne 0 ]; then
  echo "Error: Failed to build tests."
  exit 1
fi



cd ..
cwd=$(pwd)
root_test_dir="test"
run_test_sh="run_test.sh"

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

results_folder="${ancestor_test_dir}/results"
mkdir -p "${results_folder}"
current_datetime_for_file=$(date +"%Y-%m-%d_%H-%M-%S")
current_datetime_for_text=$(date +"%Y-%m-%d %H:%M:%S")
results_file="${results_folder}/results_${current_datetime_for_file}.txt"
results_file_path="${cwd}/${results_file}"

echo "\nTesting started on ${current_datetime_for_text}." | tee -a "${results_file_path}"
echo "––––––––––––––––––––––––––––––––" | tee -a "${results_file_path}"

total_tests=0
total_successes=0
total_failures=0

for test_path in $test_paths; do

  cd "${test_path}"

  stdout_temp=$(mktemp)
  stderr_temp=$(mktemp)
  correct_stdout_file="correct_output.txt"
  correct_stderr_file="expected_errors.txt"

  does_test_pass=1

  echo "${test_path}/${run_test_sh}:\nTesting..." | tee -a "${results_file_path}"

  "./$run_test_sh" > "$stdout_temp" 2> "$stderr_temp"

  if [ ! -e "$correct_stdout_file" ]; then
    echo "File ${correct_stdout_file} does not exist." | tee -a "${results_file_path}"
    does_test_pass=0
  elif [ "$(cat "$stdout_temp")" != "$(cat "$correct_stdout_file")" ]; then
    echo "Test standard output does not match with ${correct_stdout_file}." | tee -a "${results_file_path}"
    does_test_pass=0
  fi
  rm "$stdout_temp"

  if [ ! -e "$correct_stderr_file" ]; then
    echo "File ${correct_stderr_file} does not exist." | tee -a "${results_file_path}"
    does_test_pass=0
  elif [ "$(cat "$stderr_temp")" != "$(cat "$correct_stderr_file")" ]; then
    echo "Test standard error does not match with ${correct_stderr_file}." | tee -a "${results_file_path}"
    does_test_pass=0
  fi
  rm "$stderr_temp"
  
  if [ $does_test_pass -eq 1 ]; then
    ((total_successes++))
    echo "Passed!" | tee -a "${results_file_path}"
  else
    ((total_failures++))
    echo "Failed." | tee -a "${results_file_path}"
  fi
  ((total_tests++))

  echo "––––––––––––––––––––––––––––––––" | tee -a "${results_file_path}"

  cd "${cwd}"

done

echo "Summary:\n--  Total: ${total_tests}  --  Passed: ${total_successes}  --  Failed: ${total_failures}  --\n" | tee -a "${results_file_path}"

echo "The above test results are stored in ${results_file}.\n"
