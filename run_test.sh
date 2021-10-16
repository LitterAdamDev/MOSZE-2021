#!/bin/bash
IFS=$'\n'
FILE=$2
if [ -f "$FILE" ]; then
    rm $2
fi
sed -i 's/\r//g' ./input.txt
./$1 < ./input.txt > $2
sed -i 's/\r//g' ./expected_output.txt
sed -i 's/\r//g' ./generated_output.txt