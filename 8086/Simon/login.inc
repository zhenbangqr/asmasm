.DATA 
	loginTitle DB "========================================", 0dh, 0ah, "==                Login               ==", 0dh, 0ah, "========================================", 0dh,0ah, "$"
	promptLoginID DB "Enter user-ID :$"
	promptPassword DB "Password :$"
	
	inputID LABEL BYTE
	maxUserIdLength DB 20
	getUserIdLength DB ?
	inputUserID DB 20 DUP("$")
	
	inputPassword LABEL BYTE
	maxUserPasswordLength DB 20
	getUserPasswordLength DB ?
	inputUserPassword DB 20 DUP("$")
	
	loginFailMsg DB "Invalid User-ID or Password, please try again.$"
	loginSucMsg DB " Welcome to Phone Inventory System$"
	
.CODE

user_Login PROC
    
	;--Display the login title
    MOV AH,09H
    LEA DX,loginTitle
    INT 21H

    ;--New line
    MOV AH, 09H
    LEA DX, newLine
    INT 21H
	
getUserIDInput:
    
	;--Prompt for User-ID
    MOV AH, 09H
    LEA DX, promptLoginID
    INT 21H
	
	;Input User-ID
    MOV AH, 0AH
	LEA DX, inputID
    INT 21H
    
	;--Nextline
	MOV AH, 09H
    LEA DX, newLine
    INT 21H
	
	CALL openReadStaffFile
	
readAndCompare:
	resetAX
	MOV AX, [SI]
	CMP AX, 0
	JE invalidInput
	
	CALL readStaffDetails
	getDefaultIDlength:
		MOV DI, 0
		MOV BX, 0
		
		;--Count the length of default User-ID by comparing '$' symbol
		;--Loop the count if wrong symbol else jump to compareTwoIDlength 
		countIDlength:
		CMP Staff.userID[DI],'$'
		JE compareTwoIDLength
		INC DI
		MOV BX, DI
		JMP countIDlength
	
	compareTwoIDLength:
		
		;--Compare the length of inputted User-ID and default User-ID
		;--If equal jump to compareIDinput ， else jump to invalidInput 
		MOV BH, inputID[1]
		CMP BL, BH

		JNE readAndCompare
	
	compareIDinput:
		
		;--Compare User-ID character one by one 
		;--If all true then jump to getPasswordInput , else jump to invalidInput 
		resetCX
		MOV CL, inputID[1]
		MOV BX, 2
		MOV DI, 0
		
		userIdCompareLoop:
			MOV AL, inputID[BX]
			CMP AL, Staff.userID[DI]
			JNE readAndCompare
			
			INC BX
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
		
		CALL closeFile
		JMP getUserIDInput
		
	getPasswordInput:
		;--Prompt for User Password
		MOV AH,09H
		LEA DX,promptPassword
		INT 21H
		
		;--Input User Password
		MOV AH, 0AH
		LEA DX, inputPassword
		INT 21H
		
		;--Nextline
		MOV AH,09H
		LEA DX,newLine
		INT 21H	
	
	getDefaultPasswordLength:
		MOV BX,0
		
		;--Count the length of default Password by comparing '$' symbol
		;--Loop the count if wrong symbol else jump to compareTwoPasswordLength 
		
		countPasswordLength:
		CMP Staff.userPw[BX],'$'
		JE compareTwoPasswordLength
		INC BX
		JMP countPasswordLength
	
	compareTwoPasswordLength:
		;--Compare the length of inputted User Password and default User Password
		;--If equal jump to compareTwoPasswordInput ， else jump to invalidInput 
		
		MOV BH,inputPassword[1]
		CMP BL,BH
		JNE invalidInput
		
	compareTwoPasswordInput:
		MOV CX, 0
		MOV CL, BL
		MOV BX, 2
		MOV DI, 0
		
		userPasswordCompareLoop:
			MOV AL, inputPassword[BX]
			CMP AL, Staff.userPw[DI]
					
			JNE invalidInput
			INC BX
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
