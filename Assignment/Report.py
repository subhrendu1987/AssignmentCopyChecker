import os, sys, argparse, random, re
#############################################################################
def parse_args():
    parser = argparse.ArgumentParser(description="Assignment Checking Reports")
    parser.add_argument('--file', '-F',
                        action="store",
                        help="Input file",
                        default=None)
                        #required=True)
    parser.add_argument('--out', '-o',
                        action="store",
                        help="Output file",
                        default=None)

    args = parser.parse_args()
    return args
#############################################################################
args=parse_args()
file_handler=open(args.file,"r")
variable=[l.strip() for l in file_handler if len(l)>0]
file_handler.close()
comment_idx=[idx for idx,l in enumerate(variable) if(l.startswith("#"))]
group_results=[]
core_results=[]
for i,j in zip([0]+comment_idx,comment_idx):
	res=variable[(i+1):j]
	if(len(res)!= 5):
		print("Some Problem (See lines [%d-%d])"%(i+1,j))
		file1=os.path.basename(res[0].split(" ")[1])
		file2=os.path.basename(res[1].split(" ")[1])
		print("%s -- %s "%(file1,file2))
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


