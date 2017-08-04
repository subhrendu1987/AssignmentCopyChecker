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
1. COPY ALL FOLDERS WITH NAME AS ROLL NO IN REPO. FOLDER CONTENT MUST BE EITHER ".c" OR ".h" FILE.

2. RUN FOLLOWING COMMANDS 
	2.a `bash check_c.sh > result/c.txt`
	2.b `bash check_assembly.sh > result/assembly.txt`
	2.c `bash  check_flow.sh > result/flow.txt`
	
3. RUN 
	3.a `sort result/c.txt > result/c.sort`
	3.4 `./ignoreLine result/c.sort > result/c.csv`
	
4. VIEW RESULTS IN THE FILE `result/c.csv`

5. CHECK THIS FILES FOR PERCENTAGE OF MATCH. FOR C FILES MATCH MUST NOT BE GREATER THAN 30%.


