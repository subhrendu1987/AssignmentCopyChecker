1. COPY ALL FOLDERS WITH NAME AS ROLL NO IN REPO. FOLDER CONTENT MUST BE EITHER .c OR .h FILE.

2. RUN SCRIPTS `check_c.sh`,`check_assembly.sh`,`sh check_flow.sh`
	AND REDIRECT OUTPUT TO "RESULT/C.TXT","RESULT/ASSEMBLY.TXT","RESULT/FLOW.TXT" RESPECTIVELY.
	
3.a. RUN `sort result/c.txt > result/c.sort` AND `./ignoreLine result/c.sort > c.csv`
OR
3.b. RUN `python Report.py -F result/c.txt > result/c.csv`

4. CHECK `result/c.csv` THIS FILES FOR PERCENTAGE OF MATCH. FOR C FILES MATCH MUST NOT BE GREATER THAN 30%.
