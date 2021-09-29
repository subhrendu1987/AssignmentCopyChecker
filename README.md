# AssignmentCopyChecker
This uses sim_c to check copy in the assignments
sim_c requires the follwoing commands for execution</br >
`gcc, flex, cp, ln, echo, rm, wc`
## Download and extract the files
git clone https://github.com/subhrendu1987/AssignmentCopyChecker.git<br>
`cd AssignmentCopyChecker`<br>
`ln ../SimilarityChecker/sim_c++` <br>
`ln ../SimilarityChecker/sim_c` <br>
## Compile  framework
make

## How to use
make fresh
cd Assignment
make
mkdir results; mkdir codebase;

* COPY ALL FOLDERS WITH NAME AS ROLL NO IN REPO. FOLDER CONTENT MUST BE EITHER ".c/.cpp/.h/.cpp" FILE.

* RUN FOLLOWING COMMANDS <br>
	* `bash check_cpp.sh`<br>
		* this generates two files `results/Match.txt` and `results/Match.filter`<br>
	* `bash check_assembly.sh > result/assembly.txt` not working now <br>
	* `bash  check_flow.sh > result/flow.txt`not working now <br>
	
* RUN <br>
	* `python Report.py -F "result/cpp.txt" -o "result/cpp.csv" > result/cpp.error`<br>
	
* VIEW RESULTS IN THE FILE `result/c.csv`

* CHECK THIS FILES FOR PERCENTAGE OF MATCH. FOR C FILES MATCH MUST NOT BE GREATER THAN 30%.

* `sort result/c.txt > result/c.sort`<br>
* `./ignoreLine result/c.sort > result/c.csv`<br>
	
