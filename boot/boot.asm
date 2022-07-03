org 0x7c00

KERNEL_OFFSET equ 0x1000       ; offset of kernel in memory

    mov [BOOT_DRIVE], dl       ; boot from the correct drive
    mov bp, 0x9000             ; set the stack
    mov sp, bp

    mov bx, MSG_INIT
    call print
    call print_nl

    call load_kernel           ; read the kernel from the disk
    call video_init            ; go to 13h video mode
    call switch_to_pm          ; go to 32 bit mode (jumps to begin_pm)
    jmp $                      ; never executes

%include "boot/print.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/switch_pm.asm"

bits 16
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print
    call print_nl

    mov bx, KERNEL_OFFSET
    mov dh, 32                 ; load enough sectors
    mov dl, [BOOT_DRIVE]       ; boot from the correct drive
    call disk_load             ; load kernel to 0x1000
    ret

video_init:                    ; go to graphics mode
    mov ah, 0
    mov al, 0x13
    int 0x10
    ret

bits 32
begin_pm:
    call KERNEL_OFFSET         ; give control to the kernel
    jmp $                      ; hang if kernel returns


BOOT_DRIVE db 0
MSG_INIT db "Starting up", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0

times 510 - ($-$$) db 0        ; boot signature
dw 0xaa55