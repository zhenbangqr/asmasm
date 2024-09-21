.DATA 
	loginTitle DB "========================================", 0DH, 0AH, "==                Login               ==", 0DH, 0AH, "========================================", 0DH, 0AH, "$"
	logoutTitle DB "========================================", 0DH, 0AH, "==                Logout              ==", 0DH, 0AH, "========================================", 0DH, 0AH, "$"
	promptLoginID DB "Enter user-ID : $"
	promptPassword DB "Password : $"
	
	welcomeTitle DB "=================================================", 0DH, 0AH, "==     Welcome to FS Phone Inventory System    ==", 0DH, 0AH, "=================================================", 0DH, 0AH, "$"
	loginComfirmation DB "Do you want to Log In?", 0DH, 0AH, "1. Login", 0DH, 0AH, "2. Exit", 0DH, 0AH, "Enter your choice(1 or 2): $"
	logOutComfirmation DB "Do you confirm want to Log Out?", 0DH, 0AH, "1. Yes", 0DH, 0AH, "2. No", 0DH, 0AH, "Enter your choice(1 or 2): $"
	
	inputID LABEL BYTE
	maxUserIdLength DB 20
	getUserIdLength DB ?
	inputUserID DB 20 DUP("$")
	
	inputPassword LABEL BYTE
	maxUserPasswordLength DB 20
	getUserPasswordLength DB ?
	inputUserPassword DB 20 DUP("$")
	
	loginFailMsg DB "Invalid User-ID or Password, please try again.$"
	loginSucMsg DB "Login successfully$"
	
.CODE

loginPage PROC

	loginComfirmationLoop:
		MOV AH, 09H
		LEA DX, welcomeTitle
		INT 21H
		
		MOV AH, 09H
		LEA DX, loginComfirmation
		INT 21H
		
		resetAX
		MOV AH, 01H
		INT 21H
		SUB AL, 30H
		
		print_NewLine
		
		CMP AL, 1
		JB invalidLoginChoice
		CMP AL, 2
		JA invalidLoginChoice
		
		MOV choice, AL
		clnScr
		RET
		
	invalidLoginChoice:
		MOV AH, 09H
		LEA DX, invalidInputMsg
		INT 21H
		
		MOV AH, 09H
		LEA DX, newLine
		INT 21H
		
		pause
		clnScr
		
		JMP loginComfirmationLoop
		
loginPage ENDP

user_Login PROC
	getUserIDInput:
		;--Display the login title
		MOV AH,09H
		LEA DX,loginTitle
		INT 21H
		
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
			MOV DI, OFFSET Staff.userID
			CALL calculateStringLength
			JMP compareTwoIDLength
		
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
			MOV CL, BL
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
			
			print_NewLine
			
			;--Display invalid ID message
			MOV AH,09H
			LEA DX,loginFailMsg
			INT 21H
		
			print_NewLine
			
			CALL closeReadFile
			
			pause
			clnScr
			
			JMP getUserIDInput
		
		pwNotEql:
			JMP readAndCompare
		
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
			print_NewLine
		
		getDefaultPasswordLength:
			LEA DI,Staff.userID ;LEA DI,Staff.userID
			CALL calculateStringLength
			JMP compareTwoPasswordLength
		
		compareTwoPasswordLength:
			;--Compare the length of inputted User Password and default User Password
			;--If equal jump to compareTwoPasswordInput ， else jump to invalidInput 
			
			MOV BH,inputPassword[1]
			CMP BL,BH
			JNE pwNotEql
			
		compareTwoPasswordInput:
			MOV CX, 0
			MOV CL, BL
			MOV BX, 2
			MOV DI, 0
			
			userPasswordCompareLoop:
				MOV AL, inputPassword[BX]
				CMP AL, Staff.userPw[DI]
						
				JNE pwNotEql
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
			
			pause
			clnScr
			
			RET
		
user_Login ENDP




logOutComfirm PROC

	logOutComfirmationLoop:
		MOV AH, 09H
		LEA DX, logoutTitle
		INT 21H
		
		MOV AH, 09H
		LEA DX, logOutComfirmation
		INT 21H
	
		resetAX
		MOV AH, 01H
		INT 21H
		SUB AL, 30H
		
		print_NewLine
	
		CMP AL, 1
		JB invalidLogOutChoice
		CMP AL, 2
		JA invalidLogOutChoice
		
		MOV choice, AL
		
		clnScr
		
		RET
	
	invalidLogOutChoice:
		MOV AH, 09H
		LEA DX, invalidInputMsg
		INT 21H
		
		MOV AH, 09H
		LEA DX, newLine
		INT 21H
		
		pause
		clnScr
		
		JMP logOutComfirmationLoop

logOutComfirm ENDP