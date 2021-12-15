#!/bin/bash
IFS=$'\n'
valgrind ./$1 < ./memcheck_input.txt
