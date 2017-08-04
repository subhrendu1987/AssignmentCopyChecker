#include<stdio.h>
#define DISCARD 33

int main(int argc, char** argv){
	FILE* fp;
	char buffer[200];
	int i,blanks=0;
	if(argc<2){
		printf("USAGE: ignoreLine [FILE NAME]\n");
		return(0);
	}
	fp=fopen(argv[1],"r");
	// Discard blanks
	while(fscanf(fp,"%s",buffer)!=EOF){
		if((strcmp(buffer,"\n")==0)||(strcmp(buffer,"File")==0)){
			break;
		}
		blanks++;
	}
	for(i=0;i<5;i++){
		fscanf(fp,"%s",buffer);
	}
	// Discard header lines	
	while(fscanf(fp,"%s",buffer)!=EOF){
		if((strcmp(buffer,"\n")==0)||(strcmp(buffer,"File")==0)){
			for(i=0;i<5;i++){
				fscanf(fp,"%s",buffer);
			}
		}else{
			break;
		}
	}
	printf("%s",&buffer[DISCARD]);
	for(i=0;i<7;i++){
		fscanf(fp,"%s",buffer);
		if(i==2){
			printf(" %s",buffer);
		}else if(i==5){
			printf(" %s",&buffer[DISCARD]);
		}
	}
	printf("\n");
	// Print result lines
	while(fscanf(fp,"%s",buffer)!=EOF){
		if((strcmp(buffer,"Total:")!=0)){
		printf("%s",&buffer[DISCARD]);
			for(i=0;i<7;i++){
				fscanf(fp,"%s",buffer);
				if(i==2){
					printf(" %s",buffer);
				}else if(i==5){
					printf(" %s",&buffer[DISCARD]);
				}
			}
			printf("\n");
		}else{
			break;
		}
	}
	
	
	
	fclose(fp);
	return(0);
}
