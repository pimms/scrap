#pragma once
#include "Scrap.h"


// 		MNEMONIC 			VALUE	ARGS		DESCRIPTION
#define OP_POP				0x00 //				Pop and discard top value
#define OP_COPY 			0x01 //				Copy top value
#define OP_RETURN 			0x04 // 			Return 

// Stack management 
#define OP_I_LOAD 			0x20 // LIT
#define OP_I_RETURN 		0x21 //
#define OP_I_STORE			0x22 //	LIT
#define OP_I_PUSH			0x23 // LIT			Push a literal value of type INT

#define OP_F_LOAD 			0x30 // LIT
#define OP_F_RETURN 		0x31 //
#define OP_F_STORE			0x32 //	LIT
#define OP_F_PUSH			0x33 // LIT	

#define OP_D_LOAD 			0x40 // LIT
#define OP_D_RETURN 		0x41 //
#define OP_D_STORE			0x42 //	LIT
#define OP_D_PUSH			0x43 // LIT	

#define OP_L_LOAD 			0x50 // LIT
#define OP_L_RETURN 		0x51 //
#define OP_L_STORE			0x52 //	LIT
#define OP_L_PUSH			0x53 // LIT	

#define OP_C_LOAD 			0x60 // LIT
#define OP_C_RETURN 		0x61 //
#define OP_C_STORE			0x62 //	LIT
#define OP_C_PUSH			0x63 // LIT	

#define OP_B_LOAD 			0x70 // LIT
#define OP_B_RETURN 		0x71 //
#define OP_B_STORE			0x72 //	LIT
#define OP_B_PUSH			0x73 // LIT	

// Conversion
#define OP_F2D				0x81 //
#define OP_F2L				0x82 //
#define OP_F2I				0x83 //
#define OP_F2C				0x84 //
#define OP_F2B				0x85 //

#define OP_D2F				0x86 //
#define OP_D2L				0x87 //
#define OP_D2I				0x88 //
#define OP_D2C				0x89 //
#define OP_D2B				0x8A //

#define OP_L2I				0x8B //
#define OP_L2F				0x8C //
#define OP_L2D				0x8D //
#define OP_L2C				0x8E //
#define OP_L2B				0x8F //

#define OP_I2L				0x90 //
#define OP_I2F				0x91 //
#define OP_I2D				0x92 //
#define OP_I2C				0x93 //
#define OP_I2B				0x94 //

#define OP_C2I				0x95 //
#define OP_C2L				0x96 //
#define OP_C2F				0x97 //
#define OP_C2D				0x98 //
#define OP_C2B				0x99 //

#define OP_B2I				0x9A //
#define OP_B2F				0x9B //
#define OP_B2D				0x9C //
#define OP_B2L				0x9D //
#define OP_B2C				0x9E //

// Common arithmetics
#define OP_I_ADD			0xA0 //
#define OP_I_SUB			0xA1 //
#define OP_I_MUL			0xA2 //
#define OP_I_DIV			0xA3 //

#define OP_F_ADD			0xA4 //
#define OP_F_SUB			0xA5 //
#define OP_F_MUL			0xA6 //
#define OP_F_DIV			0xA7 //

#define OP_D_ADD			0xA8 //
#define OP_D_SUB			0xA9 //
#define OP_D_MUL			0xAA //
#define OP_D_DIV			0xAB //

#define OP_L_ADD			0xAC //
#define OP_L_SUB			0xAD //
#define OP_L_MUL			0xAE //
#define OP_L_DIV			0xAF //

#define OP_C_ADD			0xB0 //
#define OP_C_SUB			0xB1 //
#define OP_C_MUL			0xB2 //
#define OP_C_DIV			0xB3 //

// Integral arithmetic
#define OP_I_SHL 			0xB4 // REG OR LIT
#define OP_I_SHR			0xB5 // REG OR LIT
#define OP_I_MOD			0xB6 //
#define OP_I_XOR			0xB7 //
#define OP_I_AND			0xB8 //
#define OP_I_OR 			0xB9 //

#define OP_L_SHL 			0xBA // REG OR LIT
#define OP_L_SHR			0xBB // REG OR LIT
#define OP_L_MOD			0xBC //
#define OP_L_XOR			0xBD //
#define OP_L_AND			0xBE //
#define OP_L_OR 			0xBF //

#define OP_C_SHL 			0xC0 // REG OR LIT
#define OP_C_SHR			0xC1 // REG OR LIT
#define OP_C_MOD			0xC2 //
#define OP_C_XOR			0xC3 //
#define OP_C_AND			0xC4 //
#define OP_C_OR 			0xC5 //

// Functions
#define OP_CALL 			0xD0

// Branching
#define OP_BRANCH 			0xE0
#define OP_BIFNULL			0xE1
#define OP_BIFNOTNULL		0xE2
#define OP_BIFGREATER		0xE3
#define OP_BIFGREATEREQ		0xE4
#define OP_BIFLESS 			0xE5
#define OP_BIFLESSEQ		0xE6
#define OP_BIFEQUAL			0xE7
#define OP_BIFNOTEQUAL		0xE8

#define OP_UNDEFINED		0xFF

namespace scrap {

enum InstrArgType {
	ARG_NONE,

	// Holds an 8, 32 or 64 bit value, depending on the context.
	ARG_LITERAL,

	// Holds a register (single byte)
	ARG_REGISTER,

	// Holds 40 bits. If the first byte is 0, the next
	// 4 bytes holds an unsigned integer containing the index. Only used
	// when accessing array elements.
	ARG_IDX_OR_REG,
	
	// Holds an ID (32 bit unsigned)
	ARG_ID, 

	// Holds two IDs (2x32 bit unsigned)
	ARG_ID_ID,

	// Holds 40 bits. If the first byte is VarType::OBJECT, the
	// next four bytes holds an ID (32 bit unsigned). 
	ARG_TYPE,

	// Holds a 32 bit unsigned, branch destination
	ARG_BRANCH,
};

/* Map between instructions and the argument type(s) */
struct InstructionInfo {
	const char name[16];
	const unsigned char instr;
	const InstrArgType argtype;
};

unsigned arglen(byte instruction);

static const InstructionInfo g_reserved {
	"RESERVED", 0xFF, ARG_NONE
};

const InstructionInfo g_instructionMap[] = {
	{"POP",		  	OP_POP,			ARG_NONE	},		//0x00
	{"COPY", 	  	OP_COPY, 		ARG_NONE	},		//0x01
	g_reserved,											//0x02
	g_reserved,											//0x03
	{"RETURN",		OP_RETURN, 		ARG_NONE},			//0x04
	g_reserved,											//0x05
	g_reserved,											//0x06
	g_reserved,											//0x07
	g_reserved,											//0x08
	g_reserved,											//0x09
	g_reserved,											//0x0a
	g_reserved,											//0x0b
	g_reserved,											//0x0c
	g_reserved,											//0x0d
	g_reserved,											//0x0e
	g_reserved,											//0x0f
	g_reserved,											//0x10
	g_reserved,											//0x11
	g_reserved,											//0x12
	g_reserved,											//0x13
	g_reserved,											//0x14
	g_reserved,											//0x15
	g_reserved,											//0x16
	g_reserved,											//0x17
	g_reserved,											//0x18
	g_reserved,											//0x19
	g_reserved,											//0x1a
	g_reserved,											//0x1b
	g_reserved,											//0x1c
	g_reserved,											//0x1d
	g_reserved,											//0x1e
	g_reserved,											//0x1f
	{"I_LOAD", 	  	OP_I_LOAD,		ARG_REGISTER	},	//0x20
	{"I_RETURN",   	OP_I_RETURN,	ARG_NONE		},	//0x21
	{"I_STORE",	  	OP_I_STORE,		ARG_REGISTER	},	//0x22
	{"I_PUSH",	  	OP_I_PUSH,		ARG_LITERAL		},	//0x23
	g_reserved,											//0x24
	g_reserved,											//0x25
	g_reserved,											//0x26
	g_reserved,											//0x27
	g_reserved,											//0x28
	g_reserved,											//0x29
	g_reserved,											//0x2a
	g_reserved,											//0x2b
	g_reserved,											//0x2c
	g_reserved,											//0x2d
	g_reserved,											//0x2e
	g_reserved,											//0x2f
	{"F_LOAD", 	  	OP_F_LOAD,		ARG_REGISTER  },	//0x30
	{"F_RETURN",   	OP_F_RETURN,	ARG_NONE	  },	//0x31
	{"F_STORE",	  	OP_F_STORE,		ARG_REGISTER  },	//0x32
	{"F_PUSH",	  	OP_F_PUSH,		ARG_LITERAL	  },	//0x33
	g_reserved,											//0x34
	g_reserved,											//0x35
	g_reserved,											//0x36
	g_reserved,											//0x37
	g_reserved,											//0x38
	g_reserved,											//0x39
	g_reserved,											//0x3a
	g_reserved,											//0x3b
	g_reserved,											//0x3c
	g_reserved,											//0x3d
	g_reserved,											//0x3e
	g_reserved,											//0x3f
	{"D_LOAD", 	  	OP_D_LOAD,		ARG_REGISTER  },	//0x40
	{"D_RETURN",   	OP_D_RETURN,	ARG_NONE	  },	//0x41
	{"D_STORE",	  	OP_D_STORE,		ARG_REGISTER  },	//0x42
	{"D_PUSH",	  	OP_D_PUSH,		ARG_LITERAL	  },	//0x43
	g_reserved,											//0x44
	g_reserved,											//0x45
	g_reserved,											//0x46
	g_reserved,											//0x47
	g_reserved,											//0x48
	g_reserved,											//0x49
	g_reserved,											//0x4a
	g_reserved,											//0x4b
	g_reserved,											//0x4c
	g_reserved,											//0x4d
	g_reserved,											//0x4e
	g_reserved,											//0x4f
	{"L_LOAD", 	  	OP_L_LOAD,		ARG_REGISTER  },	//0x50
	{"L_RETURN",   	OP_L_RETURN,	ARG_NONE	  },	//0x51
	{"L_STORE",	  	OP_L_STORE,		ARG_REGISTER  },	//0x52
	{"L_PUSH",	  	OP_L_PUSH,		ARG_LITERAL	  },	//0x53
	g_reserved,											//0x54
	g_reserved,											//0x55
	g_reserved,											//0x56
	g_reserved,											//0x57
	g_reserved,											//0x58
	g_reserved,											//0x59
	g_reserved,											//0x5a
	g_reserved,											//0x5b
	g_reserved,											//0x5c
	g_reserved,											//0x5d
	g_reserved,											//0x5e
	g_reserved,											//0x5f
	{"C_LOAD", 	  	OP_C_LOAD,		ARG_REGISTER  },	//0x60
	{"C_RETURN",   	OP_C_RETURN,	ARG_NONE	  },	//0x61
	{"C_STORE",	  	OP_C_STORE,		ARG_REGISTER  },	//0x62
	{"C_PUSH",	  	OP_C_PUSH,		ARG_LITERAL	  },	//0x63
	g_reserved,											//0x64
	g_reserved,											//0x65
	g_reserved,											//0x66
	g_reserved,											//0x67
	g_reserved,											//0x68
	g_reserved,											//0x69
	g_reserved,											//0x6a
	g_reserved,											//0x6b
	g_reserved,											//0x6c
	g_reserved,											//0x6d
	g_reserved,											//0x6e
	g_reserved,											//0x6f
	{"B_LOAD", 	  	OP_B_LOAD,		ARG_REGISTER  },	//0x70
	{"B_RETURN",   	OP_B_RETURN,	ARG_NONE	  },	//0x71
	{"B_STORE",	  	OP_B_STORE,		ARG_REGISTER  },	//0x72
	{"B_PUSH",	  	OP_B_PUSH,		ARG_LITERAL	  },	//0x73
	g_reserved,											//0x74
	g_reserved,											//0x75
	g_reserved,											//0x76
	g_reserved,											//0x77
	g_reserved,											//0x78
	g_reserved,											//0x79
	g_reserved,											//0x7a
	g_reserved,											//0x7b
	g_reserved,											//0x7c
	g_reserved,											//0x7d
	g_reserved,											//0x7e
	g_reserved,											//0x7f
	g_reserved,											//0x80
	{"F2D",		  	OP_F2D,			ARG_NONE		},	//0x81
	{"F2L",		  	OP_F2L,			ARG_NONE		},	//0x82
	{"F2I",		  	OP_F2I,			ARG_NONE		},	//0x83
	{"F2C",		  	OP_F2C,			ARG_NONE		},	//0x84
	{"F2B",		  	OP_F2B,			ARG_NONE		},	//0x85
	{"D2F",		  	OP_D2F,			ARG_NONE		},	//0x86
	{"D2L",		  	OP_D2L,			ARG_NONE		},	//0x87
	{"D2I",		  	OP_D2I,			ARG_NONE		},	//0x88
	{"D2C",		  	OP_D2C,			ARG_NONE		},	//0x89
	{"D2B",		  	OP_D2B,			ARG_NONE		},	//0x8A
	{"L2I",		  	OP_L2I,			ARG_NONE		},	//0x8B
	{"L2F",		  	OP_L2F,			ARG_NONE		},	//0x8C
	{"L2D",		  	OP_L2D,			ARG_NONE		},	//0x8D
	{"L2C",		  	OP_L2C,			ARG_NONE		},	//0x8E
	{"L2B",		  	OP_L2B,			ARG_NONE		},	//0x8F
	{"I2L",		  	OP_I2L,			ARG_NONE		},	//0x90
	{"I2F",		  	OP_I2F,			ARG_NONE		},	//0x91
	{"I2D",		  	OP_I2D,			ARG_NONE		},	//0x92
	{"I2C",		  	OP_I2C,			ARG_NONE		},	//0x93
	{"I2B",		  	OP_I2B,			ARG_NONE		},	//0x94
	{"C2I",		  	OP_C2I,			ARG_NONE		},	//0x95
	{"C2L",		  	OP_C2L,			ARG_NONE		},	//0x96
	{"C2F",		  	OP_C2F,			ARG_NONE		},	//0x97
	{"C2D",		  	OP_C2D,			ARG_NONE		},	//0x98
	{"C2B",		  	OP_C2B,			ARG_NONE		},	//0x99
	{"B2I",		  	OP_B2I,			ARG_NONE		},	//0x9A
	{"B2F",		  	OP_B2F,			ARG_NONE		},	//0x9B
	{"B2D",		  	OP_B2D,			ARG_NONE		},	//0x9C
	{"B2L",		  	OP_B2L,			ARG_NONE		},	//0x9D
	{"B2C",		  	OP_B2C,			ARG_NONE		},	//0x9E
	g_reserved,											//0x9F
	{"I_ADD",	  	OP_I_ADD,		ARG_NONE		},	//0xA0
	{"I_SUB",	  	OP_I_SUB,		ARG_NONE		},	//0xA1
	{"I_MUL",	  	OP_I_MUL,		ARG_NONE		},	//0xA2
	{"I_DIV",	  	OP_I_DIV,		ARG_NONE		},	//0xA3
	{"F_ADD",	  	OP_F_ADD,		ARG_NONE		},	//0xA4
	{"F_SUB",	  	OP_F_SUB,		ARG_NONE		},	//0xA5
	{"F_MUL",	  	OP_F_MUL,		ARG_NONE		},	//0xA6
	{"F_DIV",	  	OP_F_DIV,		ARG_NONE		},	//0xA7
	{"D_ADD",	  	OP_D_ADD,		ARG_NONE		},	//0xA8
	{"D_SUB",	  	OP_D_SUB,		ARG_NONE		},	//0xA9
	{"D_MUL",	  	OP_D_MUL,		ARG_NONE		},	//0xAA
	{"D_DIV",	  	OP_D_DIV,		ARG_NONE		},	//0xAB
	{"L_ADD",	  	OP_L_ADD,		ARG_NONE		},	//0xAC
	{"L_SUB",	  	OP_L_SUB,		ARG_NONE		},	//0xAD
	{"L_MUL",	  	OP_L_MUL,		ARG_NONE		},	//0xAE
	{"L_DIV",	  	OP_L_DIV,		ARG_NONE		},	//0xAF
	{"C_ADD",	  	OP_C_ADD,		ARG_NONE		},	//0xB0
	{"C_SUB",	  	OP_C_SUB,		ARG_NONE		},	//0xB1
	{"C_MUL",	  	OP_C_MUL,		ARG_NONE		},	//0xB2
	{"C_DIV",	  	OP_C_DIV,		ARG_NONE		},	//0xB3
	{"I_SHL", 	  	OP_I_SHL,		ARG_NONE 		},	//0xB4
	{"I_SHR",	  	OP_I_SHR,		ARG_NONE 		},	//0xB5
	{"I_MOD",	  	OP_I_MOD,		ARG_NONE		},	//0xB6
	{"I_XOR",	  	OP_I_XOR,		ARG_NONE		},	//0xB7
	{"I_AND",	  	OP_I_AND,		ARG_NONE		},	//0xB8
	{"I_OR", 	  	OP_I_OR,		ARG_NONE		},	//0xB9
	{"L_SHL", 	  	OP_L_SHL,		ARG_NONE 		},	//0xBA
	{"L_SHR",	  	OP_L_SHR,		ARG_NONE 		},	//0xBB
	{"L_MOD",	  	OP_L_MOD,		ARG_NONE		},	//0xBC
	{"L_XOR",	  	OP_L_XOR,		ARG_NONE		},	//0xBD
	{"L_AND",	  	OP_L_AND,		ARG_NONE		},	//0xBE
	{"L_OR", 	  	OP_L_OR,		ARG_NONE		},	//0xBF
	{"C_SHL", 	  	OP_C_SHL,		ARG_NONE 		},	//0xC0
	{"C_SHR",	  	OP_C_SHR,		ARG_NONE 		},	//0xC1
	{"C_MOD",	  	OP_C_MOD,		ARG_NONE		},	//0xC2
	{"C_XOR",	  	OP_C_XOR,		ARG_NONE		},	//0xC3
	{"C_AND",	  	OP_C_AND,		ARG_NONE		},	//0xC4
	{"C_OR", 	  	OP_C_OR,		ARG_NONE		},	//0xC5
	g_reserved,											//0xC6
	g_reserved,											//0xC7
	g_reserved,											//0xC8
	g_reserved,											//0xC9
	g_reserved,											//0xCA
	g_reserved,											//0xCB
	g_reserved,											//0xCC
	g_reserved,											//0xCD
	g_reserved,											//0xCE
	g_reserved,											//0xCF
	{"CALL", 		OP_CALL,		ARG_ID},			//0xD0
	g_reserved,											//0xD1
	g_reserved,											//0xD2
	g_reserved,											//0xD3
	g_reserved,											//0xD4
	g_reserved,											//0xD5
	g_reserved,											//0xD6
	g_reserved,											//0xD7
	g_reserved,											//0xD8
	g_reserved,											//0xD9
	g_reserved,											//0xDA
	g_reserved,											//0xDB
	g_reserved,											//0xDC
	g_reserved,											//0xDD
	g_reserved,											//0xDE
	g_reserved,											//0xDF
	{"OP_BRANCH",	 	OP_BRANCH,			ARG_BRANCH},//0xE0
	{"OP_BIFNULL",	 	OP_BIFNULL,			ARG_BRANCH},//0xE1
	{"OP_BIFNOTNULL",	OP_BIFNOTNULL,		ARG_BRANCH},//0xE2
	{"OP_BIFGREATER",	OP_BIFGREATER,		ARG_BRANCH},//0xE3
	{"OP_BIFGREATEREQ",	OP_BIFGREATEREQ,	ARG_BRANCH},//0xE4
	{"OP_BIFLESS",		OP_BIFLESS,			ARG_BRANCH},//0xE5
	{"OP_BIFLESSEQ",	OP_BIFLESSEQ,		ARG_BRANCH},//0xE6
	{"OP_BIFEQUAL",		OP_BIFEQUAL,		ARG_BRANCH},//0xE7
	{"OP_BIFNOTEQUAL",	OP_BIFNOTEQUAL,		ARG_BRANCH},//0xE8
	g_reserved,											//0xE9
	g_reserved,											//0xEA
	g_reserved,											//0xEB
	g_reserved,											//0xEC
	g_reserved,											//0xED
	g_reserved,											//0xEE
	g_reserved,											//0xEF
	g_reserved,											//0xF0
	g_reserved,											//0xF1
	g_reserved,											//0xF2
	g_reserved,											//0xF3
	g_reserved,											//0xF4
	g_reserved,											//0xF5
	g_reserved,											//0xF6
	g_reserved,											//0xF7
	g_reserved,											//0xF8
	g_reserved,											//0xF9
	g_reserved,											//0xFA
	g_reserved,											//0xFB
	g_reserved,											//0xFC
	g_reserved,											//0xFD
	g_reserved,											//0xFE
	g_reserved,											//0xFF
};

}
