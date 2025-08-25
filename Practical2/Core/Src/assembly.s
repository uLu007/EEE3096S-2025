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

main_loop:
	LDR R3, [R0, #0x10]		@ read GPIOA IDR (button presses)

check_sw2:
	LSRS R4, R3, #2			@check sw2
	BSC check_sw3			@skip if not pressed
	MOVS R2, #0xAA
	B write_leds

check_sw3:
	LSRS R4, R3, #3 		@check sw3
	BSC normal				@skip if not pressed
	B write_leds

normal:
	LSRS R4, R3, #0 		@check sw0
	BSC inc_by_1			@if not pressed, inc by 1
	ADDS R2, R2, #2			@else increase by 2
	B delay

inc_by_1:
	ADDS R2, R2, #1

@============DELAYS
delay:
	LSRS R4, R3, #1			@check sw1
	BSC long_delay			@if not pressed, use long delay
	BL short_delay			@else use short
	B write_leds

long_delay:
	LDR R5, LONG_DELAY_CNT
long_loop:
	SUBS R5, R5, #1
	BNE long_loop
	BX LR

short_delay:
	LDR R5, SHORT_DELAY_CNT
short_loop:
	SUBS R5, R5, #1
	BNE short_loop
	BX LR

@============WRITE LEDS
write_leds:
	STR R2, [R1, #0x14]
	B main_loop

@ LITERALS; DO NOT EDIT
	.align
RCC_BASE: 			.word 0x40021000
AHBENR_GPIOAB: 		.word 0b1100000000000000000
GPIOA_BASE:  		.word 0x48000000
GPIOB_BASE:  		.word 0x48000400
MODER_OUTPUT: 		.word 0x5555

@ TODO: Add your own values for these delays
LONG_DELAY_CNT: 	.word 2800000
SHORT_DELAY_CNT: 	.word 1200000
