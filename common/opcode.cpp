#include "opcode.h"

Opcode::Opcode() {
	mBigEndian = IsBigEndian();

	printf("%s endian\n", mBigEndian?"big":"little");
}

byte* Opcode::GetRaw() {
	int len = 0;
	byte *ret = NULL;
	list<byte>::iterator it;
	int i = 0;

	len = mBytes.size();
	if (len == 0) return NULL;
	ret = new byte[len];

	for (it = mBytes.begin(); it != mBytes.end(); it++) {
		ret[i++] = *it;
	}

	return ret;
}

Opcode* Opcode::AddByte(byte val) {
	mBytes.push_back(val);
	return this;
}

Opcode* Opcode::AddInt(int val) {
	byte *b = (byte*)&val;

	if (mBigEndian) {
		for (int i=3; i>=0; i--) {
			AddByte(b[i]);
		}
	} else {
		for (int i=0; i<4; i++) {
			AddByte(b[i]);
		}
	}

	return this;
}


bool Opcode::IsBigEndian() {
	union {
        uint i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}