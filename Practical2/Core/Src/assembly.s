/*
 * assembly.s
 *
 */
 
 @ DO NOT EDIT
	.syntax unified
    .text
    .global ASM_Main
    .thumb_func

@ DO NOT EDIT
vectors:
	.word 0x20002000
	.word ASM_Main + 1

@ DO NOT EDIT label ASM_Main
ASM_Main:

	@ Some code is given below for you to start with
	LDR R0, RCC_BASE  		@ Enable clock for GPIOA and B by setting bit 17 and 18 in RCC_AHBENR
	LDR R1, [R0, #0x14]
	LDR R2, AHBENR_GPIOAB	@ AHBENR_GPIOAB is defined under LITERALS at the end of the code
	ORRS R1, R1, R2
	STR R1, [R0, #0x14]

	LDR R0, GPIOA_BASE		@ Enable pull-up resistors for pushbuttons
	MOVS R1, #0b01010101
	STR R1, [R0, #0x0C]
	LDR R1, GPIOB_BASE  	@ Set pins connected to LEDs to outputs
	LDR R2, MODER_OUTPUT
	STR R2, [R1, #0]
	MOVS R2, #0         	@ NOTE: R2 will be dedicated to holding the value on the LEDs

@ TODO: Add code, labels and logic for button checks and LED patterns
	MOVS R7, #0 	@LED bitmask
	LDR R6, LONG_DELAY_CNT
	LDR R5, SORT_DELAY_CNT

main_loop:
	LDR R3, GPIOA_BASE		@ read GPIOA IDR (button presses)
	LDR R6, [R0, #0x10]		@GPIOA -> IDR

@========check button presses=========
check_sw2:	@check switch 2
	LDR R4, sw2
	TST R1, R4
	BEQ fixed_pattern

check_sw3:	@check switch 3
	LDR R4, sw3
	TST R1, R4
	BEQ freeze

check_sw0:	@check switch 0
	LDR R4, sw0
	TST R1, R4
	BEQ inc_by_2

check_sw1:	@check switch 1
	LDR R4, sw1
	TST R1, R4
	BEQ short

check_sw0_and_sw1: 	@check switch 0 and 1
	LDR R4, sw0_sw1
	TST R1, R4
	BEQ sw0_and_sw1
===========================

	CMP R6, #0 				@initiate delay
	BNE long_delay
	LDR R6, LONG_DELAY_CNT 	@reset delay counter
	ADDS R3 ,R3 , #1
	CMP R3, #255
	BGT reset_bit					@reset R3
	MOVS R2, R3
	B write_leds

sw0_and_sw1:
	CMP R5, #0 				@initiate delay
	BNE short_delay
	LDR R5, SHORT_DELAY_CNT
	ADDS R3, R3, #2
	CMP R3, #255
	BGT reset_bit
	MOVS R2, R3
	B write_leds

@====switch 0 held: inc by 2
inc_by_2:
	CMP R6, #0 				@initiate delay
	BNE long_delay
	LDR R6, LONG_DELAY_CNT
	ADDS R3, R3, #2
	CMP R3, #255
	BGT reset_bit
	MOVS R2, R3
	B write_leds

@======switch 1 held: short delay
short:
	CMP R5, #0
	BNE short_delay
	LDR R5, SHORT_DELAY_CNT
	ADDS R3, R3, #1
	CMP R3, #255
	BGT reset_bit
	MOVS R2, R3
	B write_leds

@======switch 2 held: 0xAA
fixed_pattern:
	MOVS R2, 0xAA
	MOVS R3, 0xAA
	B write_leds

@======switch 3 held: freeze
freeze:
	MOVS R2, R3
	B main_loop

@===========WRITE LEDS
write_leds:
	LDR R1, GPIOB_BASE
	STR R2, [R1, #0x14]
	B main_loop

@=========Reset counts and LEDs
reset_bit:
	MOVS R3, #0
	MOVS R4, #0
	MOVES R2, R3
	B write_leds

@=====DELAYS
long_delay:
	SUBS R6, #1
	BNE long_delay
	B main_loop

short_delay:
	SUBS R5, #1
	BNE short_delay
	B main_loop


@ LITERALS; DO NOT EDIT
	.align
RCC_BASE: 			.word 0x40021000
AHBENR_GPIOAB: 		.word 0b1100000000000000000
GPIOA_BASE:  		.word 0x48000000
GPIOB_BASE:  		.word 0x48000400
MODER_OUTPUT: 		.word 0x5555

@ TODO: Add your own values for these delays
LONG_DELAY_CNT: 	.word 1400000
SHORT_DELAY_CNT: 	.word 600000

@==switches
sw0:				.word	0b1
sw1:				.word	0b10
sw2:				.word	0b100
sw3:				.word	0b1000
sw0_sw1:			.word	0b11

