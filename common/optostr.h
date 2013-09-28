#pragma once

/***** Class OpcodeText *****
* Class for converting scrap-bytecode to
* human readable text.
*****/

#include "codes.h"
#include <vector>
using namespace std;

class Opcode;

class OpcodeText {
public:
	void		Parse(Opcode *opcode);

private:
	vector<byte> mOpcodes;

	int			ParamCount(byte code);
	string		GetLiteral(byte code);
	string		GetParameter(byte ctx, int opcodeIdx, int paramIdx);


	void*		GetDword(int opcodeIdx);
	string		GetUint(int opcodeIdx);
	string		GetInt(int opcodeIdx);
	string		GetFloat(int opcodeIdx);
};