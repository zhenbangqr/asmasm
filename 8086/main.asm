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
    newLine DB 0DH, 0AH, "$"    ; Newline definition
	invalidInputMsg DB "Invalid input. Please Enter a valid input.", 0DH, 0AH, "$"  	
	exitMessage DB "EXIT", 0DH, 0AH, "$"
	
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
INCLUDE Simon\stockOut.inc
INCLUDE Zb\stockIn.inc
INCLUDE Kh\repMenu.inc
INCLUDE Kh\report.inc

MAIN PROC

    MOV AX,@DATA
    MOV DS,AX
	
	clnScr

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
		MOV numStockFound, 0
		CALL reportMenu
		
		JMP menuLoop
	
	stockOut:
		MOV numStockFound, 0
		CALL stockOutMenu
		
		CMP choice, 2
		JE menuLoop
		
		CALL compareAndStockOut
		CALL closeWriteFile
		CALL clearReadFileBuffer
		JMP stockOut
	
	searchStock:
		MOV numStockFound, 0
		CALL searchMenu
		
		CMP choice, 2
		JE menuLoop
		
		CALL inputAndSearch
		CALL closeReadFile
		CALL clearReadFileBuffer
		JMP searchStock
	
	logOut:
		CALL logOutComfirm
		
		CMP choice, 2
		JE menuLoop
		JMP mainPageLoop
	
	stockIn:
		MOV numStockFound, 0
		CALL stockInMenu
		
		CMP choice, 2
		JE menuLoop
		
		CALL compareAndStockIn
		CALL closeWriteFile
		CALL clearReadFileBuffer
		JMP stockIn
	
	exitProgram:
		MOV AH,09H
		LEA DX, exitMessage
		INT 21H
		
		MOV AX, 4C00H
		INT 21H
		
MAIN ENDP
END MAIN
