.DATA
	continueStockInPrompt DB "1. Search stock for stock in", 0DH, 0AH, "2. Exit Search", 0DH, 0AH, "Please Enter Your Choice : $"
	promptStockInQty DB "Enter Stock In Quantity: $"
	stockInSuccessMsg DB "Stock in sucessfull$"
	
		inputNumber LABEL BYTE
		numberLength DB 3
		numberActNum DB ?
		numberInputed DB 3 DUP("$")
	
	quantity DD ?
.CODE
stockInMenu PROC
	print_NewLine
	
	MOV AH, 09H
	LEA DX, continueStockInPrompt
	INT 21H
	
	MOV AH, 01H
	INT 21H
	SUB AL,30H
	
	print_NewLine
		
	CMP AL, 1
	JB invalidOfInput
	CMP AL, 2
	JA invalidOfInput
	
	MOV choice, AL
	
	clnScr
	
	RET
	
	invalidOfInput:
	MOV AH, 09H
	LEA DX, invalidInputMsg
	INT 21H
	
	print_NewLine
	
	pause
	clnScr
	
	JMP stockInMenu	
stockInMenu ENDP

compareAndStockIn PROC
	CALL openWriteStockFile
	CALL writeStockFileTitle
	
	CALL openReadStockFile
	CALL jumpToNextLine
	
	CALL searchPhoneStock
	
	searchStockIn:
		CALL clearStockVariableBuffer
		CALL readStockDetails
		CALL compareExactString
		CMP searchCheck, 1
		JE enterStockInQty
		
		CALL appendToBuffer
		
		MOV AL, [SI]
		CMP AX, 0
		JE endStockIn
	
		JMP searchStockIn
		
	enterStockInQty:
		MOV AH, 09H
		LEA DX, promptStockInQty
		INT 21H
	
		CALL inputQuantity
		
		CMP digitValidate, 0
		JE invalidQtyInput

		LEA DI, numberInputed
		CALL stringToNumber
		
		ADD WORD PTR Stock.totalStockInQty, AX
		ADD WORD PTR Stock.phoneQty, AX
		
		MOV AH, 09H
		LEA DX, stockInSuccessMsg
		INT 21H
		
		print_NewLine
		
		CALL appendToBuffer
		
		JMP searchStockIn
	
	endStockIn:
		pause
		clnScr
		RET
	
	InvalidQtyInput:
		MOV AH, 09H
		LEA DX, invalidInputMsg
		INT 21H
    
		MOV AH, 09H
		LEA DX, newLine
		INT 21H
		
		pause
		clnScr

		JMP enterStockInQty
	
	checkSearchResult:
		CMP numStockFound, 0
		JE noStockInMsg
		print_NewLine
		JMP endStockIn
	
	noStockInMsg:
		MOV AH, 09H
		LEA DX, searchFailMsg
		INT 21H
		
		print_NewLine
		
		JMP endStockIn

compareAndStockIn ENDP