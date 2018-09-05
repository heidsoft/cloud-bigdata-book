#!/bin/bash
while read line
do
	echo $line
	echo "~~~~~~~~~~~~"
done < /etc/passwd
