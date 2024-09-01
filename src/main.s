.gba
.thumb
.open "./BPRE0.gba","./multi.gba", 0x08000000
.loadtable "./charmap.txt"

.org 0x813B8C2 ; disable help system buttons, otherwise that would pop up instead on on_key_l and on_key_r
.byte 0x1D 
.byte 0xE0
.pool

.org 0x806F282
.thumb
.align 2
LDR r0, =switch_menu|1
BX r0
.pool

.org freespace
.importobj "./build/linked.o"

;super   equ 0x030030F0
choicex equ 0x0203ADE4

switch_menu:
	LDR r0, =hijack|1 ;C code
	BL goto_r0
	CMP r0, #0 ;hijack returns 1 if the input has been handled, 0 otherwise.
	BEQ handle_input_normally
goto_0806F358:
	LDR r0, =0x0806F358|1
	BX r0

handle_input_normally:
	LDR r0, =super
	LDRH r1, [r0, 0x2E]
	LDR r0, =0x40
	AND r0, r1
	CMP r0, #0
	BEQ goto_0806F2D2
	LDR r0, =0x0806F28E|1
	BX r0

goto_0806F2D2:
	LDR r0, =0x0806F2D2|1
goto_r0:
	BX r0

.align 2
menu_text:
.string 0x00,"Menu option\nDue",0xFF

instructions:
.string " ",0x79,0x7A,": select stat     A: confirm",0xFE,0x7B,0x7C,": change EV      B: exit",0xFF
confirm_str:
.string " Confirm      Back",0xFF

.thumb
.align 2
check_money: ; stolen from script 0x92 check_money, at 0x0806C1A2
    push    {r1-R4, LR}
    ldr     R4, =0x020370D0 ; var_800D
    ldr     R0, =0x03005008 ; saveblock1_mapdata
    ldr     R0, [R0]
    ldr     R1, =0x290
    add     R0, R0, R1
    mov     R1, R2
    ldr     R3, =0x0809FD88|1
    bl      goto_r3
    lsl     R0, R0, #0x18
    lsr     R0, R0, #0x18
    strh    R0, [R4]
    mov     R0, #0
    pop     {r1-r4, r0}
    bx      R0

goto_r3:
	bx r3

.pool

.org 0x83DEF2C
.thumb
.align 2
.word gui_1_new_gui_by_item|1;
.pool
.close
