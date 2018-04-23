import os, sys, argparse, random, re
#############################################################################
def parse_args():
    parser = argparse.ArgumentParser(description="Assignment Checking Reports")
    
    parser.add_argument('--file', '-F',
                        action="store",
                        help="Input file from Similarity Checker",
                        default=None,
                        required=True)
    
    args = parser.parse_args()
    return args
#############################################################################
args=parse_args()
file_handler=open(args.file,"r")
lines=[l.strip() for l in file_handler if len(l)>0]
file_handler.close()
report=[]
for line in lines:
	if("consists for" in line and "material" in line):
		report.append(line)
for line in report:
	items=line.split(" ")
	items[0]=os.path.basename(items[0])
	items[6]=os.path.basename(items[6])
	str1=" ".join(items[0:3])
	str2="".join(items[3:5])
	str3=" ".join(items[5:7])
	print("%s\t%s\t%s"%(str1,str2,str3))
#############################################################################
'''
comment_idx=[idx for idx,l in enumerate(variable) if(l.startswith("#"))]
group_results=[]
core_results=[]
for i,j in zip([0]+comment_idx,comment_idx):
	res=variable[(i+1):j]
	if(len(res)!= 5):
		print("--------------------------------------------------------")
		print("Some Problem (See lines [%d-%d])"%(i+1,j))
		file1=os.path.basename(res[0].split(" ")[1])
		file2=os.path.basename(res[1].split(" ")[1])
		#print("./sim_[c, c++,t] -sep %s %s"%(res[0].split(" ")[1].replace(":",""),res[1].split(" ")[1].replace(":","")))
		print("rerun")
		print(res)
		print("--------------------------------------------------------")
	else:
		core_results.append(res[4])
	group_results.append(res)
#############################################################################
if(args.out!=None):
	f=open(args.out, 'w')
for res in core_results:
		if(args.out!=None):
			print >> f, res
		else:
			print res
if(args.out!=None):
	f.close()
'''

