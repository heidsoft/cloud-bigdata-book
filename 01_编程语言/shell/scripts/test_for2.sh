#!/bin/bash

for i in {1,2,3,4,5,6,7,8,9} 
do
	for j in {1,2,3,4,5,6,7,8,9}
	do
		((num=$i*$j))
		echo -n -e $i\*$j=$num"\t"
	done
	echo  
done
