.DATA
	reportTitle DB "==                          Report                           ==", 0DH, 0AH, "$"
	reportChoice DB "1. Monthly Stock In Report", 0DH, 0AH, "2. Monthly Stock Out Report", 0DH, 0AH, "3. Inventory Status Report", 0DH, 0AH, "4. Return", 0DH, 0AH, "$"
	promptReportChoice DB "Enter a digit(1-4): $"
	repDivider DB "===============================================================", 0DH, 0AH, "$"
	stockInTitle DB "==                  Monthly Stock In Report                  ==", 0DH, 0AH, "$"
	stockOutTitle DB "==                  Monthly Stock Out Report                 ==", 0DH, 0AH, "$"
	invStatTitle DB "==                  Inventory Status Report                  ==", 0DH, 0AH, "$"
	highestStockOut DB "                                   Highest Stock Out|$"
	NL DB 0DH, 0AH, "$"
	
.CODE

reportMenu PROC
	
	reportList:
		clnScr
		reportDivider
		
		
		MOV AH,09H
		LEA DX,reportTitle
		INT 21H
		
		reportDivider
		
		MOV AH,09H
		LEA DX,reportChoice
		INT 21H
		
		MOV AH,09H
		LEA DX,promptReportChoice
		INT 21H
		
		MOV AH,01H
		INT 21H
		SUB AL,30H
		
		CMP AL,1
		JB reportInvalidChoice
		CMP AL,4
		JA reportInvalidChoice
		
		print_Newline
		MOV choice,AL
		clnScr
		reportDivider
		JMP compareChoice
		
	reportInvalidChoice:
		
		print_Newline
		
		MOV AH,09H
		LEA DX,invalidInputMsg
		INT 21H
		
		print_Newline
		pause
	
		JMP reportList
		
	compareChoice:
		CMP choice,1
		JE stockInReport
		CMP choice,2
		JE stockOutReport
		CMP choice,3
		JE invStatReport
		
		clnScr
		JMP repMenuEnd
	
	stockInReport:
		
		MOV AH,09H
		LEA DX,stockInTitle
		INT 21H
		CALL stockReportDetails
		reportDivider
		pause

		JMP reportList
		
	jumpSkip:
		JMP invStatReport

	stockOutReport:
	
		MOV AH,09H
		LEA DX,stockOutTitle
		INT 21H
		CALL stockReportDetails
		reportDivider
		pause
	
		JMP reportList
	
	invStatReport:
	
		MOV AH,09H
		LEA DX,invStatTitle
		INT 21H
		CALL stockReportDetails
		reportDivider
		pause
		
		JMP reportList
	
	repMenuEnd:
		RET
	
reportMenu ENDP