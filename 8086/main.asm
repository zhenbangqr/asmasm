.MODEL SMALL
.STACK 100
.DATA 
	ten8Bits DB 10
    choice DB ?
	searchCheck DB 0
	numStockFound DB 0
	loginCheck DB 0
	eofHolder DW ?
    newLine DB 0DH,0AH,"$"    ; Newline definition
	invalidInputMsg db "Invalid input. Please Enter a valid input.", 0dh, 0ah, "$"  	
	MSG1 db "STOCK IN$"
	MSG2 db "STOCK OUT$"
	MSG3 db "SEARCH$"
	MSG4 db "GENERATE$"
	MSG5 db "EXIT$"

	phone STRUC
		phoneName DB 20 DUP('$')
		phoneRAM DB 20 DUP('$')
		phoneROM DB 20 DUP('$')
		phoneColor DB 20 DUP('$')
		phonePrice DW 1234
		phonePriceFP DW 50
		phoneQty DW ?
	phone ENDS
	
	Stock phone<>

.CODE

;-- INCLUDE the menu file
INCLUDE Fs\menu.inc
INCLUDE Fs\search.inc
INCLUDE Fs\read.inc
INCLUDE Fs\display.inc
INCLUDE Simon\utils.inc
INCLUDE Simon\login.inc
INCLUDE Kh\report.inc
INCLUDE zb\stokin.inc

MAIN PROC 
    MOV AX,@DATA
    MOV DS,AX

loginLoop:
	CALL user_login
	
menuLoop:
	
    CALL user_Menu    ; Call the user menu function
	
    ; Go To Choice
    CMP choice, 1
    JE stockIn
    CMP choice, 2
    JE stockOut
    CMP choice, 3
    JE searchStock
    CMP choice, 4
    JE generateReport
	
    JMP exitProgram

stockIn:
    ; Code for Stock In
	MOV AH,09H
    LEA DX, MSG1
    INT 21H
	
	MOV AH, 09H
    LEA DX, newLine
    INT 21H
	
	JMP menuLoop

stockOut:
    ; Code for Stock Out
	MOV AH,09H
    LEA DX, MSG2
    INT 21H
	
	MOV AH, 09H
    LEA DX, newLine
    INT 21H
	
    JMP menuLoop

searchStock:
    ; Code for Search Stock
	searchStockLoop:
		MOV numStockFound, 0
		CALL searchMenu
		
		CMP choice, 2
		JE menuLoop
		
		call clear_Screen 
		
		CALL openReadStockFile
		MOV searchCheck, 0
		CALL searchPhoneStock
		JMP compareStringLoop

	
	compareStringLoop:
		CALL readStockDetails

		MOV searchCheck, 0
		CALL cmpString
		CMP searchCheck, 1
		JE displayResult

		MOV AX, [SI]
		CMP AX, 0
		JE EndDisplayResult
		
		JMP compareStringLoop
	
	displayResult:
		INC numStockFound
		CALL displayStockDetails
		
		MOV AX, [SI]
		CMP AX, 0
		JE EndDisplayResult
		
		JMP compareStringLoop
		
	EndDisplayResult:
		CALL closeStockFile
		JMP searchStockLoop
		
generateReport:
    ; Code for Generate Report
	
	MOV AH,09H
    LEA DX, MSG4
    INT 21H
	
	MOV AH, 09H
    LEA DX, newLine
    INT 21H
	
	CALL reportMenu
	
    JMP menuLoop

exitProgram:
	MOV AH,09H
    LEA DX, MSG5
    INT 21H
	
	MOV AH, 09H
    LEA DX, newLine
    INT 21H
    
	MOV AX, 4C00H
    INT 21H

MAIN ENDP
END MAIN
