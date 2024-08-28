.DATA 
	loginTitle DB "========================================", 0dh, 0ah, "==       Phone Inventory System       ==", 0dh, 0ah, "========================================", 0dh,0ah, "$"
	promptLoginID DB "Enter user-ID :$"
	promptPassword DB "Password :$"
	
	userID LABEL BYTE
	maxUserIdLength DB 10
	getUserIdLength DB ?
	inputUserID DB 10 DUP("$")
	
	userPassword LABEL BYTE
	maxUserPasswordLength DB 10
	getUserPasswordLength DB ?
	inputUserPassword DB 10 DUP("$")
	
	defaultPassword DB "best69"
	defaultUserID DB "user69"
	
	defaultLengthPassword EQU ($-defaultPassword)
	defaultLengthUSER EQU ($-defaultUserID)
	
	invalidInput DB "Invalid User-ID or Password, please try again.$"
	
.CODE

user_Login PROC
    ; Display the login title
    MOV AH,09H
    LEA DX,loginTitle
    INT 21H

    ; New line
    MOV AH,09H
    LEA DX,newLine
    INT 21H
	
getUserInput:
	MOV SI,0
    ; Prompt for User ID
    MOV AH,09H
    LEA DX,promptLoginID
    INT 21H
	
    MOV AH,0AH
	LEA DX,userID
    INT 21H
    
	MOV AH,09H
    LEA DX,newLine
    INT 21H
	
	MOV AH,09H
    LEA DX,newLine
    INT 21H
    
compareUserID:
	
	MOV CX,defaultLengthUSER
	userIdCompare:
	
	MOV BL,inputUserID[SI]
    CMP BL,defaultUserID[SI]
	
    JNE invalidUser
	INT 21H
	
	INC SI	
	LOOP userIdCompare
    JMP getUserPassword

invalidUser:
	
	MOV AH,09H
    LEA DX,newLine
    INT 21H
	
    ; Display invalid ID message
    MOV AH,09H
    LEA DX,invalidInput
    INT 21H

    ; New line
    MOV AH,09H
    LEA DX,newLine
    INT 21H
	
	MOV AX, 4C00H
    INT 21H
    JMP getUserInput

getUserPassword:
	
    ; Prompt for Password
    MOV AH,09H
    LEA DX,promptPassword
    INT 21H
	
	MOV AH,0AH
	LEA DX,userPassword
    INT 21H
    
	MOV AH,09H
    LEA DX,newLine
    INT 21H
	
	MOV AH,09H
    LEA DX,inputUserPassword
    INT 21H


compareUserPassword:
	
	MOV CX,defaultLengthPassword
	PasswordCompare:
	
	MOV BL,inputUserPassword[SI+2]
    CMP BL,defaultPassword[SI]
	
	MOV AH,02H
	MOV DL,inputUserPassword[SI+1]
	INT 21H
	
	MOV AH,09H
    LEA DX,newLine
    INT 21H
	
	MOV AH,02H
	MOV DL,defaultPassword[SI]
	INT 21H
	
    ;--JNE invalidUser
	INT 21H
	
	INC SI	
	LOOP PasswordCompare
    
	RET
		
user_Login ENDP

.CODE
