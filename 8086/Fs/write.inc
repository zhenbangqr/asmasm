.DATA  
	stockFileTitle DB "Name;RAM;ROM;Color;Price;Qty;StockInQty;StockOutQty;$"

	errorMsg DB "Error find file$"

	writeBuffer DB 1024 DUP(?)    ; Buffer to store file content
	bufferLength DW ?
	bufferIndex DW 0
	tempFileName DB 'TEMP.TXT', 0
	fileHandleWrite DW ?         ; To store the file handle
	
.CODE

;----------------------------------------
;	openWriteTempFile
;	open temp file for write
;----------------------------------------
openWriteTempFile PROC

	MOV AH, 3DH						;Open file
	MOV AL, 1						;Open for write
	MOV DX, OFFSET tempFileName 	;Open temp.txt
	INT 21H
	mov fileHandleWrite, AX			;Store File Handle
	
	RET
	
openWriteTempFile ENDP


;----------------------------------------
;	openWriteStockFile
;	open stock file for write
;----------------------------------------
openWriteStockFile PROC

	MOV AH, 3DH						;Open file
	MOV AL, 1						;Open for write
	MOV DX, OFFSET stockFileName	;Open phone.txt
	INT 21H
	mov fileHandleWrite, AX			;Store File Handle
	
	RET
	
openWriteStockFile ENDP

;----------------------------------------
;	closeWriteFile
;	close the writing file
;----------------------------------------
closeWriteFile PROC

	MOV BX, fileHandleWrite		;use file handler for writing
	MOV AH, 3EH					;close file
	INT 21H
	
	RET
	
closeWriteFile ENDP

;----------------------------------------
;	writeStockFileTitle
;	write the title of the stock file
;----------------------------------------
writeStockFileTitle PROC

	MOV SI, 0
	LEA SI, writeBuffer

	LEA DI, stockFileTitle	;point the addres of title to DI
	CALL appendString		;call to write the title to buffer
	
	LEA DI, newLine
	call appendString		;call to write the newline to buffer
	
	MOV AL, '$'
	MOV [SI], AL
	INC SI
	
	CALL writeToFile		;call to write the buffer to stockFile
	
	RET
	
writeStockFileTitle ENDP

;----------------------------------------
;	appendStringToBuffer
;	append the data to the write buffer
;----------------------------------------
appendToBuffer PROC

	PUSH SI 				;This SI hold the readFilePointer
	
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
	
	MOV bufferIndex, SI
	
	CALL writeToFile
	
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
	JE doneAppending	;append until delimeter
	
	MOV [SI], AL
	INC DI
	INC SI
	JMP appendString

	doneAppending:
		RET
		
appendString ENDP


;---------------------------------
;	writeToFile
;	write the buffer to the file
;---------------------------------
writeToFile PROC

	LEA DI, writeBuffer
	CALL countBufferLength	;count the buffer length

    MOV AH, 40H				;write the buffer to the file
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
	
writeToFile ENDP


;-----------------------------------------------
;	countBufferLength
;	count the length of the buffer for writting
;-----------------------------------------------
countBufferLength PROC

	resetBX
	resetAX
	
	countLengthOfBuffer:
		;--Count the length of string by comparing 0DH (next line)
		
		MOV AL, [DI]
		
		CMP AL, 0DH;
		JE endReadBufferLength
		
		INC DI
		INC BX
		
		JMP countLengthOfBuffer
		
	endReadBufferLength:
		ADD BX, 2
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
	; Delete the phone file and create again
	MOV AH, 41h        	  ; DOS function to delete a file
	LEA DX, stockFileName ; Address of file to delete (Phone.txt)
	INT 21H            	  ; Call DOS

	MOV AH, 3Ch         ; DOS function to create a file
	LEA DX, stockFileName ; Address of the file name
	MOV CX, 0           ; File attributes (0 = normal file)
	INT 21H             ; Call DOS interrupt

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