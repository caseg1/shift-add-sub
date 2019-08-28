/*
 * Name:  Casey Grove
 * Email: caseg1@umbc.edu
 * Description: hw3.c calls mult and div functions from hw3_asm.S
 *        Parses floats and prints out bit information
 * Best case multi: 774 cycles (1Ret, 3Mov, 64Bnot, 64Sub, 128And, 128Lsr, 193Cmp, 193Btake)
 * Worst case mult: 902 cycles (Best case multi + 64Add, 64Bfi)
 * Best case divid: 710 cycles (1Str, 1Ret, 2Mov, 63Bnot, 64And, 128Lsl, 128Sub, 193Btake, 193Cmp)
 * Worst case divi: 902 cycles (Best case div + 128Add, 64Mov, 128Bnot (-128Btake))
 */

#define _POSIX_C_SOURCE 200112L

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Perform unsigned integer multiplication of @a m1 and @a m2, then
 * return the product.
 *
 * You have your choice of whatever algorithm to implement. If the
 * product cannot fit in 64-bits, then truncate result to lower
 * 64-bits.
 *
 * SPECIAL RESTRICTION: you may only use addition, subtraction,
 * shifts, rotate, compare, and branch operations. You <strong>may
 * not</strong> use any built-in multiplication instructions for this
 * assignment.
 * @param[in] m1 Multiplicand
 * @param[in] m2 Multiplier
 * @return Product of @m1 times @m2, truncated to lower 64-bits.
 */
extern uint64_t uint64_mult(uint64_t m1, uint64_t m2);

/**
 * Perform unsigned division of @a d1 and @a d2, then return the
 * quotient and remainder.
 * You have your choice of whatever algorithm to implement.
 *
 * SPECIAL RESTRICTION: you may only use addition, subtraction,
 * shifts, rotate, compare, and branch operations. You <strong>may
 * not</strong> use any built-in division instructions for this
 * assignment.
 * 
 * @param[in] d1 Dividend
 * @param[in] d2 Divisor
 * @param[out] quot Remainder
 * @return Quotient of @d1 divided by @d2.
 */
extern uint64_t uint64_div(uint64_t d1, uint64_t d2, uint64_t const *rem);

/**
 * Given a double, break down its IEEE-754 bitwise representation.
 * @param[in] val 64-bit floating point value to analyze.
 */
static void float64_parse(uint64_t val)
{
	double d = *((double *)&(val));
	printf("For the value %g (bit pattern 0x%016" PRIx64 "):\n", d, val);

	unsigned int expBias = 1023; // for 64bit values
	unsigned int signBit = (val & 0x8000000000000000) ? 1 : 0;
	uint64_t exponent = (val & 0x7FF0000000000000) >> 52;
	uint64_t significand = val & 0xFFFFFFFFFFFFF;
	signed int actMag = exponent - expBias + !signBit;
	
	printf("  Sign bit: %u\n", signBit);
	printf("  Exponent bits: %" PRIu64 " (actual magnitude: %i)\n", exponent, actMag);
	printf("  Significand: 0x%013" PRIx64 "\n", significand);

	/* Special Values */
	if (signBit == 1 && significand == 0)     { printf("     * negative zero\n"); };
	if (exponent == 0 && significand != 0)    { printf("     * denormalized\n");  };
	if (exponent == 2047 && significand == 0) { printf("     * infinity\n");      };
	if (exponent == 2047 && significand != 0) { printf("     * not a number\n");  };       
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Need 2 arguments for this program\n");
		exit(EXIT_FAILURE);
	}

	char *endptr;
	unsigned long long arg1 = strtoull(argv[1], &endptr, 0);
	if (!(*(argv[1])) || *endptr) {
		fprintf(stderr, "Argument 1 not a number: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	unsigned long long arg2 = strtoull(argv[2], &endptr, 0);
	if (!*(argv[2]) || *endptr) {
		fprintf(stderr, "Argument 2 not a number: %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}
	uint64_t i1 = (uint64_t) arg1;
	uint64_t i2 = (uint64_t) arg2;
	uint64_t retval1, retval2 = 0;
	retval1 = uint64_mult(i1, i2);
	printf("Calculated multiplication product: %" PRIu64 "\n", retval1);
	printf("  (Correct answer = %" PRIu64 ")\n", i1 * i2);

	retval1 = uint64_div(i1, i2, &retval2);
	printf("Division quotient: %" PRIu64 ", remainder %" PRIu64 "\n",
	       retval1, retval2);
	uint64_t quot = i1 / i2;
	uint64_t rem = i1 - (i1 / i2) * i2;
	printf("  (Correct answer = q %" PRIu64 ", r %" PRIu64 ")\n", quot,
	       rem);

	float64_parse(i1);
	float64_parse(i2);
	return 0;
}
