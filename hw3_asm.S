/*
 * Name:  Casey Grove
 * Email: caseg1@umbc.edu
 * Description: hw3_asm.S takes in two values for multi/div
 *        Performs shift-add/shift-sub and returns product/quot/rem
 */
	.section .text
	.globl uint64_mult
	.globl uint64_div

uint64_mult:
	/* x0 == multiplicand
	   x1 == multiplier */
	mov x2, #0  // x2 == product(high)
	mov x4, #64	// x4 == counter (64bit numbers)

m_loop:	
	cmp x4, #0
	b.eq m_done
	
	// check LSB of lower product
	and x3, x1, #1
	cmp x3, #0
	b.eq m_shr

	// add multiplicand if LSB of lower is 1
	add x2, x2, x0
	
m_shr:	
	lsr x1, x1, #1

	// check LSB of higher product
	and x3, x2, #1
	cmp x3, #0
	b.eq no_gap

	// flip MSB of low x1 if LSB of higher is 1
	bfi x1, x2, #63, #1
	
no_gap:
	lsr x2, x2, #1
	sub x4, x4, #1	//counter--
	b m_loop
	
m_done:
	mov x0, x1
	ret


uint64_div:
	/* Hint: Don't modify x2 throughout this function */
	/* x0 == quotient
	   x1 == divisor */
	mov x3, #0   // x3 == remainder (high)
	mov x4, #64	// x4 == counter (64bit numbers)	

d_loop:
	cmp x4, #0
	b.eq d_done

	lsl x3, x3, #1

	// check MSB of quotient
	and x5, x0, #0x8000000000000000
	cmp x5, #0
	b.eq skip_add

	// add one to remainder if MSB of quotient is 1
	add x3, x3, #1

skip_add:	
	lsl x0, x0, #1

	// check if remainder is greater or equal to divisor
	sub x5, x3, x1
	cmp x5, #-1
	b.le no_keep

	add x0, x0, #1
	mov x3, x5	//change remainder
	
no_keep:
	sub x4, x4, #1	//counter--
	b d_loop

d_done:	
	// supposing x0 has your quotient, you will need
	// something like below to pass it out of this function
	str x3, [x2, #0]
	ret
