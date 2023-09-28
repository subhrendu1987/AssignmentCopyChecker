# GitLink
```
https://github.com/subhrendu1987/AssignmentCopyChecker
git clone https://github.com/subhrendu1987/AssignmentCopyChecker.git
```
# AssignmentCopyChecker
This uses sim_c to check copy in the assignments
sim_c requires the follwoing commands for execution</br >
`gcc, flex, cp, ln, echo, rm, wc`
## Download and extract the files
```
cd AssignmentCopyChecker
ln ../SimilarityChecker/sim_c++
ln ../SimilarityChecker/sim_c
```
## Compile  framework
```
make
```
## How to use
```
make fresh
cd Assignment
make
mkdir results; mkdir codebase;
```
* COPY ALL FOLDERS WITH NAME AS ROLL NO IN REPO. FOLDER CONTENT MUST BE EITHER ".c/.cpp/.h/.cpp" FILE.
* Add the assignments in the `/home/dick/AssignmentCopyChecker/Assignment/repo` and keep a copy of the same in `/home/plagcheck/CSE.206L-2020/<Course_SectionNameSpecificDIrectory`

* RUN FOLLOWING COMMANDS <br>
	* `bash check_cpp.sh`<br>
		* this generates two files `results/Match.txt` and `results/Match.csv`<br>
	* `python3.8 Report.py`<br>
		* this generates `PlagTable.csv` which provides the maximum match percentage of a student with another <br>
	* `bash check_assembly.sh > result/assembly.txt` not working now <br>
	* `bash  check_flow.sh > result/flow.txt`not working now <br>
* Use `/home/dick/AssignmentCopyChecker/Assignment/sim_c++ -d <FILE_NAME_A> <FILE_NAME_B>` for pairwise comparison
