.DATA
	continueStockOutPrompt DB "1. Search stock for stock out", 0DH, 0AH, "2. Exit Search", 0DH, 0AH, "Please Enter Your Choice : $"
	promptStockOutQty DB "Enter Stock Out Quantity: $"
	stockOutSuccessMsg DB "Stock out sucessfull$"
	notEnoughStock DB "Stock No Enough!$"
.CODE
stockOutMenu PROC
	MOV AH, 09H
	LEA DX, continueStockOutPrompt
	INT 21H
	
	MOV AH, 01H
	INT 21H
	SUB AL,30H
	
	print_NewLine
		
	CMP AL, 1
	JB invalidOfChoice
	CMP AL, 2
	JA invalidOfChoice
	
	MOV choice, AL
	
	clnScr
	
	RET
	
	invalidOfChoice:
	MOV AH, 09H
	LEA DX, invalidInputMsg
	INT 21H
	
	print_NewLine
	
	pause
	clnScr
	
	JMP stockOutmenu	
stockOutMenu ENDP

compareAndStockOut PROC
	CALL openWriteTempFile
	CALL writeStockFileTitle
	
	CALL openReadStockFile
	CALL jumpToNextLine
	
	CALL searchPhoneStock
	
	searchStockOut:
		MOV AL, [SI]
		CMP AX, 0
		JE checkSearchStockOutResult
		
		CALL clearStockVariableBuffer
		CALL readStockDetails
		CALL compareExactString
		CMP searchCheck, 1
		JE enterStockOutQty
		
		CALL appendToBuffer
	
		JMP searchStockOut
	
	endStockOut:
		pause
		clnScr
		RET
	
	enterStockOutQty:
		MOV AH, 09H
		LEA DX, promptStockOutQty
		INT 21H
	
		CALL inputQuantity
		
		CMP digitValidate, 0
		JE invalidStockOutQtyInput

		LEA DI, numberInputed
		CALL stringToNumber
		
		CMP AX, WORD PTR Stock.phoneQty
		JA stockNoEnough
		
		ADD WORD PTR Stock.totalStockOutQty, AX
		SUB WORD PTR Stock.phoneQty, AX
		
		MOV AH, 09H
		LEA DX, stockOutSuccessMsg
		INT 21H
		
		print_NewLine
		
		CALL appendToBuffer
		
		JMP searchStockOut
		
	checkSearchStockOutResult:
		CMP numStockFound, 0
		JE noStockOutMsg
		print_NewLine
		JMP endStockOut
	
	InvalidStockOutQtyInput:
		MOV AH, 09H
		LEA DX, invalidInputMsg
		INT 21H
    
		print_NewLine
		
		pause
		clnScr

		JMP enterStockOutQty
		
	stockNoEnough:
		MOV AH, 09H
		LEA DX, notEnoughStock
		INT 21H
		
		print_NewLine
		
		pause
		clnScr
		
		JMP enterStockOutQty
	
	noStockOutMsg:
		MOV AH, 09H
		LEA DX, searchFailMsg
		INT 21H
		
		print_NewLine
		
		JMP endStockOut

compareAndStockOut ENDP