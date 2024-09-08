.data  
	stockFileTitle DB "Name;RAM;ROM;Color;Price;Qty;StockInQty;StockOutQty;$"

	writeBuffer DB 1024 DUP(?)    ; Buffer to store file content
	bufferLength DW ?
	bufferIndex DW 0
	tempFileName DB "temp.txt"
	fileHandleWrite DW ?         ; To store the file handle
.code

openWriteStockFile PROC
	MOV AH, 3DH					;Open file
	MOV AL, 2					;Open for write
	MOV DX, OFFSET tempFileName
	INT 21H
	mov fileHandleWrite, AX			;Store File Handle
	
	RET
openWriteStockFile ENDP


closeWriteFile PROC
	MOV BX, fileHandleWrite
	MOV AH, 3EH
	INT 21H
	
	MOV fileHandleWrite, 0
	
	RET
closeWriteFile ENDP

;----------------------------------------
;	writeStockFileTitle
;	write the title of the stock file
;----------------------------------------
writeStockFileTitle PROC
	MOV SI, bufferIndex
	LEA SI, writeBuffer

	LEA DI, stockFileTitle
	CALL appendString
	
	LEA DI, newLine
	call appendString
	
	MOV AL, '$'
	MOV [SI], AL
	INC SI
	
	CALL writeStockFile
	
	RET
writeStockFileTitle ENDP

;----------------------------------------
;	appendStringToBuffer
;	append the data to the write buffer
;----------------------------------------
appendToBuffer PROC
	PUSH SI ;This SI hold the readFilePointer
	
	MOV SI, bufferIndex
	LEA SI, writeBuffer

	LEA DI, Stock.phoneName
	call appendString
	
	MOV AL, ';'
	MOV [SI], AL
	INC SI
	
	LEA DI, Stock.phoneRAM
	call appendString

	MOV AL, ';'
	MOV [SI], AL
	INC SI
		
	LEA DI, Stock.phoneROM
	call appendString
	
	MOV AL, ';'
	MOV [SI], AL
	INC SI
	
	LEA DI, Stock.phoneColor
	call appendString
	
	MOV AL, ';'
	MOV [SI], AL
	INC SI
	
	MOV AX, WORD PTR Stock.phonePrice
	MOV DX, WORD PTR Stock.phonePrice + 2
	CALL appendNumber

	MOV AL, '.'
	MOV [SI], AL
	INC SI
	
	MOV AX, WORD PTR Stock.phonePriceFP
	MOV DX, WORD PTR Stock.phonePriceFP + 2
	CALL appendNumber
	
	MOV AL, ';'
	MOV [SI], AL
	INC SI
	
	MOV AX, WORD PTR Stock.phoneQty
	MOV DX, WORD PTR Stock.phoneQty + 2
	CALL appendNumber
	
	MOV AL, ';'
	MOV [SI], AL
	INC SI
	
	MOV AX, WORD PTR Stock.totalStockInQty
	MOV DX, WORD PTR Stock.totalStockInQty + 2
	CALL appendNumber
	
	MOV AL, ';'
	MOV [SI], AL
	INC SI
	
	MOV AX, WORD PTR Stock.totalStockOutQty
	MOV DX, WORD PTR Stock.totalStockOutQty + 2
	CALL appendNumber
	
	MOV AL, ';'
	MOV [SI], AL
	INC SI
	
	LEA DI, newLine
	call appendString
	
	MOV AL, '$'
	MOV [SI], AL
	INC SI
	
	MOV bufferIndex, SI
	
	CALL writeStockFile
	
	
	
	POP SI
	RET
appendToBuffer ENDP

;-----------------------------------------------
;	appendString
;	append the string
;-----------------------------------------------
appendString PROC
	MOV AL, [DI]
	CMP AL, '$'
	JE doneAppending
	
	MOV [SI], AL
	INC DI
	INC SI
	JMP appendString

doneAppending:
	RET
appendString ENDP


;---------------------------------
;	writeStockFile
;	write the buffer to the file
;---------------------------------
writeStockFile PROC
	LEA DI, writeBuffer
	CALL countBufferLength

    MOV AH, 40H
	MOV BX, fileHandleWrite
	LEA DX, writeBuffer
	MOV CX, bufferLength
	INT 21H
	
	LEA DI, writeBuffer
	
	;clear the Buffer
		MOV CX, bufferLength  ; Buffer size
		LEA DI, writeBuffer
		MOV AL, 0
	clearLoop:
		MOV [DI], AL
		INC DI
		LOOP clearLoop

	MOV bufferIndex, 0

	RET
writeStockFile ENDP


;-----------------------------------------------
;	countBufferLength
;	count the length of the buffer for writting
;-----------------------------------------------
countBufferLength PROC
	MOV BX, 0
	MOV AX, 0 
	countLengthOfBuffer:
			;--Count the length of string by comparing '$' symbol
			
			MOV AL, [DI]
			
			CMP AL, '$';
			JE endReadBufferLength
			INC DI
			INC BL
			JMP countLengthOfBuffer
			
	endReadBufferLength:
		MOV bufferLength, BX
		RET
countBufferLength ENDP

;---------------------------------------
;	appendNumber
;	append the number 
;---------------------------------------
appendNumber PROC
	MOV CX, 0
	appendNumberLoop:
		DIV ten16Bits
		PUSH DX
		INC CX
		MOV DX, 0000H
		CMP AX, DX
		JNE appendNumberLoop
	
	appendNumberToBuffer:
		POP AX
		ADD AL, 30H
		MOV [SI], AL
		INC SI
	LOOP appendNumberToBuffer
	
	RET
appendNumber ENDP

;----------------------------------------------------
;	updateFile
;	change the new file name and clear the older one
;----------------------------------------------------
updateStockFileName PROC
	MOV AH, 56h          ; DOS function to rename a file
    LEA DX, tempFileName  ; Address of the current file name
    LEA DI, stockFileName ; Address of the new file name
    INT 21H              ; Call DOS
	
	RET
updateStockFileName ENDP


switchStockFileName PROC
	CALL updateStockFileName
	
	MOV AH, 3Dh          ; DOS function to open a file
    LEA DX, tempFileName     ; Address of the file name
    MOV AL, 1            ; Open for write access (truncates the file)
    INT 21H              ; Call DOS

; Close the file (finalize truncation)
    MOV AH, 3Eh          ; DOS function to close a file
    MOV BX, AX           ; File handle from the open operation
    INT 21H              ; Call DOS


	RET
switchStockFileName ENDP