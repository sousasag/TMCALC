#!/bin/bash
dire=$(pwd)
# cd PATH_INSTALATION (change here)
cd /home/sousasag/Programas/GIT_projects/TMCALC
if [ -f "tmcalc" ]; then


	if [ $# -lt 1 -o $# -gt 2 ]; then
		echo "You need to indicate one file. Example for default calibration:"
		echo "./TMCalc.bash HD20619.ares"
		echo " "
		echo "Example to use calibration presented in Teixeira et al.2016:"
		echo "./TMCalc.bash HD20619.ares t"
		exit
	fi

	file=$1

	option=$2

	if [ $file='.' ]; then
		file=$dire/$1
	fi

	if [ -f "$file" ]; then
		echo $file
		if [ $# -gt 1 ]; then
			./tmcalc gteixeira_teff_cal.dat gteixeira_feh_cal.dat $file
		else
			./tmcalc ratios_list.dat feh_calib_lines.dat $file
		fi
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

