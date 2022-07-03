bits 16
switch_to_pm:
    cli                       ; disable interrupts
    lgdt [gdt_descriptor]     ; load the gdt
    mov eax, cr0              ; control register to 32 bit mode
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_pm      ; far jump to the pm code

bits 32
init_pm:
    mov ax, DATA_SEG
    mov ds, ax                 ; nullify segmentation
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000           ; set the stack
    mov esp, ebp

    call begin_pm