#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>

#include<string.h>



void display1() {

    char str;
    FILE *fp1, *fp2, *fp3;   
	
    printf("\n----------------Pass One-----------------\n\n");



    // 1. Input Table
    printf("\nThe contents of Input Table :\n\n");
    fp1 = fopen("input.txt", "r");
    str = fgetc(fp1);
    while (str != EOF) {
        printf("%c", str);
        str = fgetc(fp1);
    }
    fclose(fp1);

    //2. Output Table
    printf("\n\nThe contents of Intermediate Table :\n\n");
    fp2 = fopen("inter.txt", "r");
    str = fgetc(fp2);
    while (str != EOF) {
        printf("%c", str);
        str = fgetc(fp2);
    }
    fclose(fp2);

    // 3. Symtable
    printf("\n\nThe contents of Symbol Table :\n\n");
    fp3 = fopen("symtab.txt", "r");
    str = fgetc(fp3);
    while (str != EOF) {
        printf("%c", str);
        str = fgetc(fp3);
    }
    fclose(fp3);
	
}


void display2() {
    char ch;
    FILE *fp1,*fp2, *fp3, *fp4;

    printf("\n-----------------------------Pass Two-------------------------------\n\nIntermediate file is converted into object code");


    printf("\n\nThe contents of Assembly Listing file :\n\n");
    fp1 = fopen("list.txt", "r");
    ch = fgetc(fp1);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = fgetc(fp1);
    }
    fclose(fp1);

    printf("\n\nThe contents of Object code file :\n\n");
    fp4 = fopen("object.txt", "r");
    ch = fgetc(fp4);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = fgetc(fp4);
    }
    
    printf("\n");
    fclose(fp4);

}

