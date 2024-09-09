.DATA 
	
	
.CODE
StockOut PROC

	getPhoneNameLength:
		MOV DI,OFFSET Stock.phoneName
		CALL calculateStringLength
		JMP compareTwoPhoneNameLength
	
	compareTwoPhoneNameLength:
		MOV BH,searchInput[1]
		CMP BL,BH
		JNE inputAndSearch
	
	phoneNameCompareLoop:
		resetCX
		MOV CL,BL
		MOV BX,2
		MOV DI,0
		
		MOV AL,searchInput[BX]
		CMP AL,Stock.phoneName[DI]
		JNE failCompare
		
		INC BX
		INC DI
	LOOP phoneNameCompareLoop
		
StockOut ENDP

CompareStock