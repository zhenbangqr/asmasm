.DATA
    filename DB 'Phone.txt', 0
    buffer DB 128 DUP(?)    ; Buffer to store file content
    fileHandle DW ?         ; To store the file handle
	fileErrorMsg DB "Error Occured!$"
	
	phone STRUC
		phoneName DB 10 DUP(?)
		phoneRAM DB ?
		phoneROM DB ?
		phoneColor DB ?
		phonePrice DB ?
		phonePriceFP DB ?
	phone ENDS
	
	Stock phone<>
.code
readStockFile PROC

	MOV AH, 3DH				;Open file
	MOV AL, 0				;Open for reading
	MOV DX, OFFSET filename	;
	INT 21H
	JC fileError			;Jump when carry flag set(error when open file)
	mov fileHandle, AX		;Store File Handle
	
	MOV BX, fileHandle		;Use the fileHandle
	LEA DX, buffer			;Address for buffer
	MOV CX, 128				;Read 128 bytes
	MOV AH, 3FH				;Read File
	INT 21H
	JC fileError			;Jump when carry flag set(error when open file)
	
	LEA SI, buffer			;SI point to the buffer Start
	
	CALL readPhoneName
	
	MOV AH, 09H
	LEA DX, Stock.phoneName
	INT 21H
	
	CALL jumpToNextLine
	CALL readPhoneName
	
	MOV AH, 09H
	LEA DX, Stock.phoneName
	INT 21H
	
	
	mov bx, fileHandle
    mov ah, 3Eh             ; DOS function: close file
    int 21h
    jc fileError            ; If carry flag is set, an error occurred
	
	RET
	
fileError:
    ; Handle file error
    mov ah, 09h
    lea dx, fileErrorMsg
    int 21h
    jmp exitReadFile
	
exitReadFile:
    ret
readStockFile ENDP	
	


readPhoneName PROC
	LEA	DI, Stock.phoneName	;DI point to the start of phoneName

continueReadName:
	MOV AL, [SI]
	CMP AL, ';'
	JE endReadName
	MOV [DI], AL 
	INC SI
	INC DI
	JMP continueReadName
	
endReadName:
	MOV BYTE PTR [DI], 0
	RET

readPhoneName ENDP
	
	

jumpToNextLine PROC
nextLineCheck:
	MOV AL, [SI]
	CMP AL, 0DH
	JE lineFeedCheck
	INC SI

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

