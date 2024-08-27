;------------
;This function allow screen clearing by moving the line upwards and set the cursor to top lef

;--INT 10H Function 06 -------------
; AH 	  = Screen Clearing function;-------------
; AL 	  = Number of lines to scroll (with AL=00, window will be cleared)
; BH 	  = Video attribute of blank rows
; CH, CL  = Row,Column of upper left corner
; DH, DL  = Row,Column of lower right corner
; INT 10H = Invoke the interrupt

;--INT 10H Function 02 ----------
; AH 	  = Set cursor position function;------------
; BH 	  = Video page number (typically 0 for text mode)
; DH	  = Row number (0 for the top row)
; DL 	  = Column number (0 for the leftmost column)
; INT 10H = Invoke the interrupt


;------- CODE start here
.CODE
clear_Screen PROC
	
	; Clear the screen 
	MOV AH, 06H    
	MOV AL, 00H    
	MOV BH, 07H    
	MOV CH, 00H    
	MOV CL, 00H    
	MOV DH, 24H    
	MOV DL, 79H    
	INT 10H        
	
	; Set cursor at the top-left corner
	MOV AH, 02H    
	MOV BH, 00H    
	MOV DH, 01H    
	MOV DL, 00H    
	INT 10H        

	RET

clear_Screen ENDP