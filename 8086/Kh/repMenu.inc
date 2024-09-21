.DATA
	reportDivider DB "===============================================================", 0DH, 0AH, "$"
	reportTitle DB "==                          Report                           ==", 0DH, 0AH, "$"
	reportChoice DB "1. Current Inventory Stock", 0DH, 0AH, "2. Stock In Report", 0DH, 0AH, "3. Stock Out Report", 0DH, 0AH, "4. Low Stock Report", 0DH, 0AH, "5. Phone Profit Report", 0DH, 0AH, "6. Return", 0DH, 0AH, "$"
	promptReportChoice DB "Enter a digit(1-6): $"
	currStockPhoneProfitDivider DB "============================================================================", 0DH, 0AH, "$"
	currStockTitle DB "==                        Current Inventory Stock                         ==", 0DH, 0AH, "$"
	stockInTitle DB "==                      Stock In Report                      ==", 0DH, 0AH, "$"
	stockOutTitle DB "==                     Stock Out Report                      ==", 0DH, 0AH, "$"
	lowStockTitle DB "==                     Low Stock Report                      ==", 0DH, 0AH, "$"
	phoneProfitTitle DB "==                          Phone Profit Report                           ==", 0DH, 0AH, "$"
	
.CODE
;------------------------------------------------------
;	reportMenu
;	calculate the stock value for each type of phone
;------------------------------------------------------
reportMenu PROC
	
	rptList:
		clnScr
		rptDivider
		
		MOV AH, 09H
		LEA DX, reportTitle
		INT 21H
		
		rptDivider
		
		MOV AH, 09H
		LEA DX, reportChoice
		INT 21H
		
		MOV AH, 09H
		LEA DX, promptReportChoice
		INT 21H
		
		MOV AH, 01H
		INT 21H
		SUB AL, 30H
		
		CMP AL, 1
		JB rptInvalidChoice
		CMP AL, 6
		JA rptInvalidChoice
		
		print_Newline
		MOV choice, AL
		clnScr
		JMP rptCmpChoice
		
	rptInvalidChoice:
		
		print_Newline
		
		MOV AH, 09H
		LEA DX, invalidInputMsg
		INT 21H
		
		print_Newline
		pause
	
		JMP rptList
		
	rptCmpChoice:
		CMP choice, 1
		JE currStockReport
		CMP choice, 2
		JE stockInHeader
		CMP choice, 3
		JE stockOutHeader
		CMP choice, 4
		JE lowStockHeader
		CMP choice, 5
		JE phoneProfitHeader
		
		clnScr
		RET
		
	currStockReport:
		csppDivider
		MOV AH, 09H
		LEA DX, currStockTitle
		INT 21H
		
		JMP currStockPhoneProfitReport
		
	phoneProfitHeader:
		csppDivider
		MOV AH, 09H
		LEA DX, phoneProfitTitle
		INT 21H
		
		JMP currStockPhoneProfitReport
	
	stockInHeader:
		rptDivider
		MOV AH, 09H
		LEA DX, stockInTitle
		INT 21H
		JMP stockInOutLowProfitReport
	
	stockOutHeader:
		rptDivider
		MOV AH, 09H
		LEA DX, stockOutTitle
		INT 21H
		JMP stockInOutLowProfitReport
		
	lowStockHeader:
		rptDivider
		MOV AH, 09H
		LEA DX, lowStockTitle
		INT 21H
		
		JMP stockInOutLowProfitReport
	
	currStockPhoneProfitReport:
		csppDivider
		CALL stockReportDetails
		csppDivider
		
		JMP rptMenuEnd
		
	stockInOutLowProfitReport:
		rptDivider
		CALL stockReportDetails
		rptDivider
		
	
	rptMenuEnd:
		pause
		JMP rptList
		
reportMenu ENDP