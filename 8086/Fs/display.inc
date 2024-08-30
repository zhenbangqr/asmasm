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
	
	MOV BX, Phone.phonePrice
	CALL displayNumber
	
	
	MOV AH, 09H
	LEA DX, newLine
	INT 21H
	
	RET
displayStockDetails ENDP



displayNumber PROC
	MOV AX, Stock.phonePrice
	MOV CX, 0

displayNumberLoop:
	DIV ten8Bits
	MOV numberHolder, AL
	MOV AL, AH
	MOV AH, 0
	PUSH AX
	MOV AL, numberHolder
	INC CX
	CMP AL, 0
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