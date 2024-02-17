
#!/bin/bash

if [ ! $# -eq 1 ]; then
  echo "Error: Must provide a path local to 'test/' ending with the wanted test folder name."
  echo "       Examples: test_01, testsuite_01/test_01, test_library/testsuite_classifier/test_init"
  exit 1
fi

cd ../test
test_folder_path="$1"
current_datetime=$(date +"%Y-%m-%d %H:%M:%S")
readme="README.txt"

# Physically create a folder for the test and path if needed.
# Directory-wise, a test folder must not have a direct ancestor-descendant relationship with another test folder.

if [ -d "$test_folder_path" ]; then
  echo "Error: A folder already exists at test/${test_folder_path}. A test folder must be a new leaf folder."
  exit 1
fi

run_test_sh="run_test.sh"
found_run_test=0
IFS='/' read -ra paths <<< "$test_folder_path"
for sub_path in "${paths[@]}"; do
  if [ ! -d "$sub_path" ]; then
    mkdir "${sub_path}"
    if [ $? -ne 0 ]; then
      echo "Error: Failed to create test folder at test/${test_folder_path}."
      exit 1
    fi
  elif [ -e "${sub_path}/${run_test_sh}" ]; then
    echo "Error: A test folder may not be a direct descendant folder of another test folder."
    exit 1
  fi
  cd "${sub_path}"
done
for sub_path in "${paths[@]}"; do cd ".."; done

# Add basic test folder contents.

(cp -rn ../scripts/_test_template/* "$test_folder_path" || true) && \
cd "$test_folder_path" && \
{
  if [ ! -s "$readme" ]; then
    echo "\nThis test folder was created on ${current_datetime}.\n\nTest description:\n" > "${readme}"
  fi
}
if [ $? -ne 0 ]; then
  echo "Error: Failed to populate test folder at test/${test_folder_path}."
  rm -r "$test_folder_path"
  echo "The incomplete test folder at test/${test_folder_path} has been deleted."
  exit 1
fi

echo "Created test folder at test/${test_folder_path}."
