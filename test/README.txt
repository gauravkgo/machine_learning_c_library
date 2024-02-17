
This folder contains all test folders and testsuites, as well as their results.

A test folder, along with any upper directory testsuites, is created with "make_test.sh" in the scripts folder.
The resulting test folder and or testsuites will be placed in this test/ directory.
Running "make_test.sh" will create and populate the test folder with a "run_test.sh" script,
which is used to run one or more executables associated with the test.

A test folder should not contain nor be a direct ancestor folder of another test folder.
This is disallowed by "make_test.sh".

Any test or collection of tests can be run with "test.sh" in the scripts folder.
A 'results' folder will be generated in the targeted test/testsuite folder,
which holds files containing results from the test or tests.

A test is successful if the output and error output from running the local "run_test.sh" script
matches "correct_output.txt" and "expected_errors.txt" respectively.
It is preferred that these files be edited manually.
If there is confidence that "run_test.sh" produces the intended correct output and error,
then "correct_output.txt" and "expected_errors.txt" can be populated with this output with "puts_test.sh".

Prior to running tests, "test.sh" will compile all targeted tests by
compiling all targeted test c files, linking the library,
and producing executables with names ending in "_executable".
