#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct Optab{
	char opcode[100];
	int hex;	
};

typedef struct Code_line{
	char label[100];
	char opcode[100];
	char operand[100];	
};

struct Code_line codes[100];
struct Optab opcodes[100];




int opcode_fetch(){
	
	FILE *fptr = fopen("optab.txt","r");
	
	if (fptr == NULL) {
        	printf("no such file.");
        	return 0;
    	}
    	int i=0;
    	char temp[10000];
    	int temphex=0;
    	
    	char temp1[100],temp2[100];
    	fscanf ( fptr, "%s %s", temp1, temp2);
    	
        while ( fscanf ( fptr, "%s %s", opcodes[i].opcode, temp1)==2) {
		opcodes[i].hex = (int)strtol(temp1, NULL, 16);
        	printf("%s %X\n",opcodes[i].opcode,opcodes[i].hex);	
        	i++;
        }
        return i;
}

int fetch_program(){
	
	
	FILE *fptr = fopen("code.txt", "r");
	char temp1[1000];
	int j=0;
	
        while ( fscanf ( fptr, "%s %s", temp1, codes[j].operand)==2) {	
        	char * token = strtok(temp1, ":");
        	char temp [2][1000];
        	int k=0;
   		while( token != NULL ) {
      			strcpy(temp[k],token);
      			token = strtok(NULL, " ");
      			k++;
   		}
   		strcpy(codes[j].label,temp[0]);
   		strcpy(codes[j].opcode,temp[1]);
   		printf("%s %s %s\n",codes[j].label,codes[j].opcode,codes[j].operand);
        	j++;
        }

	return j;
}

void passone(int no_opcode,int no_lines){
	fflush(stdin);
	int i=0;
	int LOCCTR=0;
	FILE *fptr = fopen("inter.txt", "w");
	for(i=0;i<no_lines;i++){
		if(codes[i].opcode=="START"){
			printf("hello");
			LOCCTR = (int)strtol(codes[i].operand, NULL, 16);
			fprintf(fptr, "\t\t %s \t%s \t%s",codes[i].label, codes[i].opcode,codes[i].operand);
		}
	
	}

	fclose(fptr);
}


int main(){
	
	int no_opcode = opcode_fetch();
	int no_lines = fetch_program();
	//printf("%d %d",no_opcode,no_lines);
	passone(no_opcode,no_lines);
	
}
