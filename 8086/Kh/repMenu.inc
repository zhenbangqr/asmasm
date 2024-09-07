.DATA
	reportTitle DB "==                          Report                           ==", 0DH, 0AH, "$"
	reportChoice DB "1. Monthly Stock In Report", 0DH, 0AH, "2. Monthly Stock Out Report", 0DH, 0AH, "3. Inventory Status Report", 0DH, 0AH, "4. Return", 0DH, 0AH, "$"
	promptReportChoice DB "Enter a digit(1-4): $"
	reportDivider DB "===============================================================", 0DH, 0AH, "$"
	invStatusDivider DB "============================================================================", 0DH, 0AH, "$"
	stockInTitle DB "==                  Monthly Stock In Report                  ==", 0DH, 0AH, "$"
	stockOutTitle DB "==                  Monthly Stock Out Report                 ==", 0DH, 0AH, "$"
	invStatTitle DB "==                        Inventory Status Report                         ==", 0DH, 0AH, "$"
	
.CODE

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
		CMP AL, 4
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
		JE stockInReport
		CMP choice, 2
		JE stockOutReport
		CMP choice, 3
		JE invStatReport
		
		clnScr
		JMP rptMenuEnd
	
	stockInReport:
		rptDivider
		MOV AH, 09H
		LEA DX, stockInTitle
		INT 21H
		rptDivider
		CALL stockReportDetails
		rptDivider
		pause

		JMP rptList

	stockOutReport:
		rptDivider
		MOV AH, 09H
		LEA DX, stockOutTitle
		INT 21H
		rptDivider
		CALL stockReportDetails
		rptDivider
		pause
	
		JMP rptList
	
	invStatReport:
		invDivider
		MOV AH, 09H
		LEA DX, invStatTitle
		INT 21H
		invDivider
		CALL stockReportDetails
		invDivider
		pause
		
		JMP rptList
	
	rptMenuEnd:
		RET
	
reportMenu ENDP