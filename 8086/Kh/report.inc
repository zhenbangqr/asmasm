.DATA
	stockInTitle DB "==                Monthly Stock In Report                ==", 0DH, 0AH, "$"
	stockOutTitle DB "==                Monthly Stock Out Report               ==", 0DH, 0AH, "$"
	invStatTitle DB "==                Inventory Status Report                ==", 0DH, 0AH, "$"
	phoneDetailsTitle DB "Phone Name     |RAM       |ROM       |Color     |Quantity", 0DH, 0AH, "$"
	phoneDetailsTitleDivider DB "===============|==========|==========|==========|==========", 0DH, 0AH, "$"
	
.CODE

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

displayDetails PROC
	
	CALL openReadStockFile
	
	readUntilEnd:
		CALL readStockDetails
		JMP displayReportDetails
	
	checkEndOfFile:
		MOV AX, [SI]
		CMP AX, 0
		JE reportFinish
		
		JMP readUntilEnd
	
	displayReportDetails:
	
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
		
		MOV AX, WORD PTR Stock.phonePrice
		MOV DX, WORD PTR Stock.phonePrice + 2
		CALL displayNumber
		
		print_Newline
		
		JMP checkEndOfFile
	
	reportFinish:
		CALL closeStockFile
	
		RET
	
displayDetails ENDP