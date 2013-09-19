#pragma once

/***** opcode.h *****
* Defines a wrapper for modifying byte codes
* at run-time.
*****/

#include <list>
#include "codes.h"

using namespace std;

class Opcode {
public:
						Opcode();

	byte*				GetRaw();
	int					Length();

	Opcode*				AddByte(byte val);
	Opcode*				AddInt(int val);
	Opcode*				AddUint(uint val);

private:
	list<byte>			mBytes;
	bool				mBigEndian;

	bool				IsBigEndian();
};