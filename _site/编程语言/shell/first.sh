#!/bin/sh
#first.sh
#This file looks through all files in the current
#directory for the string POSIX, and then displays those 
#file to the standar output

for file in *
do 
	if grep -q POSIX $file
	then
		more $file
	fi
done

exit 0
