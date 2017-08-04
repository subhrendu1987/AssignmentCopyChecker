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
* COPY ALL FOLDERS WITH NAME AS ROLL NO IN REPO. FOLDER CONTENT MUST BE EITHER ".c" OR ".h" FILE.

* RUN FOLLOWING COMMANDS <br>
	* `bash check_c.sh > result/c.txt`<br>
	* `bash check_assembly.sh > result/assembly.txt`<br>
	* `bash  check_flow.sh > result/flow.txt`<br>
	
* RUN <br>
	* `sort result/c.txt > result/c.sort`<br>
	* `./ignoreLine result/c.sort > result/c.csv`<br>
	
* VIEW RESULTS IN THE FILE `result/c.csv`

* CHECK THIS FILES FOR PERCENTAGE OF MATCH. FOR C FILES MATCH MUST NOT BE GREATER THAN 30%.


