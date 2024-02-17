
#!/bin/bash

dir_name=$(basename "$PWD")
test_exec="./${dir_name}_executable"

out_minus_addr=$(mktemp)
correct_out_minus_addr=$(mktemp)

$test_exec > table_output.txt 2>&1

sed '/^Table at /d' table_output.txt > "$out_minus_addr"
sed '/^Table at /d' table_output_correct.txt > "$correct_out_minus_addr"

diff "$out_minus_addr" "$correct_out_minus_addr"

rm "$out_minus_addr" "$correct_out_minus_addr"
