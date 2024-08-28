.DATA

	reportTitle DB "========================================", 0DH, 0AH, "==               Report               ==", 0DH, 0AH, "========================================", 0DH, 0AH, "$"
	reportChoice DB "1. Monthly Sales Report", 0DH, 0AH, "2. Monthly Stock In Report", 0DH, 0AH, "3. Monthly Stock Out Report", 0DH, 0AH, "4. Inventory Status Report", 0DH, 0AH, "5. Return", 0DH, 0AH, "$"
	
	salesReportTitle DB "========================================", 0DH, 0AH, "==        Monthly Sales Report        ==", 0DH, 0AH, "========================================", 0DH, 0AH, "$"
	iPhoneSales DB "iPhone Sales", 0DH, 0AH, "$"
	iPhone15 DB "iPhone15 : $"
    iPhone14 DB "iPhone14 : $"
    iPhone13 DB "iPhone13 : $"
    iPhone12 DB "iPhone12 : $"
	units DB "(units)$"
	commisionPrompt DB 0DH, 0AH, "Commision: RM", "$"
	
	
	iPhone15Price DW 4400
	iPhone15Sales DW 45
	iPhone14Price DW 3800
	iPhone14Sales DW 52
	iPhone13Price DW 3300
	iPhone13Sales DW 60
	iPhone12Price DW 2900
	iPhone12Sales DW 68
	
	commisionRate DW 5
	thousand DW 1000
	hundred DW 100
	ten DW 10
.CODE

reportMenu PROC
	
reportList:
	MOV AH,09H
	LEA DX,reportTitle
	INT 21H
	
	MOV AH,09H
	LEA DX,reportChoice
	INT 21H
	
	MOV AH,09H
	LEA DX,promptChoice
	INT 21H
	
	MOV AH,01H
	INT 21H
	SUB AL,30H
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	CMP AL,1
	JB reportInvalidChoice
	CMP AL,5
	JA reportInvalidChoice
	
	MOV choice,AL
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	CMP choice, 1
    JE salesReport
    CMP choice, 2
    JE stockInReport
    CMP choice, 3
    JE stockOutReport
    CMP choice, 4
    JE statusReport
    CMP choice, 5
    RET
	
reportInvalidChoice:
	MOV AH,09H
	LEA DX,invalidInputMsg
	INT 21H
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	JMP reportList

salesReport:
	
	CALL generateReports
	
	JMP reportList

stockInReport:
	
	
	
	JMP reportList

stockOutReport:
	
	
	
	JMP reportList

statusReport:
	
	
	
	JMP reportList
	
	
reportMenu ENDP

generateReports PROC
	
	MOV AH,09H
	LEA DX,salesReportTitle
	INT 21H
	
	MOV AH,09H
	LEA DX,iPhoneSales
	INT 21H
	
	MOV AH,09H
	LEA DX,iPhone15
	INT 21H
	MOV AX,iPhone15Sales
	CALL displaySales

	
	MOV AH,09H
	LEA DX,units
	INT 21H
	
	MOV AH,09H
	LEA DX,commisionPrompt
	INT 21H
	MOV AX,iPhone15Price
	MOV BX,iPhone15Sales
	CALL calCommision
	CALL displaySales
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	MOV AH,09H
	LEA DX,iPhone14
	INT 21H
	MOV AX,iPhone14Sales
	CALL displaySales

	
	MOV AH,09H
	LEA DX,units
	INT 21H
	
	MOV AH,09H
	LEA DX,commisionPrompt
	INT 21H
	MOV AX,iPhone14Price
	MOV BX,iPhone14Sales
	CALL calCommision
	CALL displaySales
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	MOV AH,09H
	LEA DX,iPhone13
	INT 21H
	MOV AX,iPhone13Sales
	CALL displaySales

	
	MOV AH,09H
	LEA DX,units
	INT 21H
	
	MOV AH,09H
	LEA DX,commisionPrompt
	INT 21H
	MOV AX,iPhone13Price
	MOV BX,iPhone13Sales
	CALL calCommision
	CALL displaySales
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	MOV AH,09H
	LEA DX,iPhone12
	INT 21H
	MOV AX,iPhone12Sales
	CALL displaySales

	
	MOV AH,09H
	LEA DX,units
	INT 21H
	
	MOV AH,09H
	LEA DX,commisionPrompt
	INT 21H
	MOV AX,iPhone12Price
	MOV BX,iPhone12Sales
	CALL calCommision
	CALL displaySales
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	RET
generateReports ENDP


displaySales PROC
	; Divide the value by 1000 to get the thousands digit
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
	
	RET

displaySales ENDP

calCommision PROC
	
	; Multiply the sales by commision rate and times with the sales amount
	MOV CX,commisionRate
	MUL CX
	DIV hundred
	MUL BX
	
	RET
	
calCommision ENDP