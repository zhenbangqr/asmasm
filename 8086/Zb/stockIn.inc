.DATA
	stockInMenuTitle DB "========================================", 0DH, 0AH, "==            Stock In Menu           ==", 0DH, 0AH, "========================================", 0DH, 0AH, "$"
	enterStockInQtyTitle DB "========================================", 0DH, 0AH, "==       Enter Stock In Quantity      ==", 0DH, 0AH, "========================================", 0DH, 0AH, "$"
	continueStockInPrompt DB "1. Search stock for stock in", 0DH, 0AH, "2. Exit Search", 0DH, 0AH, "Please Enter Your Choice : $"
	promptStockInQty DB "Enter Stock In Quantity: $"
	stockInSuccessMsg DB "Stock in sucessfull$"
	
	inputNumber LABEL BYTE
	numberLength DB 3
	numberActNum DB ?
	numberInputed DB 3 DUP("$")
	
.CODE

stockInMenu PROC

	MOV AH, 09H
	LEA DX, stockInMenuTitle
	INT 21H
	
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

	MOV AH, 09H
	LEA DX, searchTitle
	INT 21H
		
	CALL openWriteTempFile
	CALL writeStockFileTitle
	
	CALL openReadStockFile
	CALL jumpToNextLine
	
	CALL searchPhoneStock
	
	searchStockIn:
		MOV AL, [SI]
		CMP AX, 0
		JE checkSearchStockInResult
		
		CALL clearStockVariableBuffer
		CALL readStockDetails
		CALL compareExactString
		CMP searchCheck, 1
		JE enterStockInQty
		
		CALL appendToBuffer
	
		JMP searchStockIn

	endStockIn:
		pause
		clnScr
		RET	
		
	noStockInMsg:
		MOV AH, 09H
		LEA DX, searchFailMsg
		INT 21H
		
		print_NewLine
		
		JMP endStockIn
	
	checkSearchStockInResult:
		CMP numStockFound, 0
		JE noStockInMsg
		print_NewLine
		JMP endStockIn
		
	enterStockInQty:
		clnScr
		MOV AH, 09H
		LEA DX, enterStockInQtyTitle
		INT 21H
		
		MOV AH, 09H
		LEA DX, promptStockInQty
		INT 21H
	
		CALL inputQuantity
		
		CMP digitValidate, 0
		JE invalidStockInQtyInput

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
	
	invalidStockInQtyInput:
		MOV AH, 09H
		LEA DX, invalidInputMsg
		INT 21H
    
		print_NewLine
		pause
		clnScr

		JMP enterStockInQty
		
compareAndStockIn ENDP