.DATA
	stockRptDetails DB "No.|Phone Name     |RAM       |ROM       |Color     |Quantity", 0DH, 0AH, "$"
	stockRptDetailsDivider DB "===|===============|==========|==========|==========|==========", 0DH, 0AH, "$"
	invStatDetails DB "No.|Phone Name     |RAM       |ROM       |Color     |Quantity  |Stock Values", 0DH, 0AH, "$"
	invStatDetailsDivider DB "===|===============|==========|==========|==========|==========|============", 0DH, 0AH, "$"
	stockRptTotal DB "                                               Total|$"
	totalStock DD 0
	stockValue DD 0
	stockValueFP DD 0
	tempStockValue DD 0
	tempStockValueFP DD 0

.CODE

stockReportDetails PROC
	
	CMP choice, 3
	JE displayInvStat
		
	MOV AH, 09H
	LEA DX, stockRptDetails
	INT 21H
	
	MOV AH, 09H
	LEA DX, stockRptDetailsDivider
	INT 21H
	
	JMP detailsNext
	
	displayInvStat:
		MOV AH, 09H
		LEA DX, invStatDetails
		INT 21H
	
		MOV AH, 09H
		LEA DX, invStatDetailsDivider
		INT 21H
	
	detailsNext:
		CALL rptDetails
		print_Newline
	RET
	
stockReportDetails ENDP

rptDetails PROC
	
	CALL openReadStockFile
	CALL jumpToNextLine
	
	rptReadStock:
		CALL clearStockVariableBuffer
		CALL readStockDetails
		INC numStockFound
		CALL sumOfStock
		JMP displayReportDetails
	
	checkEOF:
		MOV AX, [SI]
		CMP AX, 0
		JE reportSummary
		
		JMP rptReadStock
	
	displayReportDetails:
		CALL rptStockDetails
		print_Newline
		
		JMP checkEOF
	
	reportSummary:
		MOV numStockFound, 0
		CALL closeFile
		
		CMP choice, 3
		JE displayInvDivider
		
		rptDivider
		
		JMP displayTotalStock
		
		displayInvDivider:
			invDivider
		
		displayTotalStock:
			MOV AH, 09H
			LEA DX, stockRptTotal
			INT 21H
			
			MOV BX, 10
			MOV AX, WORD PTR totalStock
			MOV DX, WORD PTR totalStock + 2
			CALL displayNumber
			CALL fillSpace
			
			MOV WORD PTR totalStock, 0
			MOV WORD PTR totalStock + 2, 0
		
		CMP choice, 3
		JE displaySumOfStockValue
		
		JMP reportEnd
		
		displaySumOfStockValue:
			delimiter
			CALL sumOfStockValue
		
		reportEnd:
			RET
	
rptDetails ENDP

rptStockDetails PROC
	MOV AX, 0
	MOV AL, numStockFound
	DIV ten8Bits
	
	MOV BX, AX
	MOV AH, 02H
	MOV DL, BL
	ADD DL, 30H
	INT 21H
	
	MOV AH, 02H
	MOV DL, BH
	ADD DL, 30H
	INT 21H
	
	MOV AH, 02H
	MOV DL, '.'
	INT 21H
		
	delimiter
	
	MOV AH, 09H
	LEA DX, Stock.phoneName
	INT 21H
	
	MOV CX, 15
	LEA DI, Stock.phoneName
	CALL calculateStringLength
	SUB CX, BX
	CALL fillSpace
	
	delimiter
	
	MOV AH, 09H
	LEA DX, Stock.phoneRAM
	INT 21H
	
	MOV CX, 10
	LEA DI, Stock.phoneRAM
	CALL calculateStringLength
	SUB CX, BX
	CALL fillSpace
	
	delimiter
	
	MOV AH, 09H
	LEA DX, Stock.phoneROM
	INT 21H
	
	MOV CX, 10
	LEA DI, Stock.phoneROM
	CALL calculateStringLength
	SUB CX, BX
	CALL fillSpace
	
	delimiter
	
	MOV AH, 09H
	LEA DX, Stock.phoneColor
	INT 21H
	
	MOV CX, 10
	LEA DI, Stock.phoneColor
	CALL calculateStringLength
	SUB CX, BX
	CALL fillSpace
	
	delimiter
	
	CMP choice,1
	JE qtyIn
	CMP choice,2
	JE qtyOut
	CMP choice,3
	JE qtyInv
	
	qtyIn:
		MOV BX,10
		MOV AX, WORD PTR Stock.totalStockInQty
		MOV DX, WORD PTR Stock.totalStockInQty + 2
		CALL displayNumber
		CALL fillSpace
		JMP reportDetailsEnd
		
	qtyOut:
		MOV BX,10
		MOV AX, WORD PTR Stock.totalStockOutQty
		MOV DX, WORD PTR Stock.totalStockOutQty + 2
		CALL displayNumber
		CALL fillSpace
		JMP reportDetailsEnd
		
	qtyInv:
		MOV BX,10
		MOV AX, WORD PTR Stock.phoneQty
		MOV DX, WORD PTR Stock.phoneQty + 2
		CALL displayNumber
		CALL fillSpace
		
		delimiter
		
		CALL calStockValues
	
	reportDetailsEnd:
		RET
	
rptStockDetails ENDP

sumOfStock PROC

	CMP choice, 1
	JE calQtyStockIn
	CMP choice, 2
	JE calQtyStockOut
	CMP choice, 3
	JE calQtyInvStat
	
	calQtyStockIn:
		MOV AX, WORD PTR Stock.totalStockInQty
		MOV DX, WORD PTR Stock.totalStockInQty + 2
	
	JMP finalCal
	
	calQtyStockOut:
		MOV AX, WORD PTR Stock.totalStockOutQty
		MOV DX, WORD PTR Stock.totalStockOutQty + 2
	
	JMP finalCal
	
	calQtyInvStat:
		MOV AX, WORD PTR Stock.phoneQty
		MOV DX, WORD PTR Stock.phoneQty + 2
	
	finalCal:
		ADD WORD PTR totalStock, AX
		ADD WORD PTR totalStock + 2, DX
	
	RET

sumOfStock ENDP

calStockValues PROC
	
	MOV AX, WORD PTR Stock.phonePrice
	MOV DX, WORD PTR Stock.phonePrice + 2
	
	MUL WORD PTR Stock.phoneQty
	
	ADD WORD PTR stockValue, AX
	ADD WORD PTR stockValue + 2, DX
	
	MOV WORD PTR tempStockValue, AX
	MOV WORD PTR tempStockValue + 2, DX
	
	MOV AX, WORD PTR Stock.phonePriceFP
	MOV DX, WORD PTR Stock.phonePriceFP + 2
	
	MUL WORD PTR Stock.phoneQty
	DIV hundred16bits
	
	ADD WORD PTR stockValue, AX
	ADD WORD PTR tempStockValue, AX
	MOV WORD PTR tempStockValueFP, DX
	
	ADD WORD PTR stockValueFP, DX
	
	
	MOV AX, WORD PTR tempStockValue
	MOV DX, WORD PTR tempStockValue + 2
	CALL displayNumber
	
	MOV AH, 02H
	MOV DL, '.'
	INT 21H
	
	MOV AX, WORD PTR tempStockValueFP
	MOV DX, WORD PTR tempStockValueFP + 2
	CALL displayFloatingPoint
	
	RET
	
calStockValues ENDP

sumOfStockValue PROC
	
	MOV AX, WORD PTR stockValueFP
	MOV DX, WORD PTR stockValueFP + 2
	
	DIV hundred16bits
	
	ADD WORD PTR stockValue, AX
	MOV WORD PTR stockValueFP, DX
	
	MOV AX, WORD PTR stockValue
	MOV DX, WORD PTR stockValue + 2
	CALL displayNumber
	
	MOV AH, 02H
	MOV DL, '.'
	INT 21H
	
	MOV AX, WORD PTR stockValueFP
	MOV DX, WORD PTR stockValueFP + 2
	CALL displayFloatingPoint
	
	MOV WORD PTR stockValue, 0
	MOV WORD PTR stockValue + 2, 0
	
	MOV WORD PTR stockValueFP, 0
	MOV WORD PTR stockValueFP + 2, 0
	
	RET
	
sumOfStockValue ENDP

displayFloatingPoint PROC

	DIV ten8Bits
	MOV BX, AX
	
	MOV AH, 02H
	MOV DL, BL
	ADD DL, 30H
	INT 21H
	
	MOV AH, 02H
	MOV DL, BH
	ADD DL, 30H
	INT 21H
	
	RET
	
displayFloatingPoint ENDP