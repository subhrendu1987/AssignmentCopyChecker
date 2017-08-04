# AssignmentCopyChecker
This uses sim_c to check copy in the assignments

## Download and extract the files
git clone https://github.com/subhrendu1987/AssignmentCopyChecker.git
cd AssignmentCopyChecker
## Compile  sim_c
cd sim_2_70/
make test

## Configure framework
cd Assignment
gcc ignoreLine.c -o ignoreLine

## How to use
l. COPY ALL FOLDERS WITH NAME AS ROLL NO IN REPO. FOLDER CONTENT MUST BE EITHER ".c" OR ".h" FILE.

l. RUN FOLLOWING COMMANDS 
	l. `bash check_c.sh > result/c.txt`
	l. `bash check_assembly.sh > result/assembly.txt`
	l. `bash  check_flow.sh > result/flow.txt`
	
l. RUN 
	l. `sort result/c.txt > result/c.sort`
	l. `./ignoreLine result/c.sort > result/c.csv`
	
l. VIEW RESULTS IN THE FILE `result/c.csv`

l. CHECK THIS FILES FOR PERCENTAGE OF MATCH. FOR C FILES MATCH MUST NOT BE GREATER THAN 30%.


