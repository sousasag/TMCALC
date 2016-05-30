#!/bin/bash
dire=$(pwd)
# cd PATH_INSTALATION (change here)
cd /home/sousasag/programs/tmcalc/
if [ -f "tmcalc" ]; then


	if [ $# -ne 1 ]; then
		echo "You need to indicate one file. Example:"
		echo "./TMCalc.bash HD20619.ares"
	fi

	file=$1

	if [ -f "$file" ]; then

		./tmcalc ratios_list.dat feh_calib_lines.dat $file

	else
	
		echo "The file: $file"
		echo "does not exist in your system. Check the name and/or the path"
	fi
else
	echo "TMCalc executable file not present:"
	echo "Check path and/or compile the C code:"
	echo "Use the Makefile to compile it"
	echo '$make'
fi


cd $dire
exit

