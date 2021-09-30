# GitLink
https://github.com/subhrendu1987/AssignmentCopyChecker
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
		* this generates two files `results/Match.txt` and `results/Match.csv`<br>
	* `python3.8 Report.py`<br>
		* this generates `PlagTable.csv` which provides the maximum match percentage of a student with another <br>
	* `bash check_assembly.sh > result/assembly.txt` not working now <br>
	* `bash  check_flow.sh > result/flow.txt`not working now <br>
* Use `/home/dick/AssignmentCopyChecker/Assignment/sim_c++ -d <FILE_NAME_A> <FILE_NAME_B>` for pairwise comparison
