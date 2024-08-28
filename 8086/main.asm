.MODEL SMALL
.STACK 100
.DATA 
    choice DB ?
	loginCheck DB 0
    newLine DB 0DH,0AH,"$"    ; Newline definition
	invalidInputMsg db "Invalid input. Please Enter a valid input.", 0dh, 0ah, "$"                       
	MSG1 db "STOCK IN$"
	MSG2 db "STOCK OUT$"
	MSG3 db "SEARCH$"
	MSG4 db "GENERATE$"
	MSG5 db "EXIT$"

.CODE

;-- INCLUDE the menu file
INCLUDE Fs\menu.inc
INCLUDE Fs\search.inc
INCLUDE Fs\display.inc
INCLUDE Simon\clnScr.inc
INCLUDE Simon\login.inc
INCLUDE Kh\report.inc

MAIN PROC 
    MOV AX,@DATA
    MOV DS,AX	
	
	CALL readStockFile
	
	MOV AX, 4C00H
    INT 21H

loginLoop:
	CALL user_login
	
menuLoop:

    CALL user_Menu    ; Call the user menu function
	
	CALL clear_Screen
	
    ; Go To Choice
    CMP choice, 1
    JE stockIn
    CMP choice, 2
    JE stockOut
    CMP choice, 3
    JE searchStock
    CMP choice, 4
    JE generateReport
    CMP choice, 5
    JE exitProgram

	
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
	searchLoop:
	call searchPhoneStock
		
	CALL cmpString
	
	CALL continueSearch
	
	CMP choice, 1
	JE searchLoop
	CMP choice, 2
	JE menuLoop
	CMP choice, 3
	JE menuLoop		
	
	call clear_Screen

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
