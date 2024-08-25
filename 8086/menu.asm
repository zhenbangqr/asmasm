.model small
.stack 100
.data
	menuTitle db "========================================", 0dh, 0ah, "==       Phone Inventory System       ==", 0dh, 0ah, "========================================", 0dh,0ah, "$"
	menuChoice db "1. Stock In", 0dh, 0ah, "2. Stock Out", 0dh, 0ah, "3. Search Stock", 0dh, 0ah, "4. Generate Stock Report", 0dh, 0ah, "5. Exit", 0dh, 0ah, "$"
	promptChoice db "Enter a digit(1-5): $"
	errMsgChoice db "Invalid input. Please Enter a valid digit(1-5).", 0dh, 0ah, "$"
	sucMsgChoice db "Your choice is $"
	newLine db 0dh,0ah,"$"
	choice db ?

.code
main proc
	mov ax, @data
	mov ds, ax
	
choiceLoop:
	;Show Menu Title
	mov ah, 09h
    lea dx, menuTitle
    int 21h	

	;Show Menu Choice
	mov ah, 09h
    lea dx, menuChoice
    int 21h

	;Enter Choice
	mov ah, 09h
	lea dx, promptChoice
	int 21h

	mov ax, 0
	mov ah, 01h
	int 21h
	sub al, 30h
	
	;New Line
	mov ah, 09h
	lea dx, newLine
	int 21h
	
	;Input Validate
	cmp al, 1
	jb invalidChoice
	cmp al, 5
	ja invalidChoice
	
	
	mov choice, al
	
	;Show Success Message(can delete after this)
	mov ah, 09h
	lea dx, sucMsgChoice
	int 21h
	
	mov ah, 02h
	mov dl, choice
	add dl, 30h
	int 21h
	
	mov ah, 09h
	lea dx, newLine
	int 21h

	;Go To Choice
    cmp choice, 1
    je stockIn
    cmp choice, 2
    je stockOut
    cmp choice, 3
    je searchStock
    cmp choice, 4
    je generateReport
    cmp choice, 5
    je exitProgram
	
	
invalidChoice:
	mov ah, 09h
	lea dx, errMsgChoice
	int 21h
	
	mov ah, 09h
	lea dx, newLine
	int 21h
	
	jmp choiceLoop

stockIn:
    ; Code for Stock In 	
    jmp choiceLoop

stockOut:
    ; Code for Stock Out
    jmp choiceLoop

searchStock:
    ; Code for Search Stock
    jmp choiceLoop

generateReport:
    ; Code for Generate Report
    jmp choiceLoop

exitProgram:
    ; Exit the program
    mov ax, 4C00h
    int 21h
	
main endp
end main
	
	
	