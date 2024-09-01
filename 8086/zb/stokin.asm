.model small
.stack 100h

.data
    filename db 'stock_data.txt', 0  ; Null-terminated filename
    file_handle dw ?                  ; Variable to store the file handle
    buffer db 255 dup(0)              ; Buffer to store read data (adjust size if needed)
    bytes_read dw ?                   ; Variable to store the number of bytes read

.code
main proc
    mov ax, @data                    ; Initialize data segment
    mov ds, ax

    call read_stock_data_from_file   ; Call the stock reading function

    ; ... (Process the read data in 'buffer')

    mov ax, 4C00h                    ; Terminate the program
    int 21h

main endp

; Function to read stock data from a file (DOS)
read_stock_data_from_file proc

    ; 1. Open the file
    mov ah, 3Dh         ; Function 3Dh: Open file
    mov al, 0            ; Open mode: Read-only
    lea dx, filename    ; DS:DX points to the filename string
    int 21h             ; Execute the DOS interrupt

    ; Check if the file opened successfully
    jc open_error       ; Jump to error handling if carry flag is set

    mov [file_handle], ax ; Store the file handle

    ; 2. Read from the file
    mov ah, 3Fh         ; Function 3Fh: Read from file
    mov bx, [file_handle] ; File handle
    mov cx, 255         ; Maximum number of bytes to read (adjust if needed)
    lea dx, buffer       ; DS:DX points to the buffer
    int 21h             ; Execute the DOS interrupt

    ; Check if the read operation was successful
    jc read_error       ; Jump to error handling if carry flag is set

    mov [bytes_read], ax ; Store the number of bytes actually read

    ; 3. Close the file
    mov ah, 3Eh         ; Function 3Eh: Close file
    mov bx, [file_handle] ; File handle
    int 21h             ; Execute the DOS interrupt

    ; Check if the close operation was successful
    jc close_error      ; Jump to error handling if carry flag is set

    ret                 ; Return from the function

open_error:
    ; ... (Handle file open error)
    ret

read_error:
    ; ... (Handle file read error)
    ret

close_error:
    ; ... (Handle file close error)
    ret

read_stock_data_from_file endp

end main