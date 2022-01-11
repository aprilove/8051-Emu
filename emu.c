#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 128

typedef unsigned char byte;
typedef unsigned short word;

int* inst;
int cnt = 0;
int space = 1;
int state = 1;

struct CPU {
	// Program counter
	word PC;
	// Stack pointer
	byte SP;

	// Accumulator and B reg
	word A, B;

	// Program status word
	/*
	0. Parity				-> Gives the XOR of A
	1. User defined			-> UD
	2. Overflow flag		-> addition produces a signed overflow
	3. Register select 0	-> The low-order bit of the register bank. Set when
	banks at 0x08 or 0x18 are in use
	4. Register select 1.	-> The high-order bit of the register bank. Set when
	banks at 0x08 or 0x18 are in use
	5. Flag 0.				-> Flag
	6. Auxiliary carry		-> addition produces carry from bit 3 -> 4
	7. Carry bit			-> general register for bit computations
	*/
	byte PSW;

	// Reg
	word R0, R1, R2, R3, R4, R5, R6, R7;
};

// stack
byte stack[STACK_SIZE] = {0};

void execute();

void init(struct CPU c) {
	// Reversed due to reading convenience
	c.PC = 0;
	c.SP = 0;
	c.R0 = 0;
	c.R1 = 0;
	c.R2 = 0;
	c.R3 = 0;
	c.R4 = 0;
	c.R5 = 0;
	c.R6 = 0;
	c.R7 = 0;
	c.A = 0;
	c.B = 0;
}

int main(int argc, char** argv) {
	struct CPU c;
	init(c);

	if (argc < 2) {
		printf("Missing file");
		exit(1);
	}

	FILE* f = fopen(argv[1], "rb");
	if (f == NULL) {
		printf("Cannot open\n");
		exit(1);
	}

	inst[0] = 0x00;
	byte val = 0;
	// Assumes inputted file is legit
	while (!feof(f)) {
		for (int i = 0; i < 8;) {
			val << 1;
			if (fgetc(f) == '0' || fgetc(f) == '1') {
				val += fgetc(f);
				++i;
			}
		}
		inst[cnt] = val;
		++cnt;
		++space;
		inst = realloc(inst, (2 * sizeof(*inst)) * space);
	}

	fclose(f);
	while (state) {
		++c.PC;
		execute(inst[c.PC], c);
	}

	free(inst);

	return 0;
}

// Need to determine how operands are written
void execute(byte instruction, struct CPU c) {
	switch (instruction) {
		// NOP
		case 0x00:
			break;
		// AJMP NOT YET SUPPORTED
		case 0x21:
		case 0x01:
			++c.PC;
			break;
		// LJMP
		case 0x02:
			++c.PC;
			c.PC = inst[c.PC];
			break;
		// RR
		case 0x03:
			if (c.A % 2) {
				c.A >> 1;
				c.A += 0x80;
			} else {
				c.A >> 1;
			}
			break;
		// INC
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
		case 0x0A:
		case 0x0B:
		case 0x0C:
		case 0x0D:
		case 0x0E:

			break;
		// JBC NOT YET SUPPORTED
		case 0x10:
			break;
		// ACALL NOT FULLY IMPLEMENTED
		case 0x11:

			break;
		// LCALL NOT FULLY IMPLEMENTED
		case 0x12:

			break;
		// RRC
		case 0x13:

			break;
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:
		case 0x1A:
		case 0x1B:
		case 0x1C:
		case 0x1D:
		case 0x1E:
		case 0x1F:

			break;
		// RET
		case 0x22:

			break;
		// RL
		case 0x23:

			break;
		// ADD
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x27:
		case 0x28:
		case 0x29:
		case 0x2A:
		case 0x2B:
		case 0x2C:
		case 0x2D:
		case 0x2E:
		case 0x2F:

			break;
		// ADDC
		case 0x34:
		case 0x35:
		case 0x36:
		case 0x37:
		case 0x38:
		case 0x39:
		case 0x3A:
		case 0x3B:
		case 0x3C:
		case 0x3D:
		case 0x3E:
		case 0x3F:

			break;
		// ORL
		case 0x42:
		case 0x43:
		case 0x44:
		case 0x45:
		case 0x46:
		case 0x47:
		case 0x48:
		case 0x49:
		case 0x4A:
		case 0x4B:
		case 0x4C:
		case 0x4D:
		case 0x4E:
		case 0x4F:

			break;
		// ANL
		case 0x52:
		case 0x53:
		case 0x54:
		case 0x55:
		case 0x56:
		case 0x57:
		case 0x58:
		case 0x59:
		case 0x5A:
		case 0x5B:
		case 0x5C:
		case 0x5D:
		case 0x5E:
		case 0x5F:

			break;
		// XRL
		case 0x62:
		case 0x63:
		case 0x64:
		case 0x65:
		case 0x66:
		case 0x67:
		case 0x68:
		case 0x69:
		case 0x6A:
		case 0x6B:
		case 0x6C:
		case 0x6D:
		case 0x6E:
		case 0x6F:

			break;
		// SUBB
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9A:
		case 0x9B:
		case 0x9C:
		case 0x9D:
		case 0x9E:
		case 0x9F:

			break;
		// XCH NOT YET SUPPORTED
		// MOV NOT FULLY IMPLEMNETED
		case 0x74:
			break;
		// CLR
		case 0xC2:

			break;
		case 0xE4:

			break;
		default:
			printf("Not instruction: 0x%02X", inst);
			break;
	}
}