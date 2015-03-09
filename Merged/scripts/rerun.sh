#!/bin/bash
debug=$2
fileName=$1
executable="pdp8"

# clean and recompile
make clean
make

# check to display additional information
if [ $debug -eq 1 ]; then 
	./$executable $fileName 1
else
	./$executable $fileName 0
fi

exit





