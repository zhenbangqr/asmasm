.MODEL SMALL
.STACK 100
.DATA
		NUM1 DB ?
		NUM2 DB ?
		STR1 DB 'Enter 1st digit: $'
		STR2 DB 'Enter 2nd digit: $'
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

		MOV AH,01H
		INT 21H
		
		MOV AH,09H
		LEA DX,NL
		INT 21H
		
		MOV AH,09H
		LEA DX,STR2
		INT 21H
		
		MOV AH,01H
		INT 21H
		SUB AL,30H
		MOV NUM2,AL
		
		MOV AH,01H
		INT 21H
		
		MOV AH,09H
		LEA DX,NL
		INT 21H
		
		MOV AH,02H
		MOV BH,NUM1
		ADD BH,NUM2
		MOV DL,BH
		ADD DL,30H

		INT 21H
		
		MOV AX,4C00H
		INT 21H

MAIN ENDP
END MAIN