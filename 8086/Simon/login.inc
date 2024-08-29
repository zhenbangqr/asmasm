.DATA 
	loginTitle DB "========================================", 0dh, 0ah, "==                Login               ==", 0dh, 0ah, "========================================", 0dh,0ah, "$"
	promptLoginID DB "Enter user-ID :$"
	promptPassword DB "Password :$"
	
	userID LABEL BYTE
	maxUserIdLength DB 20
	getUserIdLength DB ?
	inputUserID DB 20 DUP("$")
	
	userPassword LABEL BYTE
	maxUserPasswordLength DB 20
	getUserPasswordLength DB ?
	inputUserPassword DB 20 DUP("$")
	
	defaultPassword DB "best69$"
	defaultUserID DB "user69$"
	
	correctIDLength EQU ($-defaultUserID)
	correctPasswordLength EQU ($-defaultPassword)
	
	loginFailMsg DB "Invalid User-ID or Password, please try again.$"
	loginSucMsg DB " Welcome to Phone Inventory System$"
	
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
    ; Prompt for User ID
    MOV AH,09H
    LEA DX,promptLoginID
    INT 21H
	
	;
    MOV AH,0AH
	LEA DX, userID
    INT 21H
    
	MOV AH,09H
    LEA DX,newLine
    INT 21H
	
	MOV AH,09H
    LEA DX, promptPassword
    INT 21H
	
	;
    MOV AH,0AH
	LEA DX,userPassword
    INT 21H
    
	MOV AH,09H
    LEA DX,newLine
    INT 21H
	
	JMP compareInput
	
compareInput:
	MOV CX, 0
	MOV CL, userID[1]
	MOV SI, 2
	MOV DI, 0
	userIdCompareLoop:
		MOV BL,userID[SI]
		CMP BL,defaultUserID[DI]
		
		MOV AH,02H
		MOV DL,userID[SI]
		INT 21H
		
		MOV AH,02H
		MOV DL,defaultUserID[DI]
		INT 21H
		
		JNE invalidInput
		INC SI
		INC DI
	LOOP userIdCompareLoop
	
	MOV CX, 0
	MOV CL, userPassword[1]
	MOV SI, 2
	MOV DI, 0
	userPasswordCompareLoop:
		MOV BL,userPassword[SI]
		CMP BL,defaultPassword[DI]
		
		MOV AH,02H
		MOV DL,userPassword[SI]
		INT 21H
		
		MOV AH,02H
		MOV DL,defaultPassword[DI]
		INT 21H
		
		JNE invalidInput
		INC SI
		INC DI
	LOOP userPasswordCompareLoop
	
	MOV AH,09H
    LEA DX,newLine
    INT 21H
	
    ; Display invalid ID message
    MOV AH,09H
    LEA DX,loginSucMsg
    INT 21H

    ; New line
    MOV AH,09H
    LEA DX,newLine
    INT 21H
	
	RET
	

invalidInput:
	MOV AH,09H
    LEA DX,newLine
    INT 21H
	
    ; Display invalid ID message
    MOV AH,09H
    LEA DX,loginFailMsg
    INT 21H

    ; New line
    MOV AH,09H
    LEA DX,newLine
    INT 21H

    JMP getUserInput
		
user_Login ENDP

.CODE
