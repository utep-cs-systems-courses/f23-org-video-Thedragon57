	.arch msp430g2553
	.p2align 1,0


	.extern P1OUT
    .extern P1DIR
    .extern GREEN
    .text

    .global led
led: nop
    bis 00010000, P1DIR
    bis 00010000, P1OUT
    ret
