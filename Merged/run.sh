#!/bin/bash
debug=$1
fileName="input.txt"
executable="pdp8"

# check to display additional information
if [ $debug -eq 1 ]; then 
	./$executable $fileName 1
else 
	./$executable $fileName
fi

exit





