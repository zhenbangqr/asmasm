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
		CALL displayDivider
		
		MOV AH,09H
		LEA DX,reportTitle
		INT 21H
		
		CALL displayDivider
		
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
		
		MOV AH,09H
		LEA DX,newLine
		INT 21H
		
		CMP choice,1
		JE stockInReport
		CMP choice,2
		JE stockOutReport
		CMP choice,3
		JE invStatReport
		
		RET
	
	reportInvalidChoice:
	
		CALL clear_Screen
				
		MOV AH,09H
		LEA DX,newLine
		INT 21H
		
		MOV AH,09H
		LEA DX,invalidInputMsg
		INT 21H
	
		MOV AH,09H
		LEA DX,newLine
		INT 21H
	
		JMP reportList

	stockInReport:
	
		CALL clear_Screen
		CALL stockInReports
		CALL pause
		CALL clear_Screen
	
		JMP reportList

	stockOutReport:
	
		CALL clear_Screen
		CALL stockOutReports
		CALL pause
		CALL clear_Screen
	
		JMP reportList

	invStatReport:
	
		CALL clear_Screen
		CALL invStatReports
		CALL pause
		CALL clear_Screen
	
		JMP reportList
	
reportMenu ENDP

; Display stock in
stockInReports PROC

	CALL displayDivider
	
	MOV AH,09H
	LEA DX,stockInTitle
	INT 21H
	
	CALL displayDivider
	
	MOV AH,09H
	LEA DX,phoneDetailsTitle
	INT 21H
	
	MOV AH,09H
	LEA DX,phoneDetailsTitleDivider
	INT 21H
	
	; Here need to do loop to display all phone details
	CALL displayDetails
	
	CALL displayDivider
	
	RET
	
stockInReports ENDP

; Display stock out
stockOutReports PROC

	CALL displayDivider
	
	MOV AH,09H
	LEA DX,stockOutTitle
	INT 21H
	
	CALL displayDivider
	
	MOV AH,09H
	LEA DX,phoneDetailsTitle
	INT 21H
	
	MOV AH,09H
	LEA DX,phoneDetailsTitleDivider
	INT 21H
	
	; Here need to do loop to display all phone details
	CALL displayDetails
	
	CALL displayDivider
	
	RET
	
stockOutReports ENDP

; Display current stock status
invStatReports PROC

	CALL displayDivider
	
	MOV AH,09H
	LEA DX,invStatTitle
	INT 21H
	
	CALL displayDivider
	
	MOV AH,09H
	LEA DX,phoneDetailsTitle
	INT 21H
	
	MOV AH,09H
	LEA DX,phoneDetailsTitleDivider
	INT 21H
	
	; Here need to do loop to display all phone details
	CALL displayDetails
	
	CALL displayDivider
	
	RET
	
invStatReports ENDP

displayDetails PROC
	
	MOV AH,09H
	LEA DX,phoneNameee
	INT 21H
	
	CALL delimiter
	
	MOV AH,09H
	LEA DX,phoneRammm
	INT 21H
	
	CALL delimiter
	
	MOV AH,09H
	LEA DX,phoneRommm
	INT 21H
	
	CALL delimiter
	
	MOV AH,09H
	LEA DX,phoneColorrr
	INT 21H
	
	CALL delimiter
	
	MOV AH,02H
	MOV AX,stockInQtyyy
	CALL displayNumber
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	RET
	
displayDetails ENDP



displayDivider PROC
	
	MOV AH,09H
	LEA DX,divider
	INT 21H
	
	RET
	
displayDivider ENDP

delimiter PROC

	MOV AH,02H
	MOV DL,"|"
	INT 21H
	RET
	
delimiter ENDP

pause PROC

    MOV AH,01H
    INT 21H
    RET
	
pause ENDP