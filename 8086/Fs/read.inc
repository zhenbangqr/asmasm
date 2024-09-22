.DATA
    stockFileName DB 'PHONE.TXT', 0
    staffFileName DB 'staff.txt', 0
    readBuffer DW 1024 DUP(?)    ; Buffer to store file content
    fileHandle DW ?         ; To store the file handle
	fileErrorMsg DB "Error Occured!$"
	
.CODE

;--------------------------------------------
;	openReadStockFile
;	open the stock file(Phone.txt) for read
;--------------------------------------------
openReadStaffFile PROC

	MOV AH, 3DH					;Open file
	MOV AL, 0					;Open for reading
	MOV DX, OFFSET staffFileName
	INT 21H
	mov fileHandle, AX			;Store File Handle
	
	MOV BX, fileHandle			;Use the fileHandle
	LEA DX, readBuffer			;Address for buffer
	MOV CX, 1024				;Read 1024 bytes
	MOV AH, 3FH					;Read File
	INT 21H
	
	MOV SI, 0
	LEA SI, readBuffer
	
	RET
	
openReadStaffFile ENDP

;--------------------------------------------
;	openReadStockFile
;	open the stock file(Phone.txt) for read
;--------------------------------------------
openReadStockFile PROC

	MOV AH, 3DH					;Open file
	MOV AL, 0					;Open for reading
	MOV DX, OFFSET stockFileName
	INT 21H
	mov fileHandle, AX			;Store File Handle
	
	MOV BX, fileHandle			;Use the fileHandle
	LEA DX, readBuffer			;Address for buffer
	MOV CX, 1024				;Read 1024 bytes
	MOV AH, 3FH					;Read File
	INT 21H
	
	LEA SI, readBuffer			;point si to the offset address of readBuffer
	
	RET
	
openReadStockFile ENDP

;------------------------------------------------------
;	closeReadFile
;	close the stock file(Phone.txt)
;------------------------------------------------------
closeReadFile PROC
	
	MOV BX, fileHandle
    MOV AH, 3EH             ; DOS function: close file
    INT 21H
	
	RET
	
closeReadFile ENDP
	
;-------------------------------------------------------
;	readStringFromFile
;	Read a string from file to a string with fill space
;-------------------------------------------------------
readStringFromFile PROC

	continueReadString:
		MOV AL, [SI]
		CMP AL, ';'			;read until delimiter
		JE endReadString
	
		MOV [DI], AL 
		INC SI
		INC DI
		JMP continueReadString
			
	endReadString:
		MOV AL, '$'
		MOV [DI], AL
		INC SI
		RET
		
readStringFromFile ENDP

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
		CMP BL, '.'
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
	CALL readStringFromFile

	LEA DI, Stock.phoneRAM
	CALL readStringFromFile
	
	LEA DI, Stock.phoneROM
	CALL readStringFromFile
	
	LEA DI, Stock.phoneColor
	CALL readStringFromFile
	
	CALL readNumber
	MOV WORD PTR Stock.phonePrice, AX
	MOV WORD PTR Stock.phonePrice + 2, DX
	
	CALL readNumber
	MOV WORD PTR Stock.phonePriceFP, AX
	MOV WORD PTR Stock.phonePriceFP + 2, DX
	
	CALL readNumber
	MOV WORD PTR Stock.phoneQty, AX
	MOV WORD PTR Stock.phoneQty + 2, DX
	
	CALL readNumber
	MOV WORD PTR Stock.totalStockInQty, AX
	MOV WORD PTR Stock.totalStockInQty + 2, DX
	
	CALL readNumber
	MOV WORD PTR Stock.totalStockOutQty, AX
	MOV WORD PTR Stock.totalStockOutQty + 2, DX

	CALL jumpToNextLine
	
	RET
	
readStockDetails ENDP

;-------------------------------------------------
;	readStaffDetails
;	read the staff id and password from the file
;-------------------------------------------------
readStaffDetails PROC

	LEA DI, Staff.userID
	resetCX
	CALL readStringFromFile
	
	LEA DI, Staff.userPw
	resetCX
	CALL readStringFromFile
	
	CALL jumpToNextLine
	
	RET
	
readStaffDetails ENDP

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

;-------------------------------------------------
;	calculateStringLength
;	calculate the length of the string
;-------------------------------------------------
calculateStringLength PROC

	resetBX
	
	countStringLength:
			;--Count the length of string by comparing '$' symbol
			
			MOV AL, [DI]
			
			CMP AL, '$'
			JE endReadStringLength
			INC DI
			INC BL
			JMP countStringLength
			
	endReadStringLength:
		RET
		
calculateStringLength ENDP

;----------------------------------------------------------
;	inputQuantity
;	input and check whether the input is all digit or not
;----------------------------------------------------------
inputQuantity PROC

	MOV AH, 0AH
	LEA DX, inputNumber ;input
	INT 21H
	
	print_NewLine
	
	CALL checkIsDigit	;go to check the input is digit or not
	
	RET
	
inputQuantity ENDP

;-----------------------------------------------------------
;	checkIsDigit
;	checkWhether a string enter is all digit
;-----------------------------------------------------------
checkIsDigit PROC

	MOV DI, 2
	
	CMP inputNumber[DI], 0DH
	JE notDigit
	
	cmpIsDigit:
		MOV AL, inputNumber[DI]
		
		CMP AL, 0DH
		JE isDigit
		CMP AL, '$'
		JE isDigit
		
		CMP AL, '0'
		JB notDigit
		CMP AL, '9'
		JA notDigit
	
		INC DI
	
		JMP cmpIsDigit
	
	notDigit:
		MOV digitValidate, 0
		RET

	isDigit:
		MOV digitValidate, 1
		
	RET
	
checkIsDigit ENDP

;----------------------------------------
;	stringToNumber
;	convert string become number
;----------------------------------------
stringToNumber PROC

	resetAX
	resetBX
	resetCX
	resetDX
	
	MOV BL, [DI]
	SUB BL, 30H
	
	MOV AX, BX
	INC DI
	
	;convert the string to number
	continueConvertNumber:
		MOV BL, [DI]
		CMP BL,	0DH
		JE endConvertNumber
		CMP BL,	'$'
		JE endConvertNumber
		
		MUL ten16Bits
		SUB BL, 30H
		ADD AX, BX
		
		INC DI
		
		JMP continueConvertNumber
	
	endConvertNumber:
		RET
		
stringToNumber ENDP

;-------------------------------------
;	clearReadBuffer
;	clear the buffer
;-------------------------------------
clearReadBuffer PROC

	MOV CX, 20
    MOV AL, 0             ; Value to fill the buffer (zero in this case)
	
	clearBufferLoop:
		MOV [DI], AL          ; Store value AL at address DI
		INC DI     	          ; Move to the next byte
    LOOP clearBufferLoop
	
	RET
	
clearReadBuffer ENDP

;--------------------------------------------
;	clearStockVariableBuffer
;	clear the buffer for all stock variable
;--------------------------------------------
clearStockVariableBuffer PROC

	LEA DI, Stock.phoneName
	CALL clearReadBuffer
	
	LEA DI, Stock.phoneRAM
	CALL clearReadBuffer
	
	LEA DI, Stock.phoneROM
	CALL clearReadBuffer
	
	LEA DI, Stock.phoneColor
	CALL clearReadBuffer

	resetAX
	MOV WORD PTR Stock.phonePrice, AX
	MOV WORD PTR Stock.phonePrice + 2, AX

	MOV WORD PTR Stock.phonePriceFP, AX
	MOV WORD PTR Stock.phonePriceFP + 2, AX

	MOV WORD PTR Stock.phoneQty, AX
	MOV WORD PTR Stock.phoneQty + 2, AX

	MOV WORD PTR Stock.totalStockInQty, AX
	MOV WORD PTR Stock.totalStockInQty + 2, AX

	MOV WORD PTR Stock.totalStockOutQty, AX
	MOV WORD PTR Stock.totalStockOutQty + 2, AX
	
	RET
	
clearStockVariableBuffer ENDP

;----------------------------------------------------
;	clearReadFileBuffer
;	clear the read buffer
;----------------------------------------------------
clearReadFileBuffer PROC

	;clear the Buffer
	MOV CX, 1024  ; Buffer size
	LEA DI, readBuffer
	MOV AL, 0
	
	clearReadBufferLoop:
		MOV [DI], AL
		INC DI
		LOOP clearReadBufferLoop
		
	RET
	
clearReadFileBuffer ENDP