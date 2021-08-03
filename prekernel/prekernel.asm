[ORG 0X00]
[BITS 16]

SECTION .TEXT

JMP 0X1000:START

SECTORCOUNT: DW 0X0000
IMAGESIZE EQU 1024 ; Size of Operating System

START:
    MOV AX, CS
    MOV DS, AX
    MOV AX, 0XB800
    MOV ES, AX

    %assign i 0
    %rep IMAGESIZE
        %assign i i+1

        MOV AX, 2
        MUL WORD[SECTORCOUNT]
        MOV SI, AX

        MOV BYTE[ES:SI+(80 * 2 * 4)], '0' + (i % 10)
        ADD WORD[SECTORCOUNT], 1

        %if i == IMAGESIZE
            JMP $
        %else
            JMP (0X1000 + i * 0X20): 0X0000
        %endif

        TIMES (512 - ( $ - $$ ) % 512) DB 0X00
    %endrep
