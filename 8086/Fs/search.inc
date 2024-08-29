.DATA
	searchPrompt DB "Please enter phone name: $"
	searchSucMsg DB "Here is your search results. Please select your next step. $"
	searchFailMsg DB "No result found. Please select your next step. $"
	nextStepPrompt DB "1. Search stock", 0DH, 0AH, "2. Exit Search", 0DH, 0AH, "Please Enter Your Choice : $"
	searchInput LABEL BYTE
	strLength DB 30
	strActNum DB ?
	searchStr DB 30 DUP("$") 
	str DB "Tiong$"
	arrNum DW ?
.CODE

;--------------------------------------------------------------
;	continueSearch
;	Ask user whether want to continue search the phone or not
;--------------------------------------------------------------
searchMenu PROC

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
	CMP AL, 2
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

searchMenu ENDP	


;----------------------------------------------
;	searchPhoneStock
;	Enter the phone name to search the stock
;----------------------------------------------
searchPhoneStock PROC
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
	
	RET
		
searchPhoneStock ENDP


;--------------------------------------------------------------
;	cmpString
;	compare the string read from file with the entered string
;--------------------------------------------------------------
cmpString PROC
	MOV DI, 0
	JMP compare
	
compare:
	MOV arrNum, DI
	MOV CX, 0
	MOV CL, searchInput[1]
	
	compareLoop:
		MOV BL, Stock.phoneName[DI]
		CMP BL, "$"
		JE failSearch
				
		CMP BL, searchInput[SI]
		JNE failCompare
		
		INC SI
		INC DI
	LOOP compareLoop
	
	MOV AH, 09H
	MOV searchCheck, 1
	INT 21H
	JMP finish

failCompare:
	INC arrNum
	MOV DI, arrNum
	JMP compare
	
failSearch:
	MOV AH, 09H
	INT 21H
	JMP finish
	
finish:
	RET
cmpString ENDP


;-----------------------------------------
;	displayStockDetails
;	display the Stock Details
;-----------------------------------------
displayStockDetails PROC
	MOV AH, 09H
	LEA DX, Stock.phoneName
	INT 21H
	
	MOV AH, 09H
	LEA DX, Stock.phoneRAM
	INT 21H
	
	MOV AH, 09H
	LEA DX, Stock.phoneROM
	INT 21H
	
	MOV AH, 09H
	LEA DX, Stock.phoneColor
	INT 21H
	
	MOV AH,09H
    LEA DX,newLine
    INT 21H
	
	RET
displayStockDetails ENDP
	
	