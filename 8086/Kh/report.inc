.DATA
	currStockDetails DB "No.|Phone Name     |RAM       |ROM       |Color     |Quantity  |Stock Values", 0DH, 0AH, "$"
	phoneProfitDetails DB "No.|Phone Name     |RAM       |ROM       |Color     |Quantity  |Profit", 0DH, 0AH, "$"
	currStockPhoneProfitDetailsDivider DB "===|===============|==========|==========|==========|==========|============", 0DH, 0AH, "$"
	stockRptDetails DB "No.|Phone Name     |RAM       |ROM       |Color     |Quantity", 0DH, 0AH, "$"
	stockRptDetailsDivider DB "===|===============|==========|==========|==========|==========", 0DH, 0AH, "$"
	stockRptTotal DB "                                               Total|$"
	
	totalStock DD 0
	
	stockValue DD 0
	stockValueFP DD 0
	totalStockValue DD 0
	totalStockValueFP DD 0
	
	profit DD 0
	profitFP DD 0
	totalProfit DD 0
	totalProfitFP DD 0
	sumOfTotalProfit DD 0
	sumOfTotalProfitFP DD 0
	profitRate DW 6

.CODE

;--------------------------------------
;	stockReportDetails
;	display the header of the report
;--------------------------------------
stockReportDetails PROC
	
	CMP choice, 1
	JE displayCurrStock
	CMP choice, 5
	JE displayPhoneProfit
		
	MOV AH, 09H
	LEA DX, stockRptDetails
	INT 21H
	
	MOV AH, 09H
	LEA DX, stockRptDetailsDivider
	INT 21H
	
	JMP detailsNext
	
	displayCurrStock:
		MOV AH, 09H
		LEA DX, currStockDetails
		INT 21H
		
		JMP displayDetailsDivider
		
	displayPhoneProfit:
		MOV AH, 09H
		LEA DX, phoneProfitDetails
		INT 21H
	
	displayDetailsDivider:
		MOV AH, 09H
		LEA DX, currStockPhoneProfitDetailsDivider
		INT 21H
	
	detailsNext:
		CALL rptDetails
		
	RET
	
stockReportDetails ENDP

;-------------------------------------------------------------------
;	rptDetails
;	read the data from file and display the summary of the report
;	Eg. Total stock and Total stock value
;-------------------------------------------------------------------
rptDetails PROC
	
	CALL openReadStockFile
	CALL jumpToNextLine
	
	rptReadStock:
		CALL clearStockVariableBuffer
		CALL readStockDetails
		
		CMP choice, 4
		JE cmpLowStock
		
		incNumStock:
			INC numStockFound
			CALL sumOfStock
		
		JMP displayReportDetails
	
	checkEOF:
		MOV AX, [SI]
		CMP AX, 0
		JE reportSummary
		
		JMP rptReadStock
		
	cmpLowStock:
		CMP WORD PTR Stock.phoneQty, 10
		JB incNumStock
		
		JMP checkEOF
	
	displayReportDetails:
		CALL rptStockDetails
		
		JMP checkEOF
	
	reportSummary:
		MOV numStockFound, 0
		CALL closeReadFile
		
		CMP choice, 1
		JE displayCSPPDivider
		CMP choice, 5
		JE displayCSPPDivider
		CMP choice, 4
		JE reportEnd
		
		rptDivider
		
		JMP displayTotalStock
		
		displayCSPPDivider:
			csppDivider
		
		displayTotalStock:
			MOV AH, 09H
			LEA DX, stockRptTotal
			INT 21H
			
			MOV BX, 10
			MOV AX, WORD PTR totalStock
			MOV DX, WORD PTR totalStock + 2
			CALL displayNumber
			CALL fillSpace
		
		CMP choice, 1
		JE displaySumStockValue
		CMP choice, 5
		JE displaySumTotalProfit
		
		print_Newline
		JMP reportEnd
		
		displaySumStockValue:
			delimiter
			CALL displaySumOfStockValue
			print_Newline
			
			JMP reportEnd
			
		displaySumTotalProfit:
			delimiter
			CALL displaySumOfTotalProfit
			print_Newline
		
		reportEnd:
			MOV WORD PTR totalStock, 0
			MOV WORD PTR totalStock + 2, 0
			RET
			
rptDetails ENDP

;-----------------------------------------------
;	rptStockDetails
;	display the details of the report
;	Eg. Phone Name, RAM, ROM, Color, Quantity
;-----------------------------------------------
rptStockDetails PROC

	resetAX
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
	
	CMP choice, 1
	JE qtyInv
	CMP choice, 2
	JE qtyIn
	CMP choice, 3
	JE qtyOut
	CMP choice, 4
	JE qtyInv
	CMP choice, 5
	JE qtyInv
	
	qtyIn:
		MOV BX, 10
		MOV AX, WORD PTR Stock.totalStockInQty
		MOV DX, WORD PTR Stock.totalStockInQty + 2
		CALL displayNumber
		CALL fillSpace
		JMP reportDetailsEnd
		
	qtyOut:
		MOV BX, 10
		MOV AX, WORD PTR Stock.totalStockOutQty
		MOV DX, WORD PTR Stock.totalStockOutQty + 2
		CALL displayNumber
		CALL fillSpace
		JMP reportDetailsEnd
		
	qtyInv:
		MOV BX, 10
		MOV AX, WORD PTR Stock.phoneQty
		MOV DX, WORD PTR Stock.phoneQty + 2
		CALL displayNumber
		CALL fillSpace
		
		CMP choice, 1
		JE calculateStockValue
		CMP choice, 5
		JE calculatePhoneProfit

		JMP reportDetailsEnd
		
		calculateStockValue:
			delimiter
			CALL calStockValue
			
			JMP reportDetailsEnd
		
		calculatePhoneProfit:
			delimiter
			CALL calProfit
	
	reportDetailsEnd:
		print_Newline
		RET
		
rptStockDetails ENDP

;---------------------------------------------
;	sumOfStock
;	calculate the sum of the quantity stock
;---------------------------------------------
sumOfStock PROC

	CMP choice, 1
	JE calQtyInvStock
	CMP choice, 2
	JE calQtyStockIn
	CMP choice, 3
	JE calQtyStockOut
	CMP choice, 5
	JE calQtyInvStock
	
	calQtyStockIn:
		MOV AX, WORD PTR Stock.totalStockInQty
		MOV DX, WORD PTR Stock.totalStockInQty + 2
	
	JMP finalCal
	
	calQtyStockOut:
		MOV AX, WORD PTR Stock.totalStockOutQty
		MOV DX, WORD PTR Stock.totalStockOutQty + 2
	
	JMP finalCal
	
	calQtyInvStock:
		MOV AX, WORD PTR Stock.phoneQty
		MOV DX, WORD PTR Stock.phoneQty + 2
	
	finalCal:
		ADD WORD PTR totalStock, AX
		ADD WORD PTR totalStock + 2, DX
	
	RET
	
sumOfStock ENDP

;------------------------------------------------------
;	calStockValue
;	calculate the stock value for each type of phone
;------------------------------------------------------
calStockValue PROC

	MOV AX, WORD PTR Stock.phonePrice
	MOV DX, WORD PTR Stock.phonePrice + 2
	
	MUL WORD PTR Stock.phoneQty
	
	MOV WORD PTR stockValue, AX
	MOV WORD PTR stockValue + 2, DX
	
	MOV AX, WORD PTR Stock.phonePriceFP
	MOV DX, WORD PTR Stock.phonePriceFP + 2
	
	MUL WORD PTR Stock.phoneQty
	DIV hundred16bits
	
	ADD WORD PTR stockValue, AX
	MOV WORD PTR stockValueFP, DX
	
	displayStockValue:
		MOV AX, WORD PTR stockValue
		MOV DX, WORD PTR stockValue + 2
		CALL displayNumber
		
		MOV AH, 02H
		MOV DL, '.'
		INT 21H
		
		MOV AX, WORD PTR stockValueFP
		MOV DX, WORD PTR stockValueFP + 2
		CALL displayFloatingPoint
	
	sumTotalStockValue:
		MOV AX, WORD PTR stockValue
		MOV DX, WORD PTR stockValue + 2
		
		ADD WORD PTR totalStockValue, AX
		ADD WORD PTR totalStockValue + 2, DX
		
		MOV AX, WORD PTR stockValueFP
		MOV DX, WORD PTR stockValueFP + 2
		
		ADD WORD PTR totalStockValueFP, AX
		ADD WORD PTR totalStockValueFP + 2, DX
		
	RET
	
calStockValue ENDP

;------------------------------------------------------------
;	sumOfStockValue
;	calculate the sum of the stock value for all the phone
;------------------------------------------------------------
displaySumOfStockValue PROC

	;round the floating point and add to profit
	MOV AX, WORD PTR totalStockValueFP
	MOV DX, WORD PTR totalStockValueFP + 2
	
	DIV hundred16bits
	
	ADD WORD PTR totalStockValue, AX
	MOV WORD PTR totalStockValueFP, DX

	MOV AX, WORD PTR totalStockValue
	MOV DX, WORD PTR totalStockValue + 2
	CALL displayNumber
	
	MOV AH, 02H
	MOV DL, '.'
	INT 21H
	
	MOV AX, WORD PTR totalStockValueFP
	MOV DX, WORD PTR totalStockValueFP + 2
	CALL displayFloatingPoint
	
	MOV WORD PTR totalStockValue, 0
	MOV WORD PTR totalStockValue + 2, 0
	
	MOV WORD PTR totalStockValueFP, 0
	MOV WORD PTR totalStockValueFP + 2, 0
	
	RET
	
displaySumOfStockValue ENDP

;---------------------------------------------------
;	calProfit
;	calculate the profit for each phone
;	display the total profit for each phone model
;	calculate the sum of total profit
;---------------------------------------------------
calProfit PROC

	MOV WORD PTR profit, 0
	MOV WORD PTR profit + 2, 0
	MOV WORD PTR profitFP, 0
	MOV WORD PTR profitFP + 2, 0
	MOV WORD PTR totalProfit, 0
	MOV WORD PTR totalProfit + 2, 0
	MOV WORD PTR totalProfitFP, 0
	MOV WORD PTR totalProfitFP + 2, 0
	
	;calculateProfitForOnePhone
	MOV AX, WORD PTR Stock.phonePrice
	MOV DX, WORD PTR Stock.phonePrice + 2
	
	MUL profitRate
	DIV hundred16bits
	
	MOV WORD PTR profit, AX
	
	;DX should be store the floating point value
	;MOV DX to AX to multiply with phone quantity first
	MOV AX, DX
	MUL WORD PTR Stock.phoneQty
	
	;store to profitFP
	MOV WORD PTR profitFP, AX

	;calculate the profit floating point for phonePriceFP
	MOV AX, WORD PTR Stock.phonePriceFP
	MOV DX, WORD PTR Stock.phonePriceFP + 2
	
	MUL profitRate
	MUL WORD PTR Stock.phoneQty
	DIV hundred16bits
	
	;cmp to round the floating point to 2 decimal places
	CMP DX, 10
	JL compareWith5
		
	CMP DX, 50
	JL addProfitFP
		
	compareWith5:
		CMP DX, 5
		JL addProfitFP
		INC AX
	
	addProfitFP:
		ADD WORD PTR profitFP, AX
	
	;multiply the profit get above to get the total profit
	MOV AX, WORD PTR profit
	MOV DX, WORD PTR profit + 2
	
	MUL WORD PTR Stock.phoneQty
	
	MOV WORD PTR totalProfit, AX
	MOV WORD PTR totalProfit + 2, DX
	
	;divide the profit floating point with 100 to seperate it into RM and Sen.
	;For example, 110 will become RM1 and 10sen
	MOV AX, WORD PTR profitFP
	MOV DX, WORD PTR profitFP + 2
		
	DIV hundred16bits
	
	MOV WORD PTR totalProfitFP, DX
	ADD WORD PTR totalProfit, AX
	
	displayTotalProfit:
		MOV AX, WORD PTR totalProfit
		MOV DX, WORD PTR totalProfit + 2
		CALL displayNumber
		
		MOV AH, 02H
		MOV DL, '.'
		INT 21H
		
		MOV AX, WORD PTR totalProfitFP
		MOV DX, WORD PTR totalProfitFP + 2
		CALL displayFloatingPoint
	
	sumTotalProfit:
		MOV AX, WORD PTR totalProfit
		MOV DX, WORD PTR totalProfit + 2
		
		ADD WORD PTR sumOfTotalProfit, AX
		ADD WORD PTR sumOfTotalProfit + 2, DX
		
		MOV AX, WORD PTR totalProfitFP
		MOV DX, WORD PTR totalProfitFP + 2
		
		ADD WORD PTR sumOfTotalProfitFP, AX
		ADD WORD PTR sumOfTotalProfitFP + 2, DX
	
	RET
	
calProfit ENDP

;-------------------------------------
;	displaySumOfTotalProfit
;	display the sum of total profit
;-------------------------------------
displaySumOfTotalProfit PROC

	;round the floating point and add to profit
	MOV AX, WORD PTR sumOfTotalProfitFP
	MOV DX, WORD PTR sumOfTotalProfitFP + 2
	
	DIV hundred16bits
	
	ADD WORD PTR sumOfTotalProfit, AX
	MOV WORD PTR sumOfTotalProfitFP, DX

	;display
	MOV AX, WORD PTR sumOfTotalProfit
	MOV DX, WORD PTR sumOfTotalProfit + 2

	CALL displayNumber
	
	MOV AH, 02H
	MOV DL, '.'
	INT 21H
	
	MOV AX, WORD PTR sumOfTotalProfitFP
	MOV DX, WORD PTR sumOfTotalProfitFP + 2
	CALL displayFloatingPoint
	
	MOV WORD PTR sumOfTotalProfit, 0
	MOV WORD PTR sumOfTotalProfit + 2, 0
	
	MOV WORD PTR sumOfTotalProfitFP, 0
	MOV WORD PTR sumOfTotalProfitFP + 2, 0
	
	RET
	
displaySumOfTotalProfit ENDP

;---------------------------------------------------------------
;	displayFloatingPoint
;	display the floating point for the stock value and profit
;---------------------------------------------------------------
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