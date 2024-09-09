.model small
.stack 200h 

.data
    filename db "PHONE_IN.txt", 0 
    file_data db 255 dup(0) 
    file_handle dw ?
    newline db 13, 10 
    prompt_model db "Enter phone model: $"
    prompt_ram db "Enter RAM (in GB): $"
    prompt_storage db "Enter storage (in GB): $"
    prompt_color db "Enter color: $"
    prompt_price db "Enter price: $"
    prompt_quantity db "Enter quantity: $"
    prompt_continue db "Do you want to add another phone (y/n)? $"
    separator db ";", 0

.code
start:
    mov ax, @data
    mov ds, ax

    ; Open the file in append mode 
    mov ah, 3dh 
    mov al, 2 ; Open file in read/write mode
    or  al, 1000b ; Set the append flag
    lea dx, filename 
    int 21h

    ; Assuming the file exists, no need to check for carry flag
    mov [file_handle], ax 

    ; Set the file pointer to the end of the file for appending
    mov ah, 42h        
    mov al, 2          
    xor cx, cx         
    xor dx, dx         
    mov bx, [file_handle]
    int 21h

input_loop:
    ; Get phone model
    lea dx, prompt_model
    mov ah, 09h
    int 21h
    call get_input
    call write_to_file

    ; Get RAM
    lea dx, prompt_ram
    mov ah, 09h
    int 21h
    call get_input
    call write_to_file

    ; Get storage
    lea dx, prompt_storage
    mov ah, 09h
    int 21h
    call get_input
    call write_to_file

    ; Get color
    lea dx, prompt_color
    mov ah, 09h
    int 21h
    call get_input
    call write_to_file

    ; Get price
    lea dx, prompt_price
    mov ah, 09h
    int 21h
    call get_input
    call write_to_file

    ; Get quantity
    lea dx, prompt_quantity
    mov ah, 09h
    int 21h
    call get_input
    call write_to_file

    ; Add newline 
    lea dx, newline
    mov ah, 40h
    mov bx, [file_handle]
    mov cx, 2 ; Length of CR+LF
    int 21h

    ; Ask if user wants to add another phone
    mov ah, 09h
    lea dx, prompt_continue
    int 21h
    mov ah, 01h ; Get single character input
    int 21h
    cmp al, 'y'
    je input_loop

    ; Close the file
    mov ah, 3eh 
    mov bx, [file_handle]
    int 21h

    ; Terminate the program
    mov ah, 4ch
    int 21h

get_input proc
    mov byte ptr file_data, 254 ; Max length of input (254 characters)
    lea dx, file_data
    mov ah, 0ah
    int 21h
    ret
get_input endp

update_Qty:
    mov cx, [si + offset_qty]
    add cx, ax
    mov [si + offset_qty], cx

    ; Increase StockInQty
    mov dx, [si + offset_stock_in]
    add dx, bx
    mov [si + offset_stock_in], dx

write_to_file proc
    ; Write user input to file
    mov ah, 40h
    mov bx, [file_handle]
    lea dx, file_data + 2 ; Skip the first two bytes of input buffer
    mov cx, 0
    mov cl, [file_data + 1] ; Length of input
    int 21h

    lea dx, separator
    mov ah, 40h
    mov bx, [file_handle]
    mov cx, 1
    int 21h

write_end:
    ret
write_to_file endp

end start