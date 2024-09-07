.DATA
	divider db "=============================================================================$"
	

;------------
;This file defines multiple MACROS to reduce code repetitions 

;------- MACRO start here

;--Macro to reset a AX register 
resetAX MACRO
		
		MOV AX,0
		
ENDM


;--Macro to reset a BX register 
resetBX MACRO
		
		MOV BX,0
	
ENDM

;--Macro to reset a CX register 
resetCX MACRO
		
		MOV CX,0
	
ENDM

;--Macro to reset a DX register 
resetDX MACRO
		
		MOV DX,0
	
ENDM

;--Macro to clear the screen
clnScr MACRO
	  
    MOV AH, 06H    ; Scroll up window
    MOV AL, 00H    ; Scroll entire window
    MOV BH, 07H    ; Set attribute (light gray on black)
    MOV CH, 00H    ; Upper-left row coordinate
    MOV CL, 00H    ; Upper-left column coordinate
    MOV DH, 24H    ; Lower-right row coordinate
    MOV DL, 79H    ; Lower-right column coordinate
    INT 10H        ; BIOS video interrupt

    ; Set cursor at the top-left corner
    MOV AH, 02H    ; Set cursor position
    MOV BH, 00H    ; Page number (usually 0)
    MOV DH, 00H    ; Row coordinate
    MOV DL, 00H    ; Column coordinate
    INT 10H        ; BIOS video interrupt
	
ENDM

;--Macro to print a newline
print_newLine MACRO

    MOV AH, 09H
    LEA DX, newLine
    INT 21H

ENDM
	
;--Macro to print divider
displayDivider MACRO
	
	MOV AH,09H
	LEA DX,divider
	INT 21H
	
ENDM

rptDivider MACRO

	MOV AH,09H
	LEA DX,reportDivider
	INT 21H
	
ENDM

invDivider MACRO

	MOV AH,09H
	LEA DX,invStatusDivider
	INT 21H
	
ENDM

;--Macro to print delimiter
delimiter MACRO
	
	MOV AH,02H
	MOV DL,"|"
	INT 21H
	
ENDM

;--Macro to pause the screen
pause MACRO
	
	MOV AH,01H
    INT 21H
	
ENDM

;--Macro to pause the screen
space MACRO
	
	MOV AH, 02H
	MOV DL, ' '
    INT 21H
	
ENDM

