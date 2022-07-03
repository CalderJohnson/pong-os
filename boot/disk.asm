disk_load:
    pusha                      ; save registers
    push dx

    mov ah, 0x02               ; int 13h: read
    mov al, dh                 ; number of sectors to read, provided by the caller
    mov cl, 0x02               ; sector 2...n
    mov ch, 0x00               ; 
    mov dh, 0x00               ; head number
    int 0x13                   ; load
    jc disk_error              ; error, stored in the carry bit

    pop dx
    cmp al, dh
    jne sectors_error          ; al is set to the number of sectors read. if it doesn't match the number of sectors requested, error
    popa                       ; restore registers
    ret

disk_error:
    mov bx, DISK_ERROR
    call print
    call print_nl
    jmp disk_loop

sectors_error:
    mov bx, SECTORS_ERROR
    call print
    call print_nl

disk_loop:
    jmp $

DISK_ERROR db "Disk read error", 0
SECTORS_ERROR db "Incorrect number of sectors read", 0