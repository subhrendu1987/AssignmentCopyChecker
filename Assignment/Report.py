import os, sys, argparse, random, re
#############################################################################
def parse_args():
    parser = argparse.ArgumentParser(description="Assignment Checking Reports")
    
    parser.add_argument('--inputfile', '-F',
                        action="store",
                        help="Input report file generated from Similarity Checker",
                        default="results/Match.csv")
    parser.add_argument('--outputfile', '-O',
                        action="store",
                        help="Input report file generated from Similarity Checker",
                        default="results/PlagTable.csv")
    
    args = parser.parse_args()
    return args
#############################################################################
args=parse_args()
file_handler=open(args.inputfile,"r")
lines=[l.strip() for l in file_handler if len(l)>0]
file_handler.close()
'''for line in lines:
	if("consists for" in line and "material" in line):
		report.append(line)
'''
report={}
for line in lines:
	items=line.split(",")
	items[0]=os.path.basename(items[0])
	items[1]=int(items[1].replace(" %",""))
	items[2]=os.path.basename(items[2])
	if(items[0] in report.keys()):
		if(report[items[0]]['weight'] < items[1]):
			report[items[0]]={'key':items[2],'weight':items[1]}
	else:
		report[items[0]]={'key':items[2],'weight':items[1]}
#############################################################################
file_handler=open(args.outputfile,"w")
for k in report.keys():
	entry=','.join([k,report[k]['key'],str(report[k]['weight'])])
	file_handler.write(entry+"\n")
file_handler.close()
#############################################################################
print("File Created: ", args.outputfile)
