.DATA
	numberHolder DB ?
.CODE

;-----------------------------------------
;	displayStockDetails
;	display the Stock Details
;-----------------------------------------
displayStockDetails PROC
	MOV AH, 02H
	MOV DL, numStockFound
	ADD DL, 30H
	INT 21H

	MOV AH, 02H
	MOV DL, '.'
	INT 21H
	
	MOV AH, 09H
	LEA DX, Stock.phoneName
	INT 21H
	
	MOV AH, 09H
	LEA DX, Stock.phoneRAM
	INT 21H
	
	MOV AH, 09H
	LEA DX, Stock.phoneROM
	INT 21H
	
	MOV AH, 09H
	LEA DX, Stock.phoneColor
	INT 21H
	
	MOV AX, WORD PTR Stock.phonePrice
	MOV DX, WORD PTR Stock.phonePrice + 2
	CALL displayNumber
	
	
	MOV AH, 09H
	LEA DX, newLine
	INT 21H
	
	RET
displayStockDetails ENDP


displayNumber PROC
	MOV CX, 0
displayNumberLoop:
	DIV ten16Bits
	PUSH DX
	INC CX
	MOV DX, 0000H
	CMP AX, DX
	JNE displayNumberLoop

printNumber:
	POP AX
	MOV AH, 02H
	MOV DL, AL
	ADD DL, 30H
	INT 21H
LOOP printNumber
	RET
displayNumber ENDP