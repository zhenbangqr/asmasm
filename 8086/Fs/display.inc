.DATA
	stockDetailsTitle DB "No. Phone Name     | RAM       | ROM      | Color     | Price     | Quantity $"
.CODE

;-----------------------------------------
;	displaySearchTitle
;	display the search result title
;-----------------------------------------
displaySearchTitle PROC
	displayDivider
	print_newLine
	MOV AH, 09H
	LEA DX, stockDetailsTitle
	INT 21H
	print_newLine
	displayDivider
	print_newLine
	
	RET
displaySearchTitle ENDP

;-----------------------------------------
;	displayStockDetails
;	display the Stock Details
;-----------------------------------------
displayStockDetails PROC
	MOV AX, 0
	MOV AL, numStockFound
	DIV ten8Bits
	
	MOV BX, AX
	MOV AH, 02H
	MOV DL, BL
	ADD DL, 30H
	INT 21H
	
	MOV AH, 02H
	MOV DL, BH
	ADD DL, 30H
	INT 21H

	MOV AH, 02H
	MOV DL, '.'
	INT 21H

	space
	
	MOV AH, 09H
	LEA DX, Stock.phoneName
	INT 21H
	
	;fill in space
	MOV CX, 15						;set the size
	LEA DI, Stock.phoneName
	CALL calculateStringLength
	SUB CX, BX
	CALL fillSpace

	delimiter
	space
	
	MOV AH, 09H
	LEA DX, Stock.phoneRAM
	INT 21H
	
	;fill in space
	MOV CX, 10						;set the size
	LEA DI, Stock.phoneRAM
	CALL calculateStringLength
	SUB CX, BX
	CALL fillSpace
	
	delimiter
	
	MOV AH, 09H
	LEA DX, Stock.phoneROM
	INT 21H
	
	;fill in space
	MOV CX, 10						;set the size
	LEA DI, Stock.phoneROM
	CALL calculateStringLength
	SUB CX, BX
	CALL fillSpace
	
	delimiter
	space
	
	MOV AH, 09H
	LEA DX, Stock.phoneColor
	INT 21H

	;fill in space
	MOV CX, 10						;set the size
	LEA DI, Stock.phoneColor
	CALL calculateStringLength
	SUB CX, BX
	CALL fillSpace
	
	delimiter
	space

	MOV BX, 10
	MOV AX, WORD PTR Stock.phonePrice
	MOV DX, WORD PTR Stock.phonePrice + 2
	CALL displayNumber
	
	MOV AH, 02H
	MOV DL, '.'
	INT 21H
	DEC BL
	
	MOV AX, WORD PTR Stock.phonePriceFP
	MOV DX, WORD PTR Stock.phonePriceFP + 2
	CALL displayNumber
	CALL fillSpace
	
	delimiter
	space
	
	MOV AX, WORD PTR Stock.phoneQty
	MOV DX, WORD PTR Stock.phoneQty + 2
	MOV BX, 10
	CALL displayNumber
	CALL fillSpace
	
	MOV AH, 09H
	LEA DX, newLine
	INT 21H
	
	RET
displayStockDetails ENDP


;-------------------------------------------
;	displayNumber
;	display the number, and mov the number 
;	of space need to fill into CX
;-------------------------------------------
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
	DEC BX
	POP AX
	MOV AH, 02H
	MOV DL, AL
	ADD DL, 30H
	INT 21H
LOOP printNumber
	MOV CX, 0
	MOV CX, BX
	
	RET
displayNumber ENDP


;---------------------------------------
;	fillSpace
;   Fill with space after displayNumber
;---------------------------------------
fillSpace PROC

autoFillSpace:
	MOV AL, ' '
	MOV AH, 02H
	MOV DL, AL
	INT 21H
LOOP autoFillSpace

	RET
fillSpace ENDP
	