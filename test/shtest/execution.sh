#!/bin/bash

function do_test() {
	./shtest "$VAR outfile"
	eval "$VAR sh_outfile"
	diff sh_outfile outfile
}

VAR="echo $SHELL | cat >"
do_test

VAR="echo $PATH | head -n3 | cat >"
do_test

VAR="cat < executor.sh | echo $PATH | tail -n3 | cat >"
do_test

# while read line; do
#     VAR=`echo "${line}"`
# 	echo "===="
# 	echo $VAR
# 	echo "===="
# 	do_test
# done < command.txt
# while read -a line; do
#     echo "-----------"
#     # echo "arr[*]: ${arr[*]}"
#     # echo "arr[0]: ${arr[0]}"
#     # echo "arr[1]: ${arr[1]}"
# 	VAR="${line[*]}"
# 	echo $VAR
# 	do_test
# done < cat command.txt
# for var in `cat command.txt`; do
#     VAR=$var
# 	echo "===="
# 	echo $VAR
# 	echo "===="
# 	do_test
# done
