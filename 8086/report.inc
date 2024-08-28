.DATA

	reportTitle DB "========================================", 0DH, 0AH, "==               Report               ==", 0DH, 0AH, "========================================", 0DH, 0AH, "$"
	reportChoice DB "1. Monthly Sales Report", 0DH, 0AH, "2. Monthly Stock In Report", 0DH, 0AH, "3. Monthly Stock Out Report", 0DH, 0AH, "4. Inventory Status Report", 0DH, 0AH, "5. Return", 0DH, 0AH, "$"
	
	salesReportTitle DB "========================================", 0DH, 0AH, "==        Monthly Sales Report        ==", 0DH, 0AH, "========================================", 0DH, 0AH, "$"
	Apple DB "Apple  : $"
    Samsung DB "Samsung: $"
    Huawei DB "Huawei : $"
    Xiaomi DB "Xiaomi : $"
	
	appleSales DW 10
	samsungSales DW 100
	huaweiSales DW 100
	xiaomiSales DW 100
	
	hundred DB 100
	ten DB 10
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
	LEA DX,Apple
	INT 21H
	MOV AX,appleSales
	CALL displaySales
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	MOV AH,09H
	LEA DX,Samsung
	INT 21H
	MOV AX,samsungSales
	CALL displaySales
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	MOV AH,09H
	LEA DX,Huawei
	INT 21H
	MOV AX,huaweiSales
	CALL displaySales
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	MOV AH,09H
	LEA DX,Xiaomi
	INT 21H
	MOV AX,xiaomiSales
	CALL displaySales
	
	MOV AH,09H
	LEA DX,newLine
	INT 21H
	
	RET
generateReports ENDP


displaySales PROC
	; Divide the sales value by 100 to get the hundreds digit
	DIV hundred
	MOV BX,AX
	
	; Display the hundreds digit
	MOV AH,02H
	MOV DL,BL
	ADD DL,30H
	INT 21H
	
	; Clear AH and get the remainder in AL
	MOV AH,0H
	MOV AL,BH
	DIV ten
	MOV BX,AX
	
	; Display the tens digit
	MOV AX,0H
	MOV AH,02H
	MOV DL,BL
	ADD DL,30H
	INT 21H
	
	; Display the units digit
	MOV DL,BH
	ADD DL,30H
	INT 21H
	
	RET; Return to the point after CALL

displaySales ENDP