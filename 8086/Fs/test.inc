.data  
	writeBuffer DW 1024 DUP(?)    ; Buffer to store file content
	bufferLength DW ?
.code

appendStringToBuffer PROC
	PUSH SI //This SI hold the readFilePointer
	
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
	
	LEA DI, newLine
	call appendString
	MOV BX, SI
	
	POP SI
	
	CALL writeStockFile
	RET
appendStringToBuffer ENDP





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





writeStockFile PROC
	LEA DI, writeBuffer
	CALL countBufferLength

    MOV AH, 40H
	MOV BX, fileHandle
	LEA DX, writeBuffer
	MOV CX, bufferLength
	
	INT 21H
	
	RET
	
writeStockFile ENDP

countBufferLength PROC
	MOV BX, 0
	MOV AX, 0 
	countLengthOfBuffer:
			;--Count the length of string by comparing '$' symbol
			
			MOV AL, [DI]
			
			CMP AX, 0
			JE endReadBufferLength
			INC DI
			INC BL
			JMP countLengthOfBuffer
			
	endReadBufferLength:
		MOV bufferLength, BX
		RET
countBufferLength ENDP
