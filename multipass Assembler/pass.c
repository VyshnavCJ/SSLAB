#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct {
	char opcode[100];
	char hex[100];	
}Op_tab;

typedef struct {
	char label[100];
	char opcode[100];
	char operand[100];	
}Code_line;

Code_line codes[100];
Op_tab opcodes[100];

typedef struct
{
	char address[100];
	char label[100];
	char opcode[100];
	char operand[100]; 
}Inter_tab;

Inter_tab inter[100];

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
	char temp1[100],temp2[100];
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

int pass_one(int no_lines){
	fflush(stdin);
	int start_address;
	int i=0;
	int LOCCTR=0x0;
	int program_size=0x0;
	FILE *fptr1 = fopen("inter.txt", "w");
	FILE *fptr2 = fopen("symtab.txt","w");
	for(i=0;i<no_lines;i++){
		int temp=0x0;
		if(strcmp(codes[i].opcode,"START")==0){
			start_address = (int)strtol(codes[i].operand, NULL, 16);
			LOCCTR = start_address;
			fprintf(fptr1, "\t\t%s\t%s\t%s\t\n",codes[i].label, codes[i].opcode,codes[i].operand);
			strcpy(inter[i].label,codes[i].label);
			strcpy(inter[i].opcode,codes[i].opcode);
			strcpy(inter[i].operand,codes[i].operand);

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
					fprintf(fptr1, "%x\t\t%s\t%s\t\t%s\n",LOCCTR,codes[i].label,codes[i].opcode,codes[i].operand);
				}else{
					if(strcmp(codes[i].opcode,"WORD")==0)
						temp=LOCCTR + 0x3;
					else if(strcmp(codes[i].opcode,"RESW")==0){
						temp  = LOCCTR + 0x3*(atoi(codes[i].operand));
					}else if(strcmp(codes[i].opcode,"RESB")==0){
						temp = LOCCTR + atoi(codes[i].operand);
					}else if(strcmp(codes[i].opcode,"BYTE")==0){
						temp = LOCCTR + (0x0)*strlen(codes[i].operand);
					}else if (strcmp(codes[i].opcode,"END")==0){
						program_size = LOCCTR - start_address;
						printf("Program Size:%x \nProgram Ended\n",program_size);
						break;
					}else{
						printf("Invalid Opcode\n");
						exit(0);
					}
					fprintf(fptr1, "%x\t%s\t%s\t\t%s\n",LOCCTR,codes[i].label,codes[i].opcode,codes[i].operand);
				}
			}
			

			sprintf(inter[i].address, "%x", LOCCTR);
			strcpy(inter[i].label,codes[i].label);
			strcpy(inter[i].opcode,codes[i].opcode);
			strcpy(inter[i].operand,codes[i].operand);
			LOCCTR = temp;
		}
	}
	char str[1000];
	sprintf(str, "%x", start_address);
	strcpy(inter[no_lines-1].operand,str);
	fprintf(fptr1, "\t\t\t\tEND \t%s\n",str);
	strcpy(inter[no_lines-1].opcode,"END");
	fclose(fptr1);
	fclose(fptr2);
	return program_size;
}


void pass_two(int no_lines,int program_size){
	FILE *fptr1 = fopen("list.txt", "w");
	FILE *fptr2 = fopen("object.txt","w");

	int i;
	int limiter=3;
	for(i=0;i<no_lines;i++){
			int temp1=(0x1)*program_size,temp2;

			if(limiter==0 && strcmp(inter[i].opcode,"END")!=0){
				int start = (int)strtol(inter[i].address, NULL, 16);
				fprintf(fptr2,"\nT%06x09",start);
				limiter=3;
			}
			
			if(strcmp(inter[i].opcode,"START")==0){
				fprintf(fptr1,"%s\t\t%s\t%s\t%s\n",inter[i].address,inter[i].label,inter[i].opcode,inter[i].operand);
				int n = 5-strlen(inter[i].label);
				int start = (int)strtol(inter[i].operand, NULL, 16);
				fprintf(fptr2,"H%s%*s%06x%06x\n",inter[i].label,n,"",start,temp1);
				fprintf(fptr2,"T%06x09",start);
			}else{
				char s1[100]="",s2[100]="";
				if(strlen(inter[i].opcode)!=0){
					int flag  = search_optab(inter[i].opcode);	
					if(flag){
						FILE *fptr = fopen("optab.txt", "r");
						char temp1[100],temp2[1000];
	
						if (fptr == NULL) {
        					printf("no such file.");
        					return ;
    					}

						fscanf ( fptr, "%s %s", temp1, temp2);

						while ( fscanf ( fptr, "%s %s", temp1, temp2)==2) {	
							if(strcmp(temp1,inter[i].opcode)==0){
								fprintf(fptr2,"%s",temp2);
								strcpy(s1,temp2);
							}
						}
						flag = search_symtab(inter[i].operand);
						if(flag){
							FILE *fptr = fopen("symtab.txt", "r");
							if (fptr == NULL) {
									printf("no such file.");
									return ;
							}
							char temp1[100],temp2[100];
							while ( fscanf ( fptr, "%s %s", temp1, temp2)==2) {	
								if(strcmp(temp1,inter[i].operand)==0){
									fprintf(fptr2,"%s",temp2);
									strcat(s1,temp2);
								}
							}
						}
						fprintf(fptr1,"%s\t\t%s\t%s\t\t%s\t%s\n",inter[i].address,inter[i].label,inter[i].opcode,inter[i].operand,s1);
					}else{
						int address = (int)strtol(inter[i].operand, NULL, 16);
						if(strcmp(inter[i].opcode,"WORD")==0){
							fprintf(fptr2,"%06x",address);
							char str[1000];
							sprintf(str, "%06x",address);
							strcpy(s2,str);
						}else if(strcmp(inter[i].opcode,"BYTE")==0){
							address = (int)strtol(inter[i].operand, NULL, 16);
							fprintf(fptr2,"%06x",address);
							char str[1000];
							sprintf(str, "%06x",address);
							strcpy(s2,str);
						}else if (strcmp(inter[i].opcode,"END")==0){
							int start = (int)strtol(inter[i].operand, NULL, 16);
							fprintf(fptr2,"\nE%06x\n",start);
							fprintf(fptr1,"\t\t");
						}else if(strcmp(inter[i].opcode,"RESW")==0 || strcmp(inter[i].opcode,"RESB")==0){	
						}else{
							printf("Invalid Opcode\n");
							exit(0);
						}
						fprintf(fptr1,"%s\t%s\t%s\t\t%s\t%s\n",inter[i].address,inter[i].label,inter[i].opcode,inter[i].operand,s2);
					}
				}
				
				limiter--;
			}
		}



	fclose(fptr1);
	fclose(fptr2);
}

int main(){
	opcode_fetch();
	int no_lines = fetch_program();
	int program_size = pass_one(no_lines);
	pass_two(no_lines,program_size);
}
