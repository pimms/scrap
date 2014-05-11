#pragma once


// 		MNEMONIC 			VALUE	ARGS		DESCRIPTION
#define OP_POP				0x00 //				Pop and discard top value
#define OP_COPY 			0x01 //				Copy top value
#define OP_ARRAYLENGTH 		0x02 //				Pops array, pushes length
#define OP_ARRAYLOAD 		0x03 //	LIT			Pushes array from register LIT

// Stack management 
#define OP_A_LOAD 			0x10 //	LIT			Pushes object in reg LIT
#define OP_A_RETURN 		0x11 //				Returns an object
#define OP_A_STORE			0x12 //	LIT			Pop object into reg LIT
#define OP_A_NEWARRAY		0x14 //	TYPE		Creates new array
#define OP_A_ARELEASE		0x15 //				Release array of objects
#define OP_A_ALOAD 			0x16 //	LIT			Push array element with idx LIT
#define OP_A_ASTORE 		0x17 // LIT			Pop into array index LIT

#define OP_I_LOAD 			0x20 // LIT
#define OP_I_RETURN 		0x21 //
#define OP_I_STORE			0x22 //	LIT
#define OP_I_PUSH			0x23 // LIT			Push a literal value of type INT
#define OP_I_NEWARRAY		0x24 //	TYPE
#define OP_I_ARELEASE		0x25 //
#define OP_I_ALOAD 			0x26 // LIT
#define OP_I_ASTORE 		0x27 // LIT

#define OP_F_LOAD 			0x30 // LIT
#define OP_F_RETURN 		0x31 //
#define OP_F_STORE			0x32 //	LIT
#define OP_F_PUSH			0x33 // LIT	
#define OP_F_NEWARRAY		0x34 //	TYPE
#define OP_F_ARELEASE		0x35 //
#define OP_F_ALOAD 			0x36 // LIT
#define OP_F_ASTORE 		0x37 // LIT

#define OP_D_LOAD 			0x40 // LIT
#define OP_D_RETURN 		0x41 //
#define OP_D_STORE			0x42 //	LIT
#define OP_D_PUSH			0x43 // LIT	
#define OP_D_NEWARRAY		0x44 //	TYPE
#define OP_D_ARELEASE		0x45 //
#define OP_D_ALOAD 			0x46 // LIT
#define OP_D_ASTORE 		0x47 // LIT

#define OP_L_LOAD 			0x50 // LIT
#define OP_L_RETURN 		0x51 //
#define OP_L_STORE			0x52 //	LIT
#define OP_L_PUSH			0x53 // LIT	
#define OP_L_NEWARRAY		0x54 //	TYPE
#define OP_L_ARELEASE		0x55 //
#define OP_L_ALOAD 			0x56 // LIT
#define OP_L_ASTORE 		0x57 // LIT

#define OP_C_LOAD 			0x60 // LIT
#define OP_C_RETURN 		0x61 //
#define OP_C_STORE			0x62 //	LIT
#define OP_C_PUSH			0x63 // LIT	
#define OP_C_NEWARRAY		0x64 //	TYPE
#define OP_C_ARELEASE		0x65 //
#define OP_C_ALOAD 			0x66 // LIT
#define OP_C_ASTORE 		0x67 // LIT

#define OP_B_LOAD 			0x70 // LIT
#define OP_B_RETURN 		0x71 //
#define OP_B_STORE			0x72 //	LIT
#define OP_B_PUSH			0x73 // LIT	
#define OP_B_NEWARRAY		0x74 //	TYPE
#define OP_B_ARELEASE		0x75 //
#define OP_B_ALOAD 			0x76 // LIT
#define OP_B_ASTORE 		0x77 // LIT

// Conversion
#define OP_A2B				0x80 //

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

// Object related instructions
#define OP_NEW				0xC6 // LIT				Create a new object
#define OP_RETAIN			0xC7 //					Increment ref. counter
#define OP_RELEASE 			0xC8 //					Decrement ref. counter
#define OP_INVOKE 			0xC9 //	LIT				Call method with id LIT
#define OP_VINVOKE			0xCA // LIT				Cakk vurtyak method
#define OP_STINVOKE			0xCB // LIT LIT			Call static method (c=0,m=1)
#define OP_LOADFIELD		0xCC // LIT				Push member variable with id LIT
#define OP_LOADSTATIC		0xCF // LIT				Push static member with id LIT
#define OP_STOREFIELD		0xD0 //	LIT				Pop into member variable
#define OP_STORESTATIC		0xD1 // LIT				Pop into static member
