#include<stdio.h>

#define SIZE 82

int main(){
int A[SIZE],B[SIZE];
int maxA,maxB;
char buffer[10];
FILE *fp;
int i,j,flag=0;
fp=fopen("CS513.csv","r");
	if(fp==NULL){
		printf("CS513.csv Not found\n");
		return(0);
	}
i=0;
	while(fscanf(fp,"%s",buffer)!=EOF){
		A[i]=atoi(buffer);
		i++;
	}
maxA=i;	
fclose(fp);	
fp=fopen("List.txt","r");
	if(fp==NULL){
		printf("List.txt Not found\n");
		return(0);
	}

i=0;
	while(fscanf(fp,"%s",buffer)!=EOF){
		B[i]=atoi(buffer);
		i++;
	}
maxB=i;
fclose(fp);
/**************************************************************************/
printf("Not Submitted\n");
/**************************************************************************/
	for(i=0;i<maxA;i++){
	flag=0;
		for(j=0;j<maxB;j++){
			if(A[i]==B[j]){
				///Do Nothing
				flag=1;
				//printf("Match Found for %d\n",A[i]);
				break;
			}
		}
		if(!flag){
			printf("%d\n",A[i]);
		}
	}	

return(0);
}
