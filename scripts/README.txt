
This directory contains bash scripts for ease of project development.
This includes building the library, downloading datasets, and running tests.

"libml_build.sh" builds the machine learning archive and shared object libraries in bin/lib.

"clean.sh" deletes all library binary and object files and test executables.

"download.sh" takes a dataset name and downloads this dataset in the data/datasets folder.

"make_test.sh" takes a directory path local to test/ and produces a test folder at this directory with the given name.

"test.sh" takes a directory path local to test/ and
compiles and runs all tests under this directory, printing test results to a results folder.
If no directory is provided as argument, then "test.sh" compiles and runs all tests.

"puts_test.sh" takes a directory path local to test/ and
updates the "correct_output.txt" and "expected_errors.txt" files of tests under this directory
with the standard output and error of the test as it is.
If no directory is provided as argument, then "puts_test.sh" updates all tests.
