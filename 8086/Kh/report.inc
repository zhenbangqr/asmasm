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
	phoneName DB "Tiong Fatt Siong    $"
	phoneRam DB "8GB     $"
	phoneRom DB "128GB   $"
	phoneColor DB "White   $"
	stockInQty DW 10
	
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
	LEA DX,phoneName
	INT 21H
	
	CALL delimiter
	
	MOV AH,09H
	LEA DX,phoneRam
	INT 21H
	
	CALL delimiter
	
	MOV AH,09H
	LEA DX,phoneRom
	INT 21H
	
	CALL delimiter
	
	MOV AH,09H
	LEA DX,phoneColor
	INT 21H
	
	CALL delimiter
	
	MOV AH,02H
	MOV AX,stockInQty
	CALL displayNumber
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	RET
	
displayDetails ENDP

displayNumber PROC

	MOV BX,AX
	MOV CX,0

	countDigit:
		MOV DX,0
		DIV ten ; Divide AX by 10
		INC CX
		CMP AX,0
		JNE countDigit

	MOV AL,CL

    CMP AL, 1
    JE display1Digit
    CMP AL, 3
    JLE display23Digit
	CMP AL, 5
    JLE display45Digit

	; Code to display a single digit
	display1Digit:
		
		MOV AX,BX
		MOV AH,02H
		MOV DL,AL
		ADD DL,30H
		INT 21H
		
		JMP doneDisplay
	
	; Code to display a two- and three-digit number
	display23Digit:
		MOV AX,BX
		CMP AX,99
		JLE display2Digits
		
		XOR DX,DX
		DIV hundred
		MOV BX,AX
		MOV CX,DX
		
		MOV AH,02H
		MOV DL,BL
		ADD DL,30H
		INT 21H
		
		MOV AX,CX
		
		display2Digits:
			XOR DX,DX
			DIV ten
			MOV BX,AX
			MOV CX,DX
			
			; Display the tens digit
			MOV AH,02H
			MOV DL,BL
			ADD DL,30H
			INT 21H
			
			; Display the units digit
			MOV DL,CL
			ADD DL,30H
			INT 21H
		
		JMP doneDisplay
	
	; Code to display a four- and five-digit number
	display45Digit:
		MOV AX,BX
		XOR DX,DX
		DIV tenThousand
		MOV CX,DX
		
		; If AX is 0, it means the number has less than 5 digits
		CMP AX,0
		JE display4Digits
	
		; If the number has 5 digits, mov the ten-thousands digit to BX
		MOV BX,AX
		
		; Display the ten-thousands digit
		MOV AH,02H
		MOV DL,BL
		ADD DL,30H
		INT 21H
		
		display4Digits:
			; Divide the value by 1000 to get the thousands digit
			MOV AX,CX
			XOR DX,DX
			DIV thousand
			MOV BX,AX
			MOV CX,DX
			
			; Display the thousands digit
			MOV AH,02H
			MOV DL,BL
			ADD DL,30H
			INT 21H
		
			; Divide the value by 100 to get the hundreds digit
			MOV AX,CX
			XOR DX,DX
			DIV hundred
			MOV BX,AX
			MOV CX,DX
			
			; Display the hundreds digit
			MOV AH,02H
			MOV DL,BL
			ADD DL,30H
			INT 21H
			
			; Divide the value by 100 to get the tens and units digit
			MOV AX,CX
			XOR DX,DX
			DIV ten
			MOV BX,AX
			MOV CX,DX
			
			; Display the tens digit
			MOV AH,02H
			MOV DL,BL
			ADD DL,30H
			INT 21H
			
			; Display the units digit
			MOV AH,02H
			MOV DL,CL
			ADD DL,30H
			INT 21H
	
	doneDisplay:
		RET
	
displayNumber ENDP

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