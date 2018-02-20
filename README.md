# AssignmentCopyChecker
This uses sim_c to check copy in the assignments

## Download and extract the files
git clone https://github.com/subhrendu1987/AssignmentCopyChecker.git<br>
cd AssignmentCopyChecker<br>
## Compile  framework
make

## How to use
make fresh

* COPY ALL FOLDERS WITH NAME AS ROLL NO IN REPO. FOLDER CONTENT MUST BE EITHER ".c" OR ".h" FILE.

* RUN FOLLOWING COMMANDS <br>
	* `bash check_c.sh > result/c.txt`<br>
	* `bash check_assembly.sh > result/assembly.txt` not working now <br>
	* `bash  check_flow.sh > result/flow.txt`not working now <br>
	
* RUN <br>
	* `sort result/c.txt > result/c.sort`<br>
	* `./ignoreLine result/c.sort > result/c.csv`<br>
	
* VIEW RESULTS IN THE FILE `result/c.csv`

* CHECK THIS FILES FOR PERCENTAGE OF MATCH. FOR C FILES MATCH MUST NOT BE GREATER THAN 30%.


