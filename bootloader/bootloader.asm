[ORG 0x00]
[BITS 16]

SECTION .text

JMP 0x07C0:START

START:
    mov ax, 0x07c0
    mov ds, ax
    mov ax, 0xB800
    mov es, ax

    mov si, 0

.CLEARSCREEN:
    mov byte[es:si], 0
    mov byte[es:si+1], 0x0A
    add si, 2
    cmp si, 80 * 25 * 2
    jl .CLEARSCREEN

    mov si, 0
    mov di, 0

.PRINTMSG:
    mov cl, byte[si + LOADERSTARTMSG]
    cmp cl, 0
    je .MSGEND
    mov byte[es:di], cl
    add si, 1
    add di, 2
    jmp .PRINTMSG

.MSGEND:
    JMP $

LOADERSTARTMSG: DB "KAKAOS Boot Loader"

TIMES 510 - ($ - $$) DB 0x00
DB 0x55
DB 0xAA
