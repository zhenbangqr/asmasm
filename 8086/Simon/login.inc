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
	
	loginFailMsg DB "Invalid User-ID or Password, please try again.$"
	loginSucMsg DB " Welcome to Phone Inventory System$"
	
.CODE

user_Login PROC
    
	;--Display the login title
    MOV AH,09H
    LEA DX,loginTitle
    INT 21H

    ;--New line
    MOV AH,09H
    LEA DX,newLine
    INT 21H
	
getUserIDInput:
    
	;--Prompt for User-ID
    MOV AH,09H
    LEA DX,promptLoginID
    INT 21H
	
	;Input User-ID
    MOV AH,0AH
	LEA DX, userID
    INT 21H
    
	;--Nextline
	MOV AH,09H
    LEA DX,newLine
    INT 21H

getDefaultIDlength:
	MOV BX,0
	MOV SI,0
	
	;--Count the length of default User-ID by comparing '$' symbol
	;--Loop the count if wrong symbol else jump to compareTwoIDlength 
	countIDlength:
	CMP defaultUserID[SI],'$'
	JE compareTwoIDLength	
	INC SI
	MOV BX,SI
	JMP countIDlength

compareTwoIDLength:
	
	;--Compare the length of inputted User-ID and default User-ID
	;--If equal jump to compareIDinput ， else jump to invalidInput 
	MOV BH,userID[1]
	CMP BL,BH
	JNE invalidInput

compareIDinput:
	
	;--Compare User-ID character one by one 
	;--If all true then jump to getPasswordInput , else jump to invalidInput 
	MOV CX, 0
	MOV CL, BL
	MOV SI, 2
	MOV DI, 0
	
	userIdCompareLoop:
		MOV BL,userID[SI]
		CMP BL,defaultUserID[DI]
		
		JNE invalidInput
		INC SI
		INC DI
	LOOP userIdCompareLoop
	
	JMP getPasswordInput

invalidInput:
	;--Loop back to the getUserIDInput until user entered exact User-ID & Password
	
	MOV AH,09H
    LEA DX,newLine
    INT 21H
	
    ;--Display invalid ID message
    MOV AH,09H
    LEA DX,loginFailMsg
    INT 21H

    ;--Nextline
    MOV AH,09H
    LEA DX,newLine
    INT 21H
	
    JMP getUserIDInput
	
getPasswordInput:
    ;--Prompt for User Password
    MOV AH,09H
    LEA DX,promptPassword
    INT 21H
	
	;--Input User Password
    MOV AH,0AH
	LEA DX, userPassword
    INT 21H
    
	;--Nextline
	MOV AH,09H
    LEA DX,newLine
    INT 21H	

getDefaultPasswordLength:
	MOV BX,0
	MOV SI,0
	
	;--Count the length of default Password by comparing '$' symbol
	;--Loop the count if wrong symbol else jump to compareTwoPasswordLength 
	
	countPasswordLength:
	CMP defaultPassword[SI],'$'
	JE compareTwoPasswordLength
	INC SI
	MOV BX,SI
	JMP countPasswordLength

compareTwoPasswordLength:
	;--Compare the length of inputted User Password and default User Password
	;--If equal jump to compareTwoPasswordInput ， else jump to invalidInput 
	
	MOV BH,userPassword[1]
	CMP BL,BH
	JNE invalidInput
	
compareTwoPasswordInput:
	MOV CX, 0
	MOV CL, BL
	MOV SI, 2
	MOV DI, 0
	
	userPasswordCompareLoop:
		MOV BL,userPassword[SI]
		CMP BL,defaultPassword[DI]
				
		JNE invalidInput
		INC SI
		INC DI
	LOOP userPasswordCompareLoop
	
	MOV AH,09H
    LEA DX,newLine
    INT 21H
	
    ; Display Success login
    MOV AH,09H
    LEA DX,loginSucMsg
    INT 21H

    ; New line
    MOV AH,09H
    LEA DX,newLine
    INT 21H
	
	RET
	
user_Login ENDP

.CODE
