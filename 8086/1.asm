.MODEL SMALL
.STACK 100
.DATA

		STR1 DB 'Enter digit 1: $'
		STR4 DB 'Sum is '
		STR5 DB 'Sum is '
		STR5 DB 'Difference are $'
		NUM1 DB ?
		NUM2 DB ?
		NL DB 0AH,0DH,'$'


.CODE
MAIN PROC

		MOV AX,@DATA
		MOV DS,AX
		
		MOV AH,09H
		LEA DX,STR1
		INT 21H
		
		MOV AH,01H
		INT 21H
		SUB AL,30H
		MOV NUM1,AL
		
		MOV AH,02H
		MOV DL,NL
		INT 21H
		
		MOV AH,09H
		LEA DX,STR2
		INT 21H
		
		MOV AH,01H
		INT 21H
		SUB AL,30H
		MOV NUM2,AL
		
		MOV AH,02H
		MOV DL,NL
		INT 21H
		
		MOV AH,09H
		LEA DX,STR3
		INT 21H
		
		MOV AL,NUM1
		MUL NUM2
		MOV BX,AX
		
		MOV AH,02H
		MOV DL,BL
		ADD DL,30H
		INT 21H
		
		MOV AX,4C00H
		INT 21H

MAIN ENDP
END MAIN