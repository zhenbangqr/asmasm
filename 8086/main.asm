.MODEL SMALL
.STACK 100
.DATA 
	ten8Bits DB 10
	ten16Bits DW 10
	hundred16bits DW 100
    choice DB ?
	searchCheck DB 0
	numStockFound DB 0
	digitValidate DB 1
    newLine DB 0DH,0AH,"$"    ; Newline definition
	invalidInputMsg db "Invalid input. Please Enter a valid input.", 0dh, 0ah, "$"  	
	MSG1 db "Enter stock in quantity: $"
	MSG2 db "STOCK OUT$"
	MSG3 db "SEARCH$"
	MSG4 db "GENERATE$"
	MSG5 db "EXIT$"

	user STRUC
		userID DB 20 DUP(?)
		userPw DB 20 DUP(?)
	user ENDS
	
	Staff user<>

	phone STRUC
		phoneName DB 20 DUP(?)
		phoneRAM DB 20 DUP(?)
		phoneROM DB 20 DUP(?)
		phoneColor DB 20 DUP(?)
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
INCLUDE Fs\write.inc
INCLUDE Simon\login.inc
INCLUDE Kh\repMenu.inc
INCLUDE Kh\report.inc

MAIN PROC 
    MOV AX,@DATA
    MOV DS,AX

mainPageLoop:
	CALL loginPage
	
	CMP choice, 1
	JE loginLoop
	
	JMP exitProgram
	
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
	
    JMP logOut

generateReport:
    ; Code for Generate Report
	CALL reportMenu
	
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
		
		CALL inputAndSearch
		JMP searchStockLoop

logOut:
	CALL logOutComfirm
	
	CMP choice, 2
	JE menuLoop
	JMP mainPageLoop

stockIn:
    ; Code for Stock In
	CALL openWriteStockFile
	CALL writeStockFileTitle
	
	CALL openReadStockFile
	CALL jumpToNextLine
	stockInProcess:
		CALL clearStockVariableBuffer
		CALL readStockDetails
		
	enterStockInQty:
		;MOV AH,09H
		;LEA DX, MSG1
		;INT 21H
	
		;CALL inputQuantity
		
		;CMP digitValidate, 0
		;JE invalidQtyInput
		
		;resetAX
		;resetDX
		;MOV DI, OFFSET numberInputed
		;CALL stringToNumber
		MOV AX, 55
		MOV DX, 0
		MOV WORD PTR Stock.totalStockInQty, AX
		MOV WORD PTR Stock.totalStockInQty + 2, DX

	CALL appendToBuffer
	
	MOV AL, [SI]
	CMP AX, 0
	JE endStockIn
	
	JMP stockInProcess
	
	endStockIn:
	CALL closeFile
	CALL closeWriteFile
	;CALL switchStockFileName   ;havent done yet
	JMP menuLoop
	
	InvalidQtyInput:
		MOV AH, 09H
		LEA DX, invalidInputMsg
		INT 21H
    
		MOV AH, 09H
		LEA DX, newLine
		INT 21H
		
		pause
		clnScr
		
		JMP enterStockInQty

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
