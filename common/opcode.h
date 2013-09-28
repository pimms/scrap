#pragma once

/***** opcode.h *****
* Defines a wrapper for modifying byte codes
* at run-time.
*****/

#include "codes.h"
#include "stack.h"

#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <vector>
using namespace std;

class IntermediateOperation;

typedef list<IntermediateOperation*>::iterator InteropIter;

/***** Class Opcode *****
* Builder of bytecode from Statements.
*****/
class Opcode {
public:
						Opcode();

	bool				IsBigEndian();

	const vector<byte>	GetBytecode();
	int					Length();


	InteropIter			AddInterop(IntermediateOperation *interop);
	void				PushTail(InteropIter it);
	void				PopTail();

	bool				BuildBytecodeFromIntermediates();


	/***** Add *****
	* Add bytes or integers to the final bytecode
	* vector. All bytes added via these functions are
	* _mutable_, but their positions are final.
	*****/
	Opcode*				AddByte(byte val);
	Opcode*				AddDword(void *dword);
	Opcode*				AddInt(int val);
	Opcode*				AddUint(uint val);

	void				ReplaceByte(int index, byte val);
	void				ReplaceUint(int index, uint val);

private:
	list<IntermediateOperation*> mInterops;
	Stack<InteropIter>	mInsertTails;

	vector<byte>		mBytes;
	bool				mBigEndian;
};