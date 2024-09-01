.model small
.stack 200h ; Increased stack size

.data
    filename db "phone_inventory.txt", 0 
    file_data db "POCO F5;16GB;256GB;BLUE;3000.00;20", 10
               db "VIVO Y2;16GB;128GB;WHITE;3000.00;19", 10
               db "VIVO Y3;16GB;128GB;WHITE;3000.00;18", 10
               db "VIVO Y4;16GB;128GB;WHITE;3000.00;17", 10
               db "VIVO Y5;16GB;128GB;WHITE;3000.00;16", 10
               db "VIVO Y6;16GB;128GB;WHITE;3000.00;15", 10
               db "VIVO Y7;16GB;128GB;WHITE;3000.00;14", 10
               db "VIVO Y8;16GB;128GB;WHITE;3000.00;13", 10
               db "VIVO Y9;16GB;128GB;WHITE;3000.00;12", 10
               db "VIVO Y10;16GB;128GB;WHITE;3000.00;11", 10, '$' 
    file_data_end: 
    file_handle dw ?

.code
start:
    mov ax, @data
    mov ds, ax

    ; Create or truncate the file for writing
    mov ah, 3ch     
    mov cx, 0      
    lea dx, filename 
    int 21h       

    jc error      

    mov [file_handle], ax 

    ; Write the data to the file
    mov ah, 40h    
    mov bx, [file_handle]
    mov cx, file_data_end - file_data 
    lea dx, file_data 
    int 21h

    jc error

    ; Close the file
    mov ah, 3eh    
    mov bx, [file_handle]
    int 21h

    ; Terminate the program
    mov ah, 4ch
    int 21h

error:
    ; Handle errors (add your error handling code here)
    ; For now, just terminate with a non-zero exit code
    mov ah, 4ch
    mov al, 1
    int 21h
    
end start