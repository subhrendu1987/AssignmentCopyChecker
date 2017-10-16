#!/bin/sh
# Run parameters
PWD=`pwd`
REPO='repo/'
EXTN='.txt'
CODEBASE='codebase/'

#################################################################################

REPO=$PWD/$REPO
CODEBASE=$PWD/$CODEBASE
CONDENSED_FILE=''
DIRPATH=''
rm -f $CODEBASE/*
# Generate codebase files########################################################
cd $REPO$1
pwd
files=`ls $1`
rename 's/ /_/g' *

files=`ls $1`
for file in $files; do
        if [ -d $file ];then
          #echo "test"
          cat $file/*.* > $file$EXTN
          #echo $file
        fi
done 

cd ..
mv $REPO*$EXTN $CODEBASE
'
# Check with sim_c###############################################################

#./sim_c -sep $CODEBASE*$EXTN

files=`ls $CODEBASE$1`

declare -a list
list=(${files// / })

limit=${#list[@]}

>&2 echo $limit

for (( i=0; i<${limit}; i++ ))
do
	file1=${list[i]}
	for (( j=(i+1); j<${limit}; j++ ))
	do
		file2=${list[j]}
		./../sim_2_70/sim_c -sep $CODEBASE$file1 $CODEBASE$file2
	done 
done
'
