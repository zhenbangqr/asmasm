.DATA
    menuTitle db "========================================", 0dh, 0ah, "==       Phone Inventory System       ==", 0dh, 0ah, "========================================", 0dh,0ah, "$"
    menuChoice db "1. Stock In", 0dh, 0ah, "2. Stock Out", 0dh, 0ah, "3. Search Stock", 0dh, 0ah, "4. Generate Stock Report", 0dh, 0ah, "5. Exit", 0dh, 0ah, "$"
    promptChoice db "Enter a digit(1-5): $"
.CODE

user_Menu PROC

choiceLoop:
    ; Show Menu Title
	MOV AH,09H
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
    MOV AX, 0
    MOV AH, 01H
    INT 21H
    SUB AL, 30H    
    
    ; New Line
    mov ah, 09h
    lea dx, newLine
    INT 21H
    
    ; Input Validation -- JB:Jump Below 1 --JA:Jump Above 5
    CMP AL, 1
    JB invalidChoice
    CMP AL, 5
    JA invalidChoice
	
	MOV choice,AL
	RET 
    
invalidChoice:
	CALL clear_Screen 
	
    MOV AH, 09H
    LEA DX, invalidInputMsg
    INT 21H
    
    MOV AH, 09H
    LEA DX, newLine
    INT 21H
    
    JMP choiceLoop
    
user_Menu ENDP
