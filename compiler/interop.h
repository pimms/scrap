#pragma once

#include "../common/opcode.h"

#include <vector>
using namespace std;

/***** Class IntermediateOperation *****
* Holds a single byte, integer or float value.
* Statements are converted into lists of multiple
* IntermediateOperations before they are converted
* to actual bytecode. 
*
* The primary goal of this intermediate step is
* to build full knowledge of absolute byte-positions
* such as the position of a function definition, or the
* end of a scope.
*****/
class IntermediateOperation {
public:
	virtual void	ProvideBytecode(Opcode *opcode) = 0;
};


/***** Class ByteOperation *****
* Holds a single byte value.
*****/
class ByteOperation : public IntermediateOperation {
public:
					ByteOperation(byte val);

	virtual void	ProvideBytecode(Opcode *opcode);

protected:
	byte			mByte;
};


/***** Class DwordOperation *****
* Holds an integer, unsigned or float.
*****/
class DwordOperation : public IntermediateOperation {
public:
					DwordOperation(void *dword);
					~DwordOperation();

	virtual void	ProvideBytecode(Opcode *opcode);

protected:
	void			*mDword;
};


/***** Class PositionInquirer *****
* Holds the absolute byteindex of a segment of the 
* code. This could be a jump-destination, the location
* of a function or similar.
*****/
class PositionInquirer : public DwordOperation {
public:
	virtual void	ProvideBytecode(Opcode *opcode);
	void			InsertPosition(Opcode *opcode, uint finalValue);

private:
	int				mReplaceIndex;
};


/***** Class PositionReference *****
* When an instance of this class is told to produce
* it's bytecode, 
*****/
class PositionReference : public IntermediateOperation {
public:
	void			ProvideBytecode(Opcode *opcode);
	
	void			AddInquirer(PositionInquirer *inquirer);

protected:
	vector<PositionInquirer*> mInquirers;
};


/***** Class FunctionTail *****
* Inserted as tail from a FunctionDefinition (Fragment subclass),
* and pops itself (and inserts a OP_RET) upon bytecode provision.
*****/
class FunctionTail : public ByteOperation {
public:
					FunctionTail() 
						: ByteOperation(OP_RET){}
	virtual void	ProvideBytecode(Opcode *opcode);
};