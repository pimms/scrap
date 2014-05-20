#include "Bytecode.h"


namespace scrap {

// len_literal is only defined for instructions
// ipush, dpush, fpush, cpush, bpush, lpush
static unsigned len_literal(byte instruction)
{
	switch (instruction) {
		case OP_I_PUSH:
			return 4;
		case OP_D_PUSH:
			return 8;
		case OP_F_PUSH:
			return 4;
		case OP_L_PUSH:
			return 8;
		case OP_C_PUSH:
			return 1;
		case OP_B_PUSH:
			return 1;
	}

	THROW(InvalidArgumentException,
		"len_literal is only defined for the instructions OP_X_PUSH");
}

unsigned arglen(const InstructionInfo *info)
{
	switch (info->argtype) {
		case ARG_NONE:
			return 0;
		case ARG_LITERAL:
			return len_literal(info->instr);
		case ARG_REGISTER:
			return 4;
		case ARG_IDX_OR_REG:
			return 5;
		case ARG_ID:
			return 4;
		case ARG_ID_ID:
			return 8;
		case ARG_TYPE:
			return 5;
	}
}

}
