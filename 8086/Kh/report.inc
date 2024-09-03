.DATA
	phoneDetailsTitle DB "No.|Phone Name     |RAM       |ROM       |Color     |Quantity", 0DH, 0AH, "$"
	phoneDetailsTitleDivider DB "===|===============|==========|==========|==========|==========", 0DH, 0AH, "$"
	phoneStockTotal DB "                                               Total|$"
	totalStock DW 0
	totalStockHigh DW 0
	stockQtyNum DW 0
	stockQtyNumHigh DW 0

.CODE

stockReportDetails PROC
	
	reportDivider
	
	MOV AH,09H
	LEA DX,phoneDetailsTitle
	INT 21H
	
	MOV AH,09H
	LEA DX,phoneDetailsTitleDivider
	INT 21H
	
	CALL displayDetails
	
	print_Newline
	
	RET
	
stockReportDetails ENDP

displayDetails PROC
	
	CALL openReadStockFile
	
	readUntilEnd:
		CALL readStockDetails
		INC numStockFound
		CALL sumOfStock
		JMP displayReportDetails
	
	checkEndOfFile:
		MOV AX, [SI]
		CMP AX, 0
		JE reportFinish
		
		JMP readUntilEnd
	
	displayReportDetails:
	
		CALL showPhoneDetails
		
		JMP checkEndOfFile
	
	reportFinish:
		MOV numStockFound, 0
		CALL closeFile
		
		reportDivider
		
		MOV AH,09H
		LEA DX,phoneStockTotal
		INT 21H
		
		MOV AX, totalStock
		MOV DX, totalStockHigh
		CALL displayNumber
		
		MOV totalStock, 0
		MOV totalStockHigh, 0
		
		RET
	
displayDetails ENDP

showPhoneDetails PROC

	MOV AX, WORD PTR numStockFound
		MOV DX, WORD PTR numStockFound + 2
		CALL displayNumber
		
		MOV AH, 02H
		MOV DL, '.'
		INT 21H
		
		CMP numStockFound,10
		JGE numberNoSpace
		
		numberSpace:
			space
		
		numberNoSpace:
			delimiter
		
		MOV AH, 09H
		LEA DX, Stock.phoneName
		INT 21H
		
		delimiter
		
		MOV AH, 09H
		LEA DX, Stock.phoneRAM
		INT 21H
		
		delimiter
		
		MOV AH, 09H
		LEA DX, Stock.phoneROM
		INT 21H
		
		delimiter
		
		MOV AH, 09H
		LEA DX, Stock.phoneColor
		INT 21H
		
		delimiter
		
		
		CMP choice,1
		JE qtyIn
		CMP choice,2
		JE qtyOut
		CMP choice,3
		JE qtyInv
		
		qtyIn:
			
		qtyOut:
			
		qtyInv:
			MOV AX, WORD PTR Stock.phoneQty
			MOV DX, WORD PTR Stock.phoneQty + 2
			CALL displayNumber
		
		print_Newline
		
		RET
	
showPhoneDetails ENDP

sumOfStock PROC
	; Accumulate phoneQty into totalStock
	MOV AX, WORD PTR Stock.phoneQty
	ADD totalStock, AX

	; If there's a high part, accumulate it
	MOV DX, WORD PTR Stock.phoneQty + 2
	ADD totalStockHigh, DX
	
	RET

sumOfStock ENDP

compareQty PROC
	CALL openReadStockFile
	CALL readStockDetails
	
	MOV AX, WORD PTR Stock.phoneQty  
	MOV DX, WORD PTR Stock.phoneQty + 2
	MOV stockQtyNum, AX
	MOV stockQtyNumHigh, DX
	
	compareNextQty:
		CALL readStockDetails
	
		MOV BX, WORD PTR Stock.phoneQty  
		MOV CX, WORD PTR Stock.phoneQty + 2
	
		CMP CX, stockQtyNumHigh
		JB smallerNum
		CMP BX, stockQtyNum
		JB smallerNum
	
		MOV stockQtyNum,BX
		MOV stockQtyNumHigh,CX
	
	smallerNum:

		MOV AX, [SI]
		CMP AX, 0
		JE compareQtyDone
		
		JMP compareNextQty
		
	compareQtyDone:
		CALL closeFile
		
		MOV AH,09H
		LEA DX,highestStockOut
		INT 21H
		
		MOV AX, stockQtyNum
		MOV DX, stockQtyNumHigh
		CALL displayNumber
		
		print_Newline

		RET
	
compareQty ENDP