.DATA
	searchPrompt DB "Please enter phone name: $"
	searchSucMsg DB "Here is your search results. Please select your next step. $"
	searchFailMsg DB "No result found. Please select your next step. $"
	nextStepPrompt DB "1. Continue Search", 0DH, 0AH, "2. View Details", 0DH, 0AH, "3. Exit Search", 0DH, 0AH, "Please Enter Your Choice : $"
	searchInput LABEL BYTE
	strLength DB 30
	strActNum DB ?
	searchStr DB 30 DUP("$") 
	str DB "Tiong$"
	arrNum DW ?
.CODE
searchPhoneStock PROC
	searchLoop:
	
	MOV AH, 09H
	LEA DX, searchPrompt
	INT 21H

	MOV AH, 0AH
	LEA DX, searchInput
	INT 21H
	
	;New Line
	MOV AH, 09H
	LEA DX, newLine
	INT 21H
	
	MOV AH, 09H
	LEA DX, searchStr
	INT 21H
	
	;New Line
	MOV AH, 09H
	LEA DX, newLine
	INT 21H
	
	CALL cmpString
	
	CALL continueSearch
	
	CMP choice, 1
	JE searchLoop
	CMP choice, 2
	JE ExitSearchLoop
	CMP choice, 3
	JE ExitSearchLoop
		
	ExitSearchLoop:
		RET
		
searchPhoneStock ENDP



cmpString PROC
	MOV DI, 0
	JMP compare
	
compare:
	MOV arrNum, DI
	MOV SI, 2
	MOV CX, 0
	MOV CL, searchInput[1]
	
	compareLoop:
		MOV BL, str[DI]
		CMP BL, "$"
		JE failSearch
		
		MOV AH, 02H
		MOV DL, str[DI]
		INT 21H
		
		MOV AH, 02H
		MOV DL, searchInput[SI]
		INT 21H
				
		CMP BL, searchInput[SI]
		JNE failCompare
		
		INC SI
		INC DI
	LOOP compareLoop
	
	MOV AH, 09H
	LEA DX, searchSucMsg
	INT 21H
	JMP finish

failCompare:
	INC arrNum
	MOV DI, arrNum
	JMP compare
	
	
failSearch:
	MOV AH, 09H
	LEA DX, searchFailMsg
	INT 21H
	JMP finish
	
finish:
	RET
cmpString ENDP



continueSearch PROC

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
	RET
	
invalidInputChoice:
	MOV AH, 09H
	LEA DX, invalidInputMsg
	INT 21H
	
	MOV AH, 09H
	LEA DX, newLine
	INT 21H
	
	JMP nextStepChoiceLoop

continueSearch ENDP	
	
	