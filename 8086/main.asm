.MODEL SMALL
.STACK 100
.DATA 
    choiceMenu DB ?
    newLine DB 0DH,0AH,"$"    ; Newline definition
	MSG1 db "STOCK IN$"
	MSG2 db "STOCK OUT$"
	MSG3 db "SEARCH$"
	MSG4 db "GENERATE$"
	MSG5 db "EXIT$"

.CODE

;-- INCLUDE the menu file
INCLUDE Fs\menu.inc
INCLUDE Simon\clnScr.inc

MAIN PROC 
    MOV AX,@DATA
    MOV DS,AX

menuLoop:
    CALL user_Menu    ; Call the user menu function
	
	CALL clear_Screen
	
    ; Go To Choice
    CMP choiceMenu, 1
    JE stockIn
    CMP choiceMenu, 2
    JE stockOut
    CMP choiceMenu, 3
    JE searchStock
    CMP choiceMenu, 4
    JE generateReport
    CMP choiceMenu, 5
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
	
	MOV AH,09H
    LEA DX, MSG3
    INT 21H
	
	MOV AH, 09H
    LEA DX, newLine
    INT 21H
	
    JMP menuLoop

generateReport:
    ; Code for Generate Report
	
	MOV AH,09H
    LEA DX, MSG4
    INT 21H
	
	MOV AH, 09H
    LEA DX, newLine
    INT 21H
	
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