void main() {
  char opcode[10], operand[10], label[10], a[10], ad[10], symbol[10], ch;
  char code[10][10], code1[10][10] = {
    "33",
    "44",
    "53",
    "57"
  };
  char mnemonic[10][10] = {
    "START",
    "LDA",
    "STA",
    "LDCH",
    "STCH",
    "END"
  };
  char mnemonic1[10][10] = {
    "LDA",
    "STA",
    "LDCH",
    "STCH"
  };
  int locctr, start, length, i = 0, j = 0, k, l = 0;
  int st, diff, address, add, len, actual_len, finaddr, prevaddr;
  FILE * fp1, * fp2, * fp3, * fp4, * fp5, * fp6, * fp7;

  fp1 = fopen("input.txt", "r");
  fp2 = fopen("symtab.txt", "w");
  fp3 = fopen("inter.txt", "w");
  fscanf(fp1, "%s%s%s", label, opcode, operand);
  if (strcmp(opcode, "START") == 0) {
    start = (int)strtol(operand, NULL, 16);
    locctr = start;
    fprintf(fp3, "%s\t%s\t%s\n", label, opcode, operand);
    fscanf(fp1, "%s%s%s", label, opcode, operand);
  } else
    locctr = 0;
  while (strcmp(opcode, "END") != 0) {
    fprintf(fp3, "%x", locctr);
    if (strcmp(label, "**") != 0)
      fprintf(fp2, "%s\t%x\n", label, locctr);
    strcpy(code[i], mnemonic[j]);
    while (strcmp(mnemonic[j], "END") != 0) {
      if (strcmp(opcode, mnemonic[j]) == 0) {
        locctr += 0x3;
        break;
      }
      strcpy(code[i], mnemonic[j]);
      j++;
    }
    if (strcmp(opcode, "WORD") == 0)
      locctr += 0x3;
    else if (strcmp(opcode, "RESW") == 0)
      locctr += (0x3 * (atoi(operand)));
    else if (strcmp(opcode, "RESB") == 0)
      locctr += (0x1)*(atoi(operand));
    else if (strcmp(opcode, "BYTE") == 0)
      locctr = locctr+0x1;
    fprintf(fp3, "\t%s\t%s\t%s\n", label, opcode, operand);
    fscanf(fp1, "%s%s%s", label, opcode, operand);
  }
  fprintf(fp3, "%x\t%s\t%s\t%s\n", locctr, label, opcode, operand);
  length = locctr - start;
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  printf("\n\nThe contents of Input file:\n\n");
  fp1 = fopen("input.txt", "r");
  ch = fgetc(fp1);
  while (ch != EOF) {
    printf("%c", ch);
    ch = fgetc(fp1);
  }
  printf("\n\nLength of the input program is %d.", length);
  printf("\n\nThe contents of Symbol Table:\n\n");
  fp2 = fopen("symtab.txt", "r");
  ch = fgetc(fp2);
  while (ch != EOF) {
    printf("%c", ch);
    ch = fgetc(fp2);
  }
  fclose(fp1);
  fclose(fp2);
  fp4 = fopen("list.txt", "w");
  fp5 = fopen("symtab.txt", "r");
  fp6 = fopen("inter.txt", "r");
  fp7 = fopen("object.txt", "w");
  fscanf(fp6, "%s%s%s", label, opcode, operand);
  while (strcmp(opcode, "END") != 0) {
    prevaddr = address;
    fscanf(fp6, "%x%s%s%s", & address, label, opcode, operand);
  }
  finaddr = address;
  fclose(fp6);
  fp6 = fopen("inter.txt", "r");
  fscanf(fp6, "%s%s%s", label, opcode, operand);
  if (strcmp(opcode, "START") == 0) {
    fprintf(fp4, "\t%s\t%s\t%s\n", label, opcode, operand);
    fprintf(fp7, "H^%s^00%s^00%x\n", label, operand, finaddr);
    fscanf(fp6, "%x%s%s%s", & address, label, opcode, operand);
    st = address;
    diff = prevaddr - st;
    fprintf(fp7, "T^00%x^%x", address, diff);
  }
  while (strcmp(opcode, "END") != 0) {
    if (strcmp(opcode, "BYTE") == 0) {
      fprintf(fp4, "%x\t%s\t%s\t%s\t", address, label, opcode, operand);
      len = strlen(operand);
      actual_len = len - 3;
      fprintf(fp7, "^");
      for (k = 2; k < (actual_len + 2); k++) {
        itoa(operand[k], ad, 16);
        fprintf(fp4, "%s", ad);
        fprintf(fp7, "%s", ad);
      }
      fprintf(fp4, "\n");
    } else if (strcmp(opcode, "WORD") == 0) {
      len = strlen(operand);
      itoa(atoi(operand), a, 10);
      fprintf(fp4, "%x\t%s\t%s\t%s\t00000%s\n", address, label, opcode, operand, a);
      fprintf(fp7, "^00000%s", a);
    } else if ((strcmp(opcode, "RESB") == 0) || (strcmp(opcode, "RESW") == 0))
      fprintf(fp4, "%x\t%s\t%s\t%s\n", address, label, opcode, operand);
    else {
      while (strcmp(opcode, mnemonic1[l]) != 0)
        l++;
      if (strcmp(operand, "COPY") == 0)
        fprintf(fp4, "%x\t%s\t%s\t%s\t%s0000\n", address, label, opcode, operand, code1[l]);
      else {
        rewind(fp5);
        fscanf(fp5, "%s%x", symbol, & add);
        while (strcmp(operand, symbol) != 0)
          fscanf(fp5, "%s%x", symbol, & add);
        fprintf(fp4, "%x\t%s\t%s\t%s\t%s%x\n", address, label, opcode, operand, code1[l], add);
        fprintf(fp7, "^%s%x", code1[l], add);
      }
    }
    fscanf(fp6, "%x%s%s%s", & address, label, opcode, operand);
  }
  fprintf(fp4, "%x\t%s\t%s\t%s\n", address, label, opcode, operand);
  fprintf(fp7, "\nE^00%x", st);
  fclose(fp7);
  fclose(fp6);
  fclose(fp5);
  fclose(fp4);
  display1();
  display2();
  getch();
}