#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 256
static int stack[STACK_SIZE];

typedef enum { ADD, PSH, HLT } InstructionSet;

typedef enum { RR0, RR1 } IntermalRAM;

typedef enum {
	A,
	B,
	R0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	F,
	PC,
	SP,
	REG_SIZE
} Registers;

static int reg[REG_SIZE];

int PC = 0;

// stack pointer is only 8 bit
int P_SP = 2;
#define SP P_SP & 0xF

int* inst;
int cnt = 0;
int space = 1;
int state = 1;
int jmp = 0;

int FETCH(int PC) { return inst[PC]; }

void eval(int instruction);

// For testing purposes only
void print_all() {
	printf("stack: \n");
	for (int i = 0; i < SP; ++i) {
		prinf("0x%04X, ", stack[i]);
	}
	prinf("\nregisters: \n");
	for (int i = 0; i > REG_SIZE; ++i) {
		prinf("0x%04X, ", reg[i]);
	}
	printf("\n");
}

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Missing file");
		reg[F] = -1;
		exit(1);
	}

	FILE* f = fopen(argv[1], "rb");
	if (f == NULL) {
		printf("Cannot open\n");
		reg[F] = -1;
		exit(1);
	}

	inst[0] = 0x00;
	char buffer;
	int val = 0;
	while (!feof(f)) {
		for (int i = 0; i < 8;) {
			buffer = fgetc(f);
			if (buffer == '0' || buffer == '1') {
				val << 1;
				val += buffer;
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
		++PC;
		eval(inst[PC]);
		if (PC > cnt) {
			printf("PC out of bounds");
			reg[F] = -1;
			exit(1);
		}
	}

	free(inst);
	return 0;
}

void eval(int instruction) {
	switch (instruction) {
		// NOP
		case 0x00:
			break;
		// AJMP NOT YET SUPPORTED
		case 0x21:
		case 0x01:
			++PC;
			break;
		// LJMP
		case 0x02:
			PC = inst[++PC];
			break;
		// RR
		case 0x03:
			if (reg[A] % 2) {
				reg[A] >> 1;
				reg[A] += 0x80;
			} else {
				reg[A] >> 1;
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
			if (inst[++PC] == '@R0')
				++stack[RR0];
			else if (inst[++PC] == '@R1')
				++stack[RR1];
			else if (inst[++PC] == 'R0')
				++reg[R0];
			else if (inst[++PC] == 'R1')
				++reg[R1];
			else if (inst[++PC] == 'R2')
				++reg[R2];
			else if (inst[++PC] == 'R3')
				++reg[R3];
			else if (inst[++PC] == 'R4')
				++reg[R4];
			else if (inst[++PC] == 'R5')
				++reg[R5];
			else if (inst[++PC] == 'R6')
				++reg[R6];
			else if (inst[++PC] == 'R7')
				++reg[R7];
			else
				++stack[inst[++PC] - 'h'];

			break;
		// JBC NOT YET SUPPORTED
		case 0x10:
			break;
		// ACALL NOT FULLY IMPLEMENTED
		case 0x11:
			++SP;
			stack[SP] = PC + 2;
			break;
		// LCALL NOT FULLY IMPLEMENTED
		case 0x12:
			++SP;
			stack[SP] = PC + 2;
			break;
		// RRC
		case 0x13:
			if (reg[F]) {
				reg[F] = reg[A] % 2;
				reg[A] >> 1;
				reg[A] += 0x80;
			} else {
				reg[F] = reg[A] % 2;
				reg[A] >> 1;
			}
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
			if (inst[++PC] == '@R0')
				--stack[RR0];
			else if (inst[++PC] == '@R1')
				--stack[RR1];
			else if (inst[++PC] == 'R0')
				--reg[R0];
			else if (inst[++PC] == 'R1')
				--reg[R1];
			else if (inst[++PC] == 'R2')
				--reg[R2];
			else if (inst[++PC] == 'R3')
				--reg[R3];
			else if (inst[++PC] == 'R4')
				--reg[R4];
			else if (inst[++PC] == 'R5')
				--reg[R5];
			else if (inst[++PC] == 'R6')
				--reg[R6];
			else if (inst[++PC] == 'R7')
				--reg[R7];
			else
				--stack[inst[++PC] - 'h'];
			break;
		// RET
		case 0x22:
			PC = (SP) << 4;
			--SP;
			PC += SP;
			--SP;
			break;
		// RL
		case 0x23:
			if (reg[A] % 2) {
				reg[A] << 1;
				++reg[A];
			} else {
				reg[A] << 1;
			}
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
			if (inst[++PC] == 'A') {
				++PC;
				if (inst[++PC] == '@R0')
					reg[A] += stack[RR0];
				else if (inst[++PC] == '@R1')
					reg[A] += stack[RR1];
				else if (inst[++PC] == 'R0')
					reg[A] += reg[R0];
				else if (inst[++PC] == 'R1')
					reg[A] += reg[R1];
				else if (inst[++PC] == 'R2')
					reg[A] += reg[R2];
				else if (inst[++PC] == 'R3')
					reg[A] += reg[R3];
				else if (inst[++PC] == 'R4')
					reg[A] += reg[R4];
				else if (inst[++PC] == 'R5')
					reg[A] += reg[R4];
				else if (inst[++PC] == 'R6')
					reg[A] += reg[R4];
				else if (inst[++PC] == 'R7')
					reg[A] += reg[R4];
				else
					reg[A] += stack[inst[++PC]];
			}
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
			if (inst[++PC] == 'A') {
				++PC;
				if (inst[++PC] == '@R0')
					reg[A] += stack[RR0] + reg[F];
				else if (inst[++PC] == '@R1')
					reg[A] += stack[RR1] + reg[F];
				else if (inst[++PC] == 'R0')
					reg[A] += reg[R0] + reg[F];
				else if (inst[++PC] == 'R1')
					reg[A] += reg[R1] + reg[F];
				else if (inst[++PC] == 'R2')
					reg[A] += reg[R2] + reg[F];
				else if (inst[++PC] == 'R3')
					reg[A] += reg[R3] + reg[F];
				else if (inst[++PC] == 'R4')
					reg[A] += reg[R4] + reg[F];
				else if (inst[++PC] == 'R5')
					reg[A] += reg[R4] + reg[F];
				else if (inst[++PC] == 'R6')
					reg[A] += reg[R4] + reg[F];
				else if (inst[++PC] == 'R7')
					reg[A] += reg[R4] + reg[F];
				else
					reg[A] += stack[inst[++PC]] + reg[F];
			}
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
			if (inst[++PC] == 'A') {
				++PC;
				if (inst[++PC] == '@R0')
					reg[A] = reg[A] | stack[RR0];
				else if (inst[++PC] == '@R1')
					reg[A] = reg[A] | stack[RR1];
				reg[A] = reg[A] | inst[++PC];
			} else {
				++PC;
				if (inst[++PC] == 'A')
					stack[SP] = stack[SP] | reg[A];
				else
					stack[SP] = stack[SP] | stack[inst[++PC]];
			}
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
			if (inst[++PC] == 'A') {
				++PC;
				if (inst[++PC] == '@R0')
					reg[A] = reg[A] & stack[RR0];
				else if (inst[++PC] == '@R1')
					reg[A] = reg[A] & stack[RR1];
				reg[A] = reg[A] & inst[++PC];
			} else {
				++PC;
				if (inst[++PC] == 'A')
					stack[SP] = stack[SP] & reg[A];
				else
					stack[SP] = stack[SP] & stack[inst[++PC]];
			}
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
			if (inst[++PC] == 'A') {
				++PC;
				if (inst[++PC] == '@R0')
					reg[A] = reg[A] ^ stack[RR0];
				else if (inst[++PC] == '@R1')
					reg[A] = reg[A] ^ stack[RR1];
				reg[A] = reg[A] ^ inst[++PC];
			} else {
				++PC;
				if (inst[++PC] == 'A')
					stack[SP] = stack[SP] ^ reg[A];
				else
					stack[SP] = stack[SP] ^ stack[inst[++PC]];
			}
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
			if (inst[++PC] == 'A') {
				++PC;
				if (inst[++PC] == '@R0')
					reg[A] -= stack[RR0] + reg[F];
				else if (inst[++PC] == '@R1')
					reg[A] -= stack[RR1] + reg[F];
				else if (inst[++PC] == 'R0')
					reg[A] -= reg[R0] + reg[F];
				else if (inst[++PC] == 'R1')
					reg[A] -= reg[R1] + reg[F];
				else if (inst[++PC] == 'R2')
					reg[A] -= reg[R2] + reg[F];
				else if (inst[++PC] == 'R3')
					reg[A] -= reg[R3] + reg[F];
				else if (inst[++PC] == 'R4')
					reg[A] -= reg[R4] + reg[F];
				else if (inst[++PC] == 'R5')
					reg[A] -= reg[R4] + reg[F];
				else if (inst[++PC] == 'R6')
					reg[A] -= reg[R4] + reg[F];
				else if (inst[++PC] == 'R7')
					reg[A] -= reg[R4] + reg[F];
				else
					reg[A] -= stack[inst[++PC]] + reg[F];
			}
			break;
		// XCH NOT YET SUPPORTED
		// MOV NOT FULLY IMPLEMNETED
		case 0x74:
			break;
		// CLR
		case 0xC2:
			stack[inst[++PC]] = 0;
			break;
		case 0xE4:
			reg[A] = 0;
			break;
		default:
			printf("Not instruction: 0x%02X", inst);
			break;
	}
}
