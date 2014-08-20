#include "ProgramParser.h"
#include "Program.h"
#include "BinaryFile.h"
#include "Function.h"
#include "Variable.h"
#include "IndexList.h"
#include "Bytecode.h"
#include <cstring>

namespace scrap {

ProgramParser::ProgramParser()
	:	_file(NULL),
		_funcList(NULL)
{ }

ProgramParser::~ProgramParser()
{
	if (_file) {
		delete _file;
	}
}


Program* ProgramParser::ParseProgramFile(string fileName)
{
	_file = new BinaryFile(fileName, READ);
	_funcList = new FunctionList();

	ReadMagicNumber();
	ReadVersionNumber();
	ReadEndian();
	unsigned mainFuncIdx = ReadFunctions();

	Program *program = new Program(_funcList, mainFuncIdx);

	if (_file->RemainingBytes() != 0) {
		delete program;
		THROW(FileException, "Program file has remaining bytes");
	}

	return program;
}



void ProgramParser::ReadMagicNumber()
{
	//             S     C     R     A     P
	byte exp[5] = {0x53, 0x43, 0x52, 0x41, 0x50};
	for (int i=0; i<5; i++) {
		if (_file->ReadByte() != exp[i]) {
			THROW(ParseError,
			"Specified program file does not contain the magic number");
		}
	}
}

void ProgramParser::ReadVersionNumber()
{
	byte maj, min;
	maj = _file->ReadByte();
	min = _file->ReadByte();

	// TODO
	// Consider whether or not to continue based on the version number.
}

void ProgramParser::ReadEndian()
{
	byte buf = _file->ReadByte();

	if (buf != L_ENDIAN && buf != B_ENDIAN) {
		THROW(ParseError, "Invalid endian specified in program file");
	}

	Endian endian = (Endian)buf;
	_file->SetFileEndianess(endian);
}


unsigned ProgramParser::ReadFunctions()
{
	unsigned funcCount = _file->ReadUnsigned();
	unsigned mainIndex = _file->ReadUnsigned();

	for (int i=0; i<funcCount; i++) {
		Function *func = ReadFunction();
		_funcList->AddFunction(func);
	}

	return mainIndex;
}

Function* ProgramParser::ReadFunction()
{
	string name = _file->ReadString();
	
	vector<TypeDesc> args;
	TypeDesc retType;

	unsigned argcount = _file->ReadUnsigned();	
	for (int i=0; i<argcount; i++) {
		 TypeDesc tdesc = ReadTypeDesc(true);
		 args.push_back(tdesc);
	}

	retType = ReadTypeDesc(false);

	FunctionAttributes attrs(retType, args, name);
	FunctionBody body = ReadFunctionBody();

	Function *function = new Function(&body, attrs);
	delete[] body.code;

	return function;
}

FunctionBody ProgramParser::ReadFunctionBody() 
{
	FunctionBody body;
	body.length = _file->ReadUnsigned();
	body.code = new byte[body.length];

	int read = 0;
	while (read < body.length) {
		byte instr = _file->ReadByte();
		body.code[read++] = instr;
		int len = arglen(instr);

		// Read the arguments. The BinaryFile will make sure that the
		// byte order in whatever arbitrary argument is correct, and they
		// can simply be copied into the method buffer.
		switch (g_instructionMap[instr].argtype) {
			case ARG_NONE: 
				break;

			case ARG_LITERAL: {
				unsigned u;
				unsigned long ul;
				switch (len) {
					case 1:
						body.code[read++] = _file->ReadByte();
						break;
					case 4:
						u = _file->ReadUnsigned();
						memcpy(body.code+read, &u, 4);
						read += 4;
						break;
					case 8:
						ul = _file->ReadULong();
						memcpy(body.code+read, &ul, 8);
						read += 8;
						break;
				}
				break;
			}

			case ARG_REGISTER: {
				body.code[read++] = _file->ReadByte();
				break;
			}

			case ARG_BRANCH:
				/* FALLTHROUGH */
			case ARG_ID: {
				unsigned u = _file->ReadUnsigned();
				memcpy(body.code+read, &u, 4);
				read += 4;
				break;
			}

			case ARG_IDX_OR_REG:
				/* FALLTHROUGH */
			case ARG_TYPE:{
				body.code[read++] = _file->ReadByte();
				unsigned u = _file->ReadUnsigned();
				memcpy(body.code+read, &u, 4);
				read += 4;
				break;
		 	}

			case ARG_ID_ID: {
				for (int i=0; i<2; i++) {
					unsigned u = _file->ReadUnsigned();
					memcpy(body.code+read, &u, 4);
					read += 4;
				}
				break;
			}
		}
	}

	return body;
}

TypeDesc ProgramParser::ReadTypeDesc(bool readName)
{
	byte type;
	string name;
	TypeDesc typeDesc;

	if (readName) {
		name = _file->ReadString();
	}

	type = _file->ReadByte();
	
	/* Assert legal VarType-value */
	switch (type) {
		case INT:
		case DOUBLE:
		case FLOAT:
		case CHAR:
		case BOOL:
		case VOID:
			break;
		default:
			THROW(InvalidTypeException,
			"Invalid type for TypeDesc in program file");
	}

	typeDesc.type = (VarType)type;
	
	if (readName) {
		typeDesc.name = name;
	}

	return typeDesc;
}

}

