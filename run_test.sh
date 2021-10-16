#!/bin/bash

IFS=$'\n'
rm $2
sed -i 's/\r//g' ./input.txt
./$1 < ./input.txt > $2