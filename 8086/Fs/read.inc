.DATA
    filename DB 'Phone.txt', 0
    buffer DW 1024 DUP(?)    ; Buffer to store file content
    fileHandle DW ?         ; To store the file handle
	fileErrorMsg DB "Error Occured!$"
.code

;--------------------------------------------
;	openReadStockFile
;	open the stock file(Phone.txt) for read
;--------------------------------------------
openReadStockFile PROC

	MOV AH, 3DH				;Open file
	MOV AL, 0				;Open for reading
	MOV DX, OFFSET filename	;
	INT 21H
	JC fileError			;Jump when carry flag set(error when open file)
	mov fileHandle, AX		;Store File Handle
	
	MOV BX, fileHandle		;Use the fileHandle
	LEA DX, buffer			;Address for buffer
	MOV CX, 1024			;Read 1024 bytes
	MOV AH, 3FH				;Read File
	INT 21H
	JC fileError			;Jump when carry flag set(error when open file)
	
	LEA SI, buffer
	
	RET

openReadStockFile ENDP


;------------------------------------------------------
;	closeStockFile
;	close the stock file(Phone.txt)
;------------------------------------------------------
closeStockFile PROC
	mov bx, fileHandle
    mov ah, 3Eh             ; DOS function: close file
    int 21h
    jc fileError            ; If carry flag is set, an error occurred
	
	RET
closeStockFile ENDP

;--------------------------------------------------
;	fileError
;	Show a message when error happen to the file
;	and close the file
;--------------------------------------------------
fileError:
    ; Handle file error
    mov ah, 09h
    lea dx, fileErrorMsg
    int 21h
    jmp exitReadFile

exitReadFile:
    call closeStockFile
	
;-----------------------------------------
;	readStringFromFile
;	Read a string from file to a holder
;-----------------------------------------
readStringFromFile PROC
continueReadString:
	MOV AL, [SI]
	CMP AL, ';'
	JE loopFillSpace
	MOV [DI], AL 
	DEC CX
	INC SI
	INC DI
	JMP continueReadString
	
loopFillSpace:
	MOV AL, ' '
	MOV [DI], AL
	INC DI
LOOP loopFillSpace
	
endReadString:
	INC SI
	RET

readStringFromFile ENDP
	
;---------------------------------------
;	readStockPrice
;	read the price from the file
;---------------------------------------
readStockPrice PROC
	MOV AX, 0
	MOV BX, 0
	
	MOV BL, [SI]
	SUB BL, 30H
	
	MOV AX, BX
	INC SI
continueReadPrice:
	MOV BL, [SI]
	CMP BL, '.'
	JE endReadPrice
	
	MUL ten16Bits
	SUB BL, 30H
	ADD AX, BX
	
	INC SI
	
	JMP continueReadPrice
endReadPrice:
	MOV WORD PTR Stock.phonePrice, AX
	MOV WORD PTR Stock.phonePrice + 2, DX

	INC SI
	
	JMP readPriceFP

readPriceFP:
	MOV AX, 0
	MOV BX, 0
	
	MOV BL, [SI]
	SUB BL, 30H
	
	MOV AX, BX
	INC SI
	
continueReadPriceFP:
	MOV BL, [SI]
	CMP BL, ';'
	JE endReadPriceFP
	
	MUL ten16Bits
	SUB BL, 30H
	ADD AX, BX
	
	INC SI
	
	JMP continueReadPriceFP
endReadPriceFP:
	MOV WORD PTR Stock.phonePriceFP, AX
	MOV WORD PTR Stock.phonePriceFP + 2, DX

	INC SI
	RET
	
readStockPrice ENDP

;----------------------------------------
;	readNumber
;	read the number from file
;----------------------------------------
readNumber PROC
	resetAX
	resetBX
	resetCX
	resetDX
	
	MOV BL, [SI]
	SUB BL, 30H
	
	MOV AX, BX
	INC SI
	
continueReadNumber:
	MOV BL, [SI]
	CMP BL, ';'
	JE endReadNumber
	
	MUL ten16Bits
	SUB BL, 30H
	ADD AX, BX
	
	INC SI
	
	JMP continueReadNumber

endReadNumber:
	INC SI
	RET

readNumber ENDP


;---------------------------------------
;	readStockDetails
;	Read one line of the file
;---------------------------------------
readStockDetails PROC
	LEA DI, Stock.phoneName
	MOV CX, 15
	CALL readStringFromFile

	LEA DI, Stock.phoneRAM
	MOV CX, 10
	CALL readStringFromFile
	
	LEA DI, Stock.phoneROM
	MOV CX, 10
	CALL readStringFromFile
	
	LEA DI, Stock.phoneColor
	MOV CX, 10
	CALL readStringFromFile
	
	CALL readStockPrice
	
	CALL readNumber
	MOV WORD PTR [Stock.phoneQty], AX
	MOV WORD PTR [Stock.phoneQty + 2], DX
	
	CALL readNumber
	MOV WORD PTR [Stock.totalStockInQty], AX
	MOV WORD PTR [Stock.totalStockInQty + 2], DX
	
	CALL readNumber
	MOV WORD PTR [Stock.totalStockOutQty], AX
	MOV WORD PTR [Stock.totalStockOutQty + 2], DX

	
	CALL jumpToNextLine
	
	RET
readStockDetails ENDP



;-------------------------------------------------
;	jumpToNextLine
;	Jump to the next line in a file while reading
;-------------------------------------------------
jumpToNextLine PROC
nextLineCheck:
	MOV AL, [SI]
	CMP AL, 0DH
	JE lineFeedCheck
	INC SI
	JMP nextLineCheck

lineFeedCheck:
	INC SI
	MOV AL, [SI]
	CMP AL, 0AH
	JE endSkipLine
	
	JMP nextLineCheck

endSkipLine:
	INC SI
	RET
	
jumpToNextLine ENDP

