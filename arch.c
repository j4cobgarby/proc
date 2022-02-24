#include <stdio.h>
#include <stdint.h>

typedef struct {
	uint16_t pc; // Program Counter
	uint16_t ir; // Instruction Register
	uint16_t a; // Accumulator
	uint16_t b; // B General Purpose (And address for memory access)
	uint16_t c; // C General Purpose
	uint16_t d; // D General Purpose
	uint16_t e; // E General Purpose (And address for GOTO)
	uint16_t sp; // Stack Pointer
	uint16_t sb; // Stack Base
	uint16_t status; // Status
} registers_t;

registers_t r = {.pc=0,.a=0,.b=0,.c=0,.d=0,.e=0,.sp=32768,.sb=32768,.status=0};

//uint16_t M[32768] = {0x000c, 0x0002, 0x020f, 0x0004, 0x030f, 0x0002, 0x3203, 0x000c, 0x0007};
uint16_t M[32768] = {0x030f, 0x0001, 0x040f, 0x0001, 0x4303, 0x430a, 0x320a, 0x000c, 0x0004};

#define STATUS_ZERO 	1 << 0
#define STATUS_CARRY	1 << 1
#define STATUS_SIGN	1 << 2
#define STATUS_PARITY	1 << 3
#define STATUS_OVERFLOW	1 << 4

int evaluate_conds(uint16_t c, uint16_t status) {
	// For each bit of cond, if it's set then the corresponding status bit
	// must be set too
	return ~c | status;
}

int step() {
	r.ir = M[r.pc++];
	if (r.ir == 0x0000) {}
	if (r.ir == 0x0001) {r.a = M[r.b];}
	if (r.ir == 0x0002) {M[r.b] = r.a;}
	if ((r.ir & 0x000f) == 0x3) {
		uint16_t *r1 = ((uint16_t*)&r)+((r.ir&0x0f00)>>8);
		uint16_t *r2 = ((uint16_t*)&r)+((r.ir&0xf000)>>12);
		r.a = *r1 + *r2;
	}
	if ((r.ir & 0x000f) == 0x4) {
		uint16_t *r1 = ((uint16_t*)&r)+((r.ir&0x0f00)>>8);
		uint16_t *r2 = ((uint16_t*)&r)+((r.ir&0xf000)>>12);
		r.a = *r1 - *r2;
	}
	if ((r.ir & 0x000f) == 0x5) {
		uint16_t *r1 = ((uint16_t*)&r)+((r.ir&0x0f00)>>8);
		uint16_t *r2 = ((uint16_t*)&r)+((r.ir&0xf000)>>12);
		r.a = *r1 | *r2;
	}
	if ((r.ir & 0x000f) == 0x6) {
		uint16_t *r1 = ((uint16_t*)&r)+((r.ir&0x0f00)>>8);
		uint16_t *r2 = ((uint16_t*)&r)+((r.ir&0xf000)>>12);
		r.a = *r1 & *r2;
	}
	if ((r.ir & 0x000f) == 0x7) {
		uint16_t *r1 = ((uint16_t*)&r)+((r.ir&0x0f00)>>8);
		uint16_t *r2 = ((uint16_t*)&r)+((r.ir&0xf000)>>12);
		r.a = *r1 ^ *r2;
	}
	if ((r.ir & 0x000f) == 0x8) {
		uint16_t *r1 = ((uint16_t*)&r)+((r.ir&0x0f00)>>8);
		uint16_t *r2 = ((uint16_t*)&r)+((r.ir&0xf000)>>12);
		r.a = *r1 << *r2;
	}
	if ((r.ir & 0x000f) == 0x9) {
		uint16_t *r1 = ((uint16_t*)&r)+((r.ir&0x0f00)>>8);
		uint16_t *r2 = ((uint16_t*)&r)+((r.ir&0xf000)>>12);
		r.a = *r1 >> *r2;
	}
	if ((r.ir & 0x000f) == 0xa) {
		uint16_t *r1 = ((uint16_t*)&r)+((r.ir&0x0f00)>>8);
		uint16_t *r2 = ((uint16_t*)&r)+((r.ir&0xf000)>>12);
		*r2 = *r1;
	}
	if ((r.ir & 0x000f) == 0xb) {
		uint16_t cond = (r.ir & 0xf0) >> 4;
		if (evaluate_conds(cond, r.status)) r.pc = r.e;
	}
	if ((r.ir & 0x000f) == 0xc) {
		uint16_t cond = (r.ir & 0xf0) >> 4;
		if (evaluate_conds(cond, r.status)) r.pc = M[r.pc];
	}
	if ((r.ir & 0x000f) == 0xd) {
		M[r.sp--] = r.a;
	}
	if ((r.ir & 0x000f) == 0xe) {
		M[++r.sp] = r.a;
	}
	if ((r.ir & 0x000f) == 0xf) {
		uint16_t *r1 = ((uint16_t*)&r)+((r.ir&0x0f00)>>8);
		*r1 = M[r.pc++];
	}
}

int main(int argc, char**argv) {
	printf("Processor\n");
	while (1) {
		getchar();
		step();
		printf("PC %d IR %04x, A B C D E: %d %d %d %d %d", r.pc, r.ir, r.a, r.b, r.c, r.d, r.e);
	}
}
