#include "interop.h"


/***** ByteOperation *****/
ByteOperation::ByteOperation(byte val) {
	mByte = val;
}

void ByteOperation::ProvideBytecode(Opcode *opcode) {
	opcode->AddByte(mByte);
}


/***** DwordOperation *****/
DwordOperation::DwordOperation(void *dword) {
	mDword = malloc(4);
	memcpy(mDword, dword, 4);
}

DwordOperation::~DwordOperation() {
	free(mDword);
}

void DwordOperation::ProvideBytecode(Opcode *opcode) {
	opcode->AddDword(mDword);
}


/***** PositionInquirer *****/
void PositionInquirer::ProvideBytecode(Opcode *opcode) {
	// Provide a temporary value
	mReplaceIndex = opcode->Length();
	opcode->AddUint(0);
}

void PositionInquirer::InsertPosition(Opcode *opcode, uint finalValue) {
	opcode->ReplaceUint(mReplaceIndex, finalValue);
}


/***** PositionReference *****/
void PositionReference::ProvideBytecode(Opcode *opcode) {
	for (int i=0; i<mInquirers.size(); i++) {
		mInquirers[i]->InsertPosition(opcode, opcode->Length());
	}
}

void PositionReference::AddInquirer(PositionInquirer *inquirer) {
	mInquirers.push_back(inquirer);
}