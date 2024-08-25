.MODEL SMALL
.STACK 100
.DATA 

.CODE

;-- INCLUDE the file path here to call function from different file  
INCLUDE Fs\menu.inc

MAIN PROC 
	MOV AX,@DATA
	MOV DS,AX

	CALL user_Menu ;--- CALLING USER MENU FUNCTION
	
	MOV AX,4C00H
	INT 21H

MAIN ENDP
END MAIN
	