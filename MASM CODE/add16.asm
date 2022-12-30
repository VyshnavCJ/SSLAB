 
ASSUME CS:CODE, DS:DATA 
DATA SEGMENT 
 	M1 	DB 10,13,"ENTER FIRST NUMBER: $" 
 	M2 	DB 10,13,"ENTER SECOND NUMBER: $" 
 	M3 	DB 10,13,"SUM: $" 
    SUM DB 03 
DATA ENDS 
 
PRTMSG MACRO MESSAGE
    LEA DX, MESSAGE
    MOV AH,09
    INT 21H
    ENDM
GETDCM MACRO
    MOV AH, 01
    INT 21H
    SUB AL, 30H
    71
    ENDM
 
PRTDCM 	MACRO 
    MOV 	DL,[SI] 
    ADD 	DL, 30H 
    MOV 	AH, 02 
    INT 	21H 
    ENDM 

CODE SEGMENT 
	START: MOV 	AX, DATA 
	 	MOV DS, AX 
	 	PRTMSG 	M1 
	 	GETDCM 
 	    MOV BH, AL  	
        GETDCM 
	 	MOV BL, AL 
	 	PRTMSG 	M2 
	 	GETDCM 
 	    MOV CH, AL  	
        GETDCM 
	 	MOV CL, AL 
	 	ADD BL, CL 
        MOV AL, BL 
 	    MOV AH, 00 
 	    AAA
 	    LEA SI, SUM 
 	    MOV [SI], AL 
 	    ADD BH, AH 
 	    ADD BH, CH 
 	    MOV AL, BH 
 	    MOV AH, 00
 	    AAA 
        INC SI 
 	    MOV [SI], AL 
 	    INC SI 
 	    MOV [SI], AH 
        PRTMSG 	M3 
 	    PRTDCM  	
        DEC SI  	
        PRTDCM
        DEC SI 
	 	PRTDCM 
	 	MOV AH, 4CH 
	 	INT 21H 
CODE ENDS 
END START 
