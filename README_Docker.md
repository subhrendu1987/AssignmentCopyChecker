# List of Useful Docker commands 
## Initialize Docker Images
### Build docker Image
```
sudo docker -t assignment-copy-checker .
``` 
or 
```
sudo docker compose build
```
### Create Folder structure
```
mkdir results; mkdir codebase; mkdir repo
```
### Save image
`sudo docker save assignment-copy-checker > assignment-copy-checker.tar`
### Load image
`sudo docker load `
## Execute Docker
### Start docker
```
sudo docker compose up -d
sudo docker exec -it copychecker /bin/bash
```
* COPY ALL FOLDERS WITH NAME AS ROLL NO IN REPO. FOLDER CONTENT MUST BE EITHER ".c/.cpp/.h/.cpp" FILE.
* Add the assignments in the `AssignmentCopyChecker/repo`
* RUN FOLLOWING COMMANDS <br>
	* `bash check_cpp.sh`<br>
		* this generates two files `results/Match.txt` and `results/Match.csv`<br>
	* `python3.8 Report.py`<br>
		* this generates `PlagTable.csv` which provides the maximum match percentage of a student with another <br>
	* `bash check_assembly.sh > result/assembly.txt` not working now <br>
	* `bash  check_flow.sh > result/flow.txt`not working now <br>
* Use `/home/dick/AssignmentCopyChecker/Assignment/sim_c++ -d <FILE_NAME_A> <FILE_NAME_B>` for pairwise comparison
## Finish execution
```
sudo docker compose down
```