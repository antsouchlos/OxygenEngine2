#!/bin/bash

if [ $# -gt 1 ]
then
	echo "$0: Invalid number of arguments provided"
	echo ""
	echo "Usage: $0 [flags]"
	echo "Example: $0 --win64"
	echo ""
	echo "[] means optional"

	exit
fi

FLAG=""

if [ $# -eq 1 ]
then
	if [ $1 = "--win64" ]
	then
		FLAG="--win64"
	elif [ $1 = "--linux64" ]
	then
		FLAG="--linux64"
	else
		echo "$1 is not an accepted flag"
		echo "Possible flags: '--win64', '--linux64'"
		exit
	fi
fi

make clean
./configure $FLAG
make
