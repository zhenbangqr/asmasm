.DATA
    filename DB 'Phone.txt', 0
    buffer DB 512 DUP(?)    ; Buffer to store file content
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
	MOV CX, 512				;Read 128 bytes
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
	PUSH CX
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
	POP CX
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
	MOV Stock.phonePrice, BX
	INC SI
continueReadPrice:
	MOV BL, [SI]
	CMP BL, '.'
	JE readStockPriceFP	;continue read floating point
	
	MOV AX, Stock.phonePrice
	MUL ten8Bits
	SUB BL, 30H
	ADD BX, AX
	MOV Stock.phonePrice, AX
	INC SI
	
	JMP continueReadPrice

readStockPriceFP:
	MOV AX, 0
	MOV BX, 0
	MOV BL, [SI]
	SUB BL, 30H
	MOV Stock.phonePriceFP, BX
	INC SI
continueReadPriceFP:
	MOV BL, [SI]
	CMP BL, ';'
	JE endReadPrice	;continue read floating point
	
	MOV AX, Stock.phonePriceFP
	MUL ten8Bits
	SUB BL, 30H
	ADD BX, AX
	MOV Stock.phonePriceFP, AX
	INC SI
	
	JMP continueReadPrice
	
endReadPrice:
	INC SI
	RET
	
readStockPrice ENDP


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

