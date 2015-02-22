#!/bin/bash
numOfAddresses=$1
fileName="input.txt"

# make sure file is clear
rm -f "input.txt" 

for ((x=1; x <= $numOfAddresses; x++)); do
	# add data to text file
	id=$(($RANDOM % 2))
	data1=$(($RANDOM % 8))
	data2=$(($RANDOM % 8))
	data3=$(($RANDOM % 8))
	data4=$(($RANDOM % 8))

	printf '%i%i%i\n0%i%i\n' "$id" "$data1" "$data2" "$data3" "$data4" >> "input.txt"
done

