.MODEL SMALL
.STACK 100
.DATA 
	ten8Bits DB 10
	ten16Bits DW 10
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

	user STRUC
		userID DB 20 DUP('$')
		userPw DB 20 DUP('$')
	user ENDS
	
	Staff user<>

	phone STRUC
		phoneName DB 20 DUP('$')
		phoneRAM DB 20 DUP('$')
		phoneROM DB 20 DUP('$')
		phoneColor DB 20 DUP('$')
		phonePrice DD ?
		phonePriceFP DD ?
		phoneQty DD ?
		totalStockInQty DD ?
		totalStockOutQty DD ?
	phone ENDS
	
	Stock phone<>
	
.CODE

;-- INCLUDE the menu file
INCLUDE Simon\utils.inc
INCLUDE Fs\menu.inc
INCLUDE Fs\search.inc
INCLUDE Fs\read.inc
INCLUDE Fs\display.inc
INCLUDE Simon\login.inc
INCLUDE Kh\repMenu.inc
INCLUDE Kh\report.inc

MAIN PROC 
    MOV AX,@DATA
    MOV DS,AX

loginLoop:
	CALL loginPage
	
	CMP choice, 2
	JE exitProgram
	
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
	
    JMP logOut


logOut:
	CALL logOutComfirm
	
	CMP choice, 2
	JE menuLoop
	JMP loginLoop

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

generateReport:
    ; Code for Generate Report
	CALL reportMenu
	
    JMP menuLoop

searchStock:
    ; Code for Search Stock
	searchStockLoop:
		MOV numStockFound, 0
		CALL searchMenu
		
		CMP choice, 2
		JE menuLoop
		
		CALL inputAndSearch
		
		CALL closeFile
		JMP searchStockLoop


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
