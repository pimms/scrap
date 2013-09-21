#pragma once

/***** codes.h *****
*
* This file contains the definitions for the 
* byte-codes used.
*
* Common type-definitions are also included in 
* this file.
*****/


// Bitwise AND-helpers
#define HIGH_WORD 		0xFFFF0000
#define LOW_WORD  		0x0000FFFF

#define HIGH_NIBBLE 	0xF0
#define LOW_NIBBLE  	0x0F

/***** VARIABLE CONSTANTS ****
* Variables are stored in a lookup-table in the
* Environment class. 
* There may only ever be (2^28)-1 variables allocated.
*****/
#define VAR_GLOBAL 		0x80000000
#define VAR_LOCAL		0x40000000
#define VAR_RETURN		0xFFFFFFFF



/***** BASIC OPERATIONS ***** 
* Start: 		0x01
* Finish: 		0x1F
*
* A "PARAM"-tag follows opcodes that expects
* a 4 byte parameter.
*
* (*) 	The back element on the stack is moved into
* 		the parameter variable.
*****/
#define OP_PUSH 				0x01  	// PARAM
#define OP_PUSH_DATA			0x02	// PARAM
#define OP_POP 					0x03	// PARAM
#define OP_CALL 				0x04	// PARAM
#define OP_RET 					0x05	// PARAM
#define OP_ALLOC 				0x06 	// PARAM
#define OP_POPMOV 				0x07	// PARAM (*)
#define OP_EXIT 				0x08	// PARAM
#define OP_MOV 					0x09	// PARAM 2  (var id)
#define OP_MOVI					0x0A	// PARAM 2  (int literal)
#define OP_MOVF 				0x0B	// PARAM 2  (float literal)
#define OP_MOVS					0x0C	// PARAM 2  (string data)


// Arithmetic operations are performed:
//		b = pop()
// 		a = pop()
// 		a = a (operator) b
// 		push(a)
#define OP_ADD					0x0D
#define OP_SUB					0x0E
#define OP_MUL					0x0F
#define OP_DIV 					0x10
#define OP_MOD					0x11

/***** COMPARISON AND JUMPS *****
* Start: 		0x20
* Finsih:		0x2F
*
* Comparisons are performed in the same order
* as arithmetics. This is a pseudo-comparison
* between the two last elements on the stack:
*
* b = pop()
* a = pop()
* if (a >= b) { stuff() }
*****/
#define OP_JMP					0x20	// PARAM
#define OP_JE					0x21	// PARAM
#define OP_JNE					0x22	// PARAM
#define OP_JG					0x23	// PARAM
#define OP_JGE					0x24	// PARAM
#define OP_JL					0x25	// PARAM
#define OP_JLE					0x26	// PARAM


/***** DATA DEFINITIONS *****
* Start: 		0xF0
* Finish: 		0xFF
*
* (*) 	Requires four bytes (string length),
* 		followed by a NULL-terminated string.
* (**)	Required an int defining the ID of the 
*		function, followed by an int containing
*		the byte offset of the first instruction
*		in the function.
*****/
#define OP_DATA_BEGIN			0xF0
#define OP_DATA_STRING			0xF1 	// PARAM (*)
#define OP_DATA_FUNC			0xF2	// PARAM (**)
//#define OP_DATA_CLASS_POS		0xF3	// PARAM

#define OP_DATA_END 			0xFF



/***** TYPE DEFINITIONS *****
* Common type definitions.
*****/
typedef unsigned		uint;
typedef unsigned char	byte;