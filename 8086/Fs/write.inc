.DATA  
	stockFileTitle DB "Name;RAM;ROM;Color;Price;Qty;StockInQty;StockOutQty;$"

	errorMsg DB "Error find file$"

	writeBuffer DB 1024 DUP(?)    ; Buffer to store file content
	bufferLength DW ?
	bufferIndex DW 0
	tempFileName DB 'temp.txt', 0
	fileHandleWrite DW ?         ; To store the file handle
	
.CODE

openWriteTempFile PROC

	MOV AH, 3DH					;Open file
	MOV AL, 1					;Open for write
	MOV DX, OFFSET tempFileName
	INT 21H
	mov fileHandleWrite, AX			;Store File Handle
	
	RET
	
openWriteTempFile ENDP

openWriteStockFile PROC

	MOV AH, 3DH					;Open file
	MOV AL, 1					;Open for write
	MOV DX, OFFSET stockFileName
	INT 21H
	mov fileHandleWrite, AX			;Store File Handle
	
	RET
	
openWriteStockFile ENDP

closeWriteFile PROC

	MOV BX, fileHandleWrite
	MOV AH, 3EH
	INT 21H
	
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

	resetBX
	resetAX
	
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

	resetCX
	
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

	CALL openWriteStockFile
	CALL writeStockFileTitle
	
	CALL openReadTempFile
	CALL jumpToNextLine
	
	writeBackToOriginFile:
		MOV AL, [SI]
		CMP AX, 0
		JE endWriteBackToOrigin
		
		CALL clearStockVariableBuffer
		CALL readStockDetails
		CALL appendToBuffer
		JMP writeBackToOriginFile
		
	endWriteBackToOrigin:
		CALL closeWriteFile
		CALL closeReadFile
		MOV AH, 41h        	  ; DOS function to delete a file
		LEA DX, tempFileName ; Address of file to delete (Phone.txt)
		INT 21H            	  ; Call DOS

		MOV AH, 3Ch         ; DOS function to create a file
		LEA DX, tempFileName ; Address of the file name
		MOV CX, 0           ; File attributes (0 = normal file)
		INT 21H             ; Call DOS interrupt
	
	done:
		RET
	
	handleError:
		; Handle the error (you can print an error message or take appropriate action)
		MOV AH, 09h          ; Print error message
		LEA DX, errorMsg
		INT 21H
		
updateStockFileName ENDP