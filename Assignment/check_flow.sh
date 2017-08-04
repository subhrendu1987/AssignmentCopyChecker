# Run parameters
PWD=`pwd`
REPO='repo/'
EXTN='.cfg'
CODEBASE='cfg/'

#################################################################################

REPO=$PWD/$REPO
CODEBASE=$PWD/$CODEBASE
CONDENSED_FILE=''
DIRPATH=''
rm -f $CODEBASE/*.*
# Generate codebase files########################################################

cd $REPO$1
files=`ls $1`

for file in $files; do
        if [ -d $file ];then
          #echo "test"
          gcc -fdump-tree-cfg $file/*.c 
          #echo $file
        fi
done 

cd ..
mv $REPO*$EXTN $CODEBASE
rm repo/a.out


# Check with sim_text###############################################################

files=`ls $CODEBASE$1`

for file1 in $files; do
	if [ -f $CODEBASE$file1 ];then
		echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
		echo $CODEBASE$file1  
		echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
		for file2 in $files; do
			if [ -f $CODEBASE$file2 ];then
			  if [ $file1 != $file2 ]; then
			     #echo $CODEBASE$file2
			    ./../sim_2_70/sim_text -sep $CODEBASE$file1 $CODEBASE$file2
			    echo "------------------------------------------------------------------------------------------------"
			  fi
			fi
		done 
	#echo "--------------------------------------------------------"
	fi
done
