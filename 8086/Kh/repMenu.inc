.DATA
	reportTitle DB "==                        Report                         ==", 0DH, 0AH, "$"
	reportChoice DB "1. Monthly Stock In Report", 0DH, 0AH, "2. Monthly Stock Out Report", 0DH, 0AH, "3. Inventory Status Report", 0DH, 0AH, "4. Return", 0DH, 0AH, "$"
	promptReportChoice DB "Enter a digit(1-4): $"
	
.CODE

reportMenu PROC
	
	reportList:
		clnScr
		displayDivider
		
		MOV AH,09H
		LEA DX,reportTitle
		INT 21H
		
		displayDivider
		
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
		JMP compareChoice
		
	reportInvalidChoice:
	
		clnScr
		
		print_Newline
		
		MOV AH,09H
		LEA DX,invalidInputMsg
		INT 21H
	
		print_Newline
	
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
	
		clnScr
		CALL stockInReports
		pause
	
		JMP reportList

	stockOutReport:
	
		clnScr
		CALL stockOutReports
		pause
	
		JMP reportList
	
	invStatReport:
	
		clnScr
		CALL invStatReports
		pause
	
		JMP reportList
	
	repMenuEnd:
		RET
	
reportMenu ENDP