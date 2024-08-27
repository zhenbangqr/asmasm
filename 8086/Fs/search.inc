.DATA
	searchPrompt DB "Please enter phone name: $"
	searchSucMsg DB "Here is your search results. Please select your next step. $"
	searchFailMsg DB "No result found. Please select your next step. $"
	nextStepPrompt DB "1. Continue Search", 0DH, 0AH, "2. View Details", 0DH, 0AH, "3. Exit Search", 0DH, 0AH, "Please Enter Your Choice : $"
	searchInput DB 20 DUP(?)
.CODE
searchPhoneStock PROC
	searchLoop:
	
	MOV AH, 09H
	LEA DX, searchPrompt
	INT 21H
	
	MOV BYTE PTR [searchInput], 20  ; Set maximum length to 20
	MOV BYTE PTR [searchInput+1], 0	; Initialize actual length to 0

	MOV AH, 0AH
	LEA DX, searchInput
	INT 21H
	
	;New Line
	MOV AH, 09H
	LEA DX, newLine
	INT 21H

	nextStepChoiceLoop:
		;New Line
		MOV AH, 09H
		LEA DX, newLine
		INT 21H
		
		MOV AH, 09H
		LEA DX, nextStepPrompt
		INT 21H
		
		MOV AH, 01H
		INT 21H
		SUB AL,30H
		
		MOV AH, 09H
		LEA DX, newLine
		INT 21H
		
		CMP AL, 1
		JB invalidInputChoice
		CMP AL, 3
		JA invalidInputChoice
		
		MOV choice, AL
		
		CMP choice, 1
		JE searchLoop
		CMP choice, 2
		JE ExitSearchLoop
		CMP choice, 3
		JE ExitSearchLoop
		
	invalidInputChoice:
		MOV AH, 09H
		LEA DX, invalidInputMsg
		INT 21H
		
		MOV AH, 09H
		LEA DX, newLine
		INT 21H
		
		JMP nextStepChoiceLoop
		
	ExitSearchLoop:
		RET
		
searchPhoneStock ENDP
	
	
	