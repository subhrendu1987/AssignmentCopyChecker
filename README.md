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


 ## Use docker
 ### Docker Installation
```
https://docs.docker.com/engine/install/ubuntu/
```
### Build and Execute 
 ```
sudo docker-compose up
sudo docker exec -it copychecker /bin/bash # In a separate terminal
cd Assignment/

# Simple Usage
# ./sim_c -sep [file1] [file2] >>  [OutputFile]

bash check_c.sh
```
### For help on `sim_c`
Use `./sim_c -h`; (or) see the following output
```
Possible options are:
	-rN	set minimum run size to N

		output runs as text (default)
	-d 	output in a diff-like format
	-n 	suppress the text of the runs
	-T 	suppress reporting the input files
	-p 	output similarity in percentages
	-P 	main contributing file to percentages only
	-tN	threshold level of percentages
	-e 	compare each file to each file separately

		compare a file to files after it only (default)
	-a 	compare to all files
	-S 	compare to old files only
	-s 	do not compare a file to itself

		sorted output, most significant first (default)
	-u 	unbuffered, unsorted output

		miscellaneous options:
	-f 	function-like forms only
	-F 	keep function identifiers in tact
	-R 	recurse into subdirectories
	-i 	read arguments (file names) from standard input
	-oF	write output to file F
	-wN	set page width to N
	-O 	show command line options at start-up
	-M 	show memory usage info at close-down
	-v 	show version number and compilation date
	-- 	lexical scan output only
```