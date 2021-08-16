[ORG 0X00]
[BITS 16]

SECTION .TEXT

START:
    MOV AX, 0X1000
    MOV DS, AX
    MOV ES, AX

    ; Activate A20 Gate using BIOS service.
    MOV AX, 0X2401
    INT 0X15

    JC .A20GATE_ERROR
    JMP .A20GATE_SUCCESS

.A20GATE_ERROR:
    IN AL, 0X92
    OR AL, 0X02
    AND AL, 0XFE
    OUT 0X92, AL


.A20GATE_SUCCESS:
    CLI
    LGDT [GDTR]
    ; PG=0, CD=1, NW=0, AM=0, WP=0, 
    ; NE=1, ET=1, TS=1, EM=0, MP=1, PE=1
    MOV EAX, 0X4000003B
    MOV CR0, EAX
    JMP DWORD 0X18: (PROTECTEDMODE - $$ + 0X10000)

[BITS 32]
PROTECTEDMODE:
    MOV AX, 0X20
    MOV DS, AX
    MOV ES, AX
    MOV FS, AX
    MOV GS, AX

    MOV SS, AX
    MOV ESP, 0XFFFE
    MOV EBP, 0XFFFE

    PUSH (SWITCHSUCCESSMSG - $$ + 0X10000)
    PUSH 0
    PUSH 4
    CALL PRINTMESSAGE
    ADD ESP, 12

    JMP DWORD 0x18: 0x10200

PRINTMESSAGE:
    PUSH EBP
    MOV EBP, ESP
    PUSH ESI
    PUSH EDI
    PUSH EAX
    PUSH ECX
    PUSH EDX

    MOV EAX, DWORD[EBP + 8]
    MOV ESI, 80 * 2
    MUL ESI
    MOV EDI, EAX

    MOV EAX, DWORD[EBP + 12]
    MOV ESI, 2
    MUL ESI
    ADD EDI, EAX

    MOV ESI, DWORD[EBP + 16]

.MESSAGELOOP:
    MOV CL, BYTE[ESI]
    CMP CL, 0
    JE .MESSAGEEND
    MOV BYTE[EDI + 0XB8000], CL
    ADD ESI, 1
    ADD EDI, 2

    JMP .MESSAGELOOP

.MESSAGEEND:
    POP EDX
    POP ECX
    POP EAX
    POP EDI
    POP ESI
    POP EBP
    RET

ALIGN 8, DB 0


DW 0X0000

GDTR:
    DW GDTEND - GDT - 1
    DD (GDT - $$ + 0X10000)

GDT:
    NULL_DESCRIPTOR:
        DW 0X0000
        DW 0X0000
        DB 0X00
        DB 0X00
        DB 0X00
        DB 0X00

    AMD64CODEDESCRIPTOR:     
        dw 0xFFFF
        dw 0x0000
        db 0x00
        db 0x9A
        db 0xAF
        db 0x00

    AMD64DATADESCRIPTOR:
        dw 0xFFFF
        dw 0x0000
        db 0x00
        db 0x92
        db 0xAF
        db 0x00

    CODE_DESCRIPTOR:
        DW 0XFFFF
        DW 0X0000
        DB 0X00
        DB 0X9A
        DB 0XCF
        DB 0X00
    
    DATA_DESCRIPTOR:
        DW 0XFFFF
        DW 0X0000
        DB 0X00
        DB 0X92
        DB 0XCF
        DB 0X00
    
GDTEND:

SWITCHSUCCESSMSG: DB "Switched to protected mode.", 0

times 512 - ($ - $$) db 0x00
