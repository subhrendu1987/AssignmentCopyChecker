#!/bin/sh
# Run parameters
PWD=`pwd`
REPO='repo/'
EXTN='.txt'
CODEBASE='codebase/'

#################################################################################
cf () { 
    for x in $@;
    do
        echo "-----"$x"-----";
        cat "$x";
    done;
    unset x
}
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

for dir in $files; do
        if [ -d $dir ];then
          >&2 echo "$dir"
          find "$dir" \( -name '*.c' -o -name "*.h" \)| tail -vn +1  > $dir$EXTN
          find "$dir" \( -name '*.c' -o -name "*.h" \) -exec cat {} \; >> $dir$EXTN
          echo "END " >> $dir$EXTN
          mv $dir$EXTN $CODEBASE
        fi
        
done 
cd ..
###############################################################
### Create un commented version
files=`ls $CODEBASE$1`

declare -a list
list=(${files// / })

limit=${#list[@]}

for (( i=0; i<${limit}; i++ ))
do
	./RemoveComments $CODEBASE${list[i]}
done

#mv $REPO*$EXTN 

# Check with sim_c###############################################################

#./sim_c -sep $CODEBASE*$EXTN

files=`ls $CODEBASE$1`

declare -a list
list=(${files// / })

limit=${#list[@]}

>&2 echo "-------------------"
>&2 echo "Count of files="$limit

for (( i=0; i<${limit}; i++ ))
do
	file1=${list[i]}
	for (( j=(i+1); j<${limit}; j++ ))
	do
		file2=${list[j]}
		./sim_c -sep $CODEBASE$file1 $CODEBASE$file2
		echo "##################################################################"
	done 
done

