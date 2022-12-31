#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display();

int main()
{
    char a[10], ad[10], label[10], opcode[10], operand[10], symbol[10];
    int start=0x0, diff=0x0, i=0x0, address=0x0, add=0x0, len=0x0, actual_len=0x0, finaddr=0x0, prevaddr=0x0, j = 0;
    char mnemonic[15][15] = {"LDA", "STA", "LDCH", "STCH"};
    char code[15][15] = {"33", "44", "53", "57"};

    FILE *fp1, *fp2, *fp3, *fp4;
    fp1 = fopen("output.txt", "w");
    fp2 = fopen("symtab.txt", "r");
    fp3 = fopen("intermediate.txt", "r");
    fp4 = fopen("objcode.txt", "w");

    fscanf(fp3, "%s\t%s\t%s", label, opcode, operand);

    while (strcmp(opcode, "END") != 0)
    {
        prevaddr = address;
        fscanf(fp3, "%x%s%s%s", &address, label, opcode, operand);
    }
    finaddr = address;
    
    fclose(fp3);
    fp3 = fopen("intermediate.txt", "r");

    fscanf(fp3, "\t%s\t%s\t%s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0)
    {
        fprintf(fp1, "\t%s\t%s\t%s\n", label, opcode, operand);
        fprintf(fp4, "H^%s^00%s^00%x\n", label, operand, finaddr);
        fscanf(fp3, "%x%s%s%s", &address, label, opcode, operand);
        start = address;
        diff = prevaddr - start;
        fprintf(fp4, "T^00%x^%x", address, diff);
    }

    while (strcmp(opcode, "END") != 0)
    {
        if (strcmp(opcode, "BYTE") == 0)
        {
            fprintf(fp1, "%x\t%s\t%s\t%s\t", address, label, opcode, operand);
            len = strlen(operand);
            actual_len = len - 3;
            fprintf(fp4, "^");
            for (i = 2; i < (actual_len + 2); i++)
            {   
                //itoa(operand[i], ad, 16);
                sprintf(ad, "%x", operand[i]);
                
                fprintf(fp1, "%s", ad);
                fprintf(fp4, "%s", ad);
            }
            fprintf(fp1, "\n");
        }

        else if (strcmp(opcode, "WORD") == 0)
        {
            fprintf(fp1, "%x\t%s\t%s\t%s\t00000%s\n", address, label, opcode, operand, operand);
            fprintf(fp4, "^00000%s", operand);
        }

        else if ((strcmp(opcode, "RESB") == 0) || (strcmp(opcode, "RESW") == 0)) {
            fprintf(fp1, "%x\t%s\t%s\t%s\n", address, label, opcode, operand);
        }

        else
        {
            while (strcmp(opcode, mnemonic[j]) != 0)
                j++;
            if (strcmp(operand, "COPY") == 0)
                fprintf(fp1, "%x\t%s\t%s\t%s\t%s0000\n", address, label, opcode, operand, code[j]);
            else
            {
                rewind(fp2);
                fscanf(fp2, "%s%x", symbol, &add);
                while (strcmp(operand, symbol) != 0)
                    fscanf(fp2, "%s%x", symbol, &add);
                fprintf(fp1, "%x\t%s\t%s\t%s\t%s%x\n", address, label, opcode, operand, code[j], add);
                fprintf(fp4, "^%s%x", code[j], add);
            }
        }

        fscanf(fp3, "%x%s%s%s", &address, label, opcode, operand);
    }

    fprintf(fp1, "%x\t%s\t%s\t%s\n", address, label, opcode, operand);
    fprintf(fp4, "\nE^00%x", start);
    
    fclose(fp4);
    fclose(fp3);
    fclose(fp2);
    fclose(fp1);

    display();

    return 0;
}

void display() {
    char ch;
    FILE *fp1, *fp2, *fp3, *fp4;

    printf("\nIntermediate file is converted into object code");

    printf("\n\nThe contents of Intermediate file:\n\n");
    fp3 = fopen("intermediate.txt", "r");
    ch = fgetc(fp3);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = fgetc(fp3);
    }
    fclose(fp3);

    printf("\n\nThe contents of Symbol Table :\n\n");
    fp2 = fopen("symtab.txt", "r");
    ch = fgetc(fp2);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = fgetc(fp2);
    }
    fclose(fp2);

    printf("\n\nThe contents of Output file :\n\n");
    fp1 = fopen("output.txt", "r");
    ch = fgetc(fp1);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = fgetc(fp1);
    }
    fclose(fp1);

    printf("\n\nThe contents of Object code file :\n\n");
    fp4 = fopen("objcode.txt", "r");
    ch = fgetc(fp4);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = fgetc(fp4);
    }
    fclose(fp4);

}