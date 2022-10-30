#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct Op_tab{
	char opcode[100];
	char hex[100];	
};

typedef struct Code_line{
	char label[100];
	char opcode[100];
	char operand[100];	
};

struct Code_line codes[100];
struct Op_tab opcodes[100];




void opcode_fetch(){
	
	FILE *fptr = fopen("optab.txt","r");
	
	if (fptr == NULL) {
        	printf("no such file.");
        	return ;
    	}
    	int i=0;
    	char temp[10000];
    	int temp_hex=0;
    	
    	char temp1[100],temp2[100];
    	fscanf ( fptr, "%s %s", temp1, temp2);
    	
        while ( fscanf ( fptr, "%s %s", opcodes[i].opcode, opcodes[i].hex)==2) {
        	//printf("%s %s\n",opcodes[i].opcode,opcodes[i].hex);	
        	i++;
        }
}

int fetch_program(){
	
	
	FILE *fptr = fopen("code.txt", "r");
	char temp1[1000];
	int j=0;
	while ( fscanf ( fptr, "%s %s", temp1, codes[j].operand)==2) {	
		for (int i = 0; i<strlen(temp1); i++) {
			while(temp1[i] != ':') {
				strncat(codes[j].label, &temp1[i], 1);
				i++;
			}
			i++;
			while(temp1[i] != '\0') {
				strncat(codes[j].opcode, &temp1[i], 1);
				i++;
			}
		}
		printf("%s\t %s \t%s\n",codes[j].label,codes[j].opcode,codes[j].operand);
		j++;
	}
	return j;
}

int search_symtab(char *label){
	FILE *fptr = fopen("symtab.txt", "r");
	if (fptr == NULL) {
        	printf("no such file.");
        	return 0;
    }
	char *temp1,*temp2;
	while ( fscanf ( fptr, "%s %s", temp1, temp2)==2) {	
		if(strcmp(temp1,label)==0){
			return 1;
		}
	}
	return 0;
}

int search_optab(char *opcode){
	FILE *fptr = fopen("optab.txt", "r");
	char temp1[100],temp2[1000];
	
	if (fptr == NULL) {
        	printf("no such file.");
        	return 0;
    }

	fscanf ( fptr, "%s %s", temp1, temp2);

	while ( fscanf ( fptr, "%s %s", temp1, temp2)==2) {	
		if(strcmp(temp1,opcode)==0){
			return 1;
		}
	}
	return 0;
}

void pass_one(int no_lines){
	fflush(stdin);
	int start_address;
	int i=0;
	int LOCCTR=0x0;
	int program_size=0x0;
	FILE *fptr = fopen("inter.txt", "w");
	FILE *fptr2 = fopen("symtab.txt","w");
	for(i=0;i<no_lines;i++){
		int temp=0x0;
		if(strcmp(codes[i].opcode,"START")==0){
			start_address = (int)strtol(codes[i].operand, NULL, 16);
			LOCCTR = start_address;
			fprintf(fptr, "  \t\t%s \t\t%s \t\t%s\n",codes[i].label, codes[i].opcode,codes[i].operand);
		}else{
			if(strlen(codes[i].label)!=0){
				int flag = search_symtab(codes[i].label);
				if(flag){
					printf("Duplicate Symbol\n");
					exit(0);
				}
				fprintf(fptr2, "%s\t%x\n",codes[i].label,LOCCTR);
			}
			if(strlen(codes[i].opcode)!=0){
				int flag  = search_optab(codes[i].opcode);	
				if(flag){
					temp = LOCCTR + 0x3;
				}else{
					if(strcmp(codes[i].opcode,"WORD")==0)
						temp=LOCCTR + 0x3;
					else if(strcmp(codes[i].opcode,"RESW")==0){
						temp  = LOCCTR + 0x3*(atoi(codes[i].operand));
					}else if(strcmp(codes[i].opcode,"RESB")==0){
						temp = LOCCTR + atoi(codes[i].operand);
					}else if(strcmp(codes[i].opcode,"BYTE")==0){
						temp = LOCCTR + strlen(codes[i].operand);
					}else if (strcmp(codes[i].opcode,"END")==0){
						program_size = LOCCTR - start_address;
						printf("Program Size:%x \nProgram Ended\n",program_size);
						break;
					}else{
						printf("Invalid Opcode\n");
						exit(0);
					}
				}
			}
			fprintf(fptr, "%x \t\t%s \t\t%s \t\t%s\n",LOCCTR,codes[i].label,codes[i].opcode,codes[i].operand);
			LOCCTR = temp;
		}
	}
	fprintf(fptr, "\t\t\t\t\tEND \t\t%x\n",start_address);
	fclose(fptr);
	fclose(fptr2);
}


int main(){
	
	opcode_fetch();
	int no_lines = fetch_program();
	pass_one(no_lines);
	
}
