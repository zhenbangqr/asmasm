.DATA
	divider DB "==========================================================", 0DH, 0AH, "$"
	reportTitle DB "==                        Report                        ==", 0DH, 0AH, "$"
	reportChoice DB "1. Monthly Stock In Report", 0DH, 0AH, "2. Monthly Stock Out Report", 0DH, 0AH, "3. Inventory Status Report", 0DH, 0AH, "4. Return", 0DH, 0AH, "$"
	promptReportChoice DB "Enter a digit(1-4): $"
	
	stockInTitle DB "==               Monthly Stock In Report                ==", 0DH, 0AH, "$"
	stockOutTitle DB "==               Monthly Stock Out Report               ==", 0DH, 0AH, "$"
	invStatTitle DB "==               Inventory Status Report                ==", 0DH, 0AH, "$"
	phoneDetailsTitle DB "Phone Name          |RAM     |ROM     |Color   |Quantity", 0DH, 0AH, "$"
	phoneDetailsTitleDivider DB "====================|========|========|========|==========", 0DH, 0AH, "$"
	
	digitCount DB ?
	
	; Example for phone details
	phoneNameee DB "Tiong Fatt Siong    $"
	phoneRammm DB "8GB     $"
	phoneRommm DB "128GB   $"
	phoneColorrr DB "White   $"
	stockInQtyyy DW 10
	
	; Divider
	tenThousand DW 10000
	thousand DW 1000
	hundred DW 100
	ten DW 10
.CODE

reportMenu PROC
	
	reportList:
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
		
		MOV choice,AL
		
		print_newLine
		
		CMP choice,1
		JE stockInReport
		CMP choice,2
		JE stockOutReport
		CMP choice,3
		JE invStatReport
		
		RET
	
	reportInvalidChoice:
	
		clnScr
				
		print_newLine
		
		MOV AH,09H
		LEA DX,invalidInputMsg
		INT 21H
	
		print_newLine
	
		JMP reportList

	stockInReport:
	
		clnScr
		CALL stockInReports
		pause
		clnScr
	
		JMP reportList

	stockOutReport:
	
		clnScr
		CALL stockOutReports
		pause
		clnScr
	
		JMP reportList

	invStatReport:
	
		clnScr
		JMP jmpSkip
		pause
		clnScr
	
		JMP reportList
	
reportMenu ENDP

; Display current stock status
invStatReports PROC

	displayDivider
	
	MOV AH,09H
	LEA DX,invStatTitle
	INT 21H
	
	displayDivider
	
	MOV AH,09H
	LEA DX,phoneDetailsTitle
	INT 21H
	
	MOV AH,09H
	LEA DX,phoneDetailsTitleDivider
	INT 21H
	
	; Here need to do loop to display all phone details
	CALL displayDetails
	
	displayDivider
	
	RET
	
invStatReports ENDP

; Display stock in
stockInReports PROC

	displayDivider
	
	MOV AH,09H
	LEA DX,stockInTitle
	INT 21H
	
	displayDivider
	
	MOV AH,09H
	LEA DX,phoneDetailsTitle
	INT 21H
	
	MOV AH,09H
	LEA DX,phoneDetailsTitleDivider
	INT 21H
	
	; Here need to do loop to display all phone details
	CALL displayDetails
	
	displayDivider
	
	RET
	
stockInReports ENDP

; Display stock out
stockOutReports PROC

	displayDivider
	
	MOV AH,09H
	LEA DX,stockOutTitle
	INT 21H
	
	displayDivider
	
	MOV AH,09H
	LEA DX,phoneDetailsTitle
	INT 21H
	
	MOV AH,09H
	LEA DX,phoneDetailsTitleDivider
	INT 21H
	
	; Here need to do loop to display all phone details
	CALL displayDetails
	
	displayDivider
	
	RET
	
stockOutReports ENDP

displayDetails PROC
	
	MOV AH,09H
	LEA DX,phoneNameee
	INT 21H
	
	delimiter
	
	MOV AH,09H
	LEA DX,phoneRammm
	INT 21H
	
	delimiter
	
	MOV AH,09H
	LEA DX,phoneRommm
	INT 21H
	
	delimiter
	
	MOV AH,09H
	LEA DX,phoneColorrr
	INT 21H
	
	delimiter
	
	MOV AH,02H
	MOV AX,stockInQtyyy
	CALL displayNumber
	
	print_newLine
	
	RET
	
displayDetails ENDP