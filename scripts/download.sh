
#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Error: Must provide type of data as command line argument (iris, stocks, etc... )."
  exit 1
fi

script_path="./_data_scripts/get_${1}_data.sh"

if [ -e "$script_path" ]; then
  bash "$script_path"
else
  echo "Error: Downloader for '$1' dataset not implemented."
  exit 1
fi
