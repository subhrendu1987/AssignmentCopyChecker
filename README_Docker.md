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
```
sudo docker save assignment-copy-checker > docker/assignment-copy-checker.tar
cd docker; split -b 99M assignment-copy-checker.tar assignment-copy-checker.tar.
```
### Load image
```
cd docker; cat assignment-copy-checker.tar.a? > assignment-copy-checker.tar
sudo docker load assignment-copy-checker.tar
```
## Execute Docker
* Modify `Folder2ID.csv` to provide name to Roll number mapping. Oterwise, manually save all submission folder with the student Roll number.
### Start docker
```
sudo docker compose up -d
sudo docker exec -it copychecker /bin/bash
```
* FOLDER CONTENT MUST BE EITHER ".c/.cpp/.h/.cpp" FILE.
* Add the assignments in the `AssignmentCopyChecker/repo`
* RUN FOLLOWING COMMANDS <br>
	* `cd Assignment; bash check_cpp.sh`<br>
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
