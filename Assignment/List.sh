#!/bin/sh

PWD=`pwd`
REPO='repo/'
#echo " " > $PWD/List.txt
declare -a list
REPO=$PWD/$REPO
cd $REPO$1
files=`ls $1`
	
	list=(${files// / })
	limit=${#list[@]}
	for (( i=0; i<${limit}; i++ ))
	do
		j=$((i+1))
		echo ${list[i]}
	done
