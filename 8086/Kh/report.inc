.DATA

	stockInTitle DB "==               Monthly Stock In Report                ==", 0DH, 0AH, "$"
	stockOutTitle DB "==               Monthly Stock Out Report               ==", 0DH, 0AH, "$"
	invStatTitle DB "==               Inventory Status Report                ==", 0DH, 0AH, "$"
	phoneDetailsTitle DB "Phone Name          |RAM     |ROM     |Color   |Quantity", 0DH, 0AH, "$"
	phoneDetailsTitleDivider DB "====================|========|========|========|==========", 0DH, 0AH, "$"

	; Example for phone details
	phoneNameee DB "Tiong Fatt Siong    $"
	phoneRammm DB "8GB     $"
	phoneRommm DB "128GB   $"
	phoneColorrr DB "White   $"
	stockInQtyyy DW 10
	
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