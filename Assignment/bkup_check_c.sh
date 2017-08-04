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
rm -f $CODEBASE/*.*
# Generate codebase files########################################################

cd $REPO$1
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


# Check with sim_c###############################################################

#./sim_c -sep $CODEBASE*$EXTN
files=`ls $CODEBASE$1`
echo "------------------------------------------------------------------------------------------------"
for file1 in $files; do
	if [ -f $CODEBASE$file1 ];then
		#echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
		#echo $CODEBASE$file1  
		#echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
		for file2 in $files; do
			if [ -f $CODEBASE$file2 ];then
			  if [ $file1 != $file2 ]; then
			     #echo $CODEBASE$file2
			    ./sim_c -sep $CODEBASE$file1 $CODEBASE$file2
			    echo "------------------------------------------------------------------------------------------------"
			  fi
			fi
		done 
	#echo "--------------------------------------------------------"
	fi
done
