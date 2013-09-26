#include "opcode.h"
#include "scrapexcept.h"

Opcode::Opcode() {
	mBigEndian = IsBigEndian();
	
	mInsertTails.Push(&mInterops.end());
}

byte* Opcode::GetRaw() {
	return &mBytes[0];
}

int Opcode::Length() {
	return mBytes.size();
}


InteropIter* Opcode::AddInterop(IntermediateOperation *interop) {
	InteropIter *tail = mInsertTails.Peek();
	return &mInterops.insert(*tail, interop);
}

void Opcode::PushTail(InteropIter *it) {
	mInsertTails.Push(it);
}

void Opcode::PopTail() {
	if (mInsertTails.Size() <= 1) {
		throw StackUnderflowException("Stack underflow: internal error");
	}

	mInsertTails.Pop();
}


Opcode* Opcode::AddByte(byte val) {
	mBytes.push_back(val);
	return this;
}

Opcode* Opcode::AddDword(void *dword) {
	byte *b = (byte*)&dword;

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

Opcode* Opcode::AddInt(int val) {
	return AddDword(&val);
}

Opcode* Opcode::AddUint(uint val) {
	return AddDword(&val);
}


void Opcode::ReplaceByte(int index, byte val) {
	if (index < 0 || index >= mBytes.size()) {
		throw range_error("Byte replacement index out of range");
	}

	mBytes[index] = val;
}

void Opcode::ReplaceUint(int index, uint val) {
	byte *b = (byte*)&val;

	if (mBigEndian) {
		for (int i=3; i>=0; i--) {
			ReplaceByte(index + i, b[i]);
		}
	} else {
		for (int i=0; i<4; i++) {
			ReplaceByte(index + i, b[i]);
		}
	}
}


bool Opcode::IsBigEndian() {
	union {
        uint i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}