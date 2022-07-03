print:
    pusha          ; save all registers
start:
    mov al, [bx]   ; get the next byte of the string
    cmp al, 0      ; if it's zero, we're done
    je done
    mov ah, 0x0e   ; print in text mode
    int 0x10       ; print the next byte
    add bx, 1      ; move to the next byte
    jmp start
done:
    popa           ; restore all registers
    ret

print_nl:
    pusha          ; save all registers
    mov ah, 0x0e   ; print in text mode
    mov al, 0x0a   ; print a newline
    int 0x10       ; print the newline
    mov al, 0x0d   ; print a carriage return
    int 0x10       ; print the carriage return
    popa           ; restore all registers
    ret