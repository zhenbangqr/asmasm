.DATA
    stockFileName DB 'Phone.txt', 0
    staffFileName DB 'staff.txt', 0
    buffer DW 1024 DUP(?)    ; Buffer to store file content
    fileHandle DW ?         ; To store the file handle
	fileErrorMsg DB "Error Occured!$"
	
	
	inputNumber LABEL BYTE
		numberLength DB 3
		numberActNum DB ?
		numberInputed DB 3 DUP("$")
	
	quantity DD ?
.code

;--------------------------------------------
;	openReadStockFile
;	open the stock file(Phone.txt) for read
;--------------------------------------------
openReadStaffFile PROC

	MOV AH, 3DH					;Open file
	MOV AL, 0					;Open for reading
	MOV DX, OFFSET staffFileName
	INT 21H
	JC fileError				;Jump when carry flag set(error when open file)
	mov fileHandle, AX			;Store File Handle
	
	MOV BX, fileHandle			;Use the fileHandle
	LEA DX, buffer				;Address for buffer
	MOV CX, 1024				;Read 1024 bytes
	MOV AH, 3FH					;Read File
	INT 21H
	JC fileError				;Jump when carry flag set(error when open file)
	
	LEA SI, buffer
	
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
	JC fileError				;Jump when carry flag set(error when open file)
	mov fileHandle, AX			;Store File Handle
	
	MOV BX, fileHandle			;Use the fileHandle
	LEA DX, buffer				;Address for buffer
	MOV CX, 1024				;Read 1024 bytes
	MOV AH, 3FH					;Read File
	INT 21H
	JC fileError				;Jump when carry flag set(error when open file)
	
	LEA SI, buffer
	
	RET

openReadStockFile ENDP


;------------------------------------------------------
;	closeStockFile
;	close the stock file(Phone.txt)
;------------------------------------------------------
closeFile PROC
	mov bx, fileHandle
    mov ah, 3Eh             ; DOS function: close file
    int 21h
    jc fileError            ; If carry flag is set, an error occurred
	
	RET
closeFile ENDP

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
    call closeFile
	
;-------------------------------------------------------
;	readStringFromFile
;	Read a string from file to a string with fill space
;-------------------------------------------------------
readStringFromFile PROC

	continueReadString:
		MOV AL, [SI]
		CMP AL, ';'
		JE endReadString
	
		MOV [DI], AL 
		INC SI
		INC DI
		JMP continueReadString
			
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
	CALL readStringFromFile

	LEA DI, Stock.phoneRAM
	CALL readStringFromFile
	
	LEA DI, Stock.phoneROM
	CALL readStringFromFile
	
	LEA DI, Stock.phoneColor
	CALL readStringFromFile
	
	CALL readStockPrice
	
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
	MOV CX, 0
	CALL readStringFromFile
	
	LEA DI, Staff.userPw
	MOV CX, 0
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
	MOV BX, 0
	
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
	LEA DX, inputNumber
	INT 21H
	
	print_NewLine
	
	CALL checkIsDigit
	
	RET
inputQuantity ENDP

;-----------------------------------------------------------
;	checkIsDigit
;	checkWhether a string enter is all digit
;-----------------------------------------------------------
checkIsDigit PROC
	MOV DI, 2
	
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