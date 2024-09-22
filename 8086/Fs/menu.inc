.DATA
    menuTitle DB "========================================", 0DH, 0AH, "==       Phone Inventory System       ==", 0DH, 0AH, "========================================", 0DH, 0AH, "$"
    menuChoice DB "1. Stock In", 0DH, 0AH, "2. Stock Out", 0DH, 0AH, "3. Search Stock", 0DH, 0AH, "4. Generate Stock Report", 0DH, 0AH, "5. Log Out", 0DH, 0AH, "$"
    promptChoice DB "Enter a digit(1-5): $"
	
.CODE

;-----------------------
;	user_Menu
;	menu after log In
;-----------------------
user_Menu PROC

	choiceLoop:
		; Show Menu Title
		MOV AH, 09H
		LEA DX, menuTitle
		INT 21H    
	
		; Show Menu Choice
		MOV AH, 09H
		LEA DX, menuChoice
		INT 21H
	
		; Enter Choice
		MOV AH, 09H
		LEA DX, promptChoice
		INT 21H
	
		; Get User Input
		resetAX
		MOV AH, 01H
		INT 21H
		SUB AL, 30H    
		
		print_NewLine
		
		; Input Validation -- JB:Jump Below 1 --JA:Jump Above 5
		CMP AL, 1
		JB invalidChoice
		CMP AL, 5
		JA invalidChoice
		
		MOV choice,AL
		
		clnScr
		
		RET 
		
	invalidChoice:
		MOV AH, 09H
		LEA DX, invalidInputMsg
		INT 21H
		
		print_NewLine
		pause
		clnScr
		
		JMP choiceLoop
		
user_Menu ENDP