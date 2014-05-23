#include "ProgramParser.h"
#include "Program.h"
#include "BinaryFile.h"
#include "Class.h"
#include "Method.h"
#include "Variable.h"





#include "IndexList.h"
#include "Bytecode.h"
#include <cstring>

namespace scrap {

ProgramParser::ProgramParser()
	:	_file(NULL),
		_classList(NULL),
		_mainClassID(0),
		_mainMethodID(0)
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
	_classList = new ClassList();

	ReadMagicNumber();
	ReadVersionNumber();
	ReadEndian();
	ReadMain();
	ReadClassList();

	Program *program = new Program();
	program->SetClassList(_classList);
	program->SetMainMethod(_mainClassID, _mainMethodID);

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
	printf("[ProgramParser] Parsing file with version %i.%i\n", maj, min);
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

void ProgramParser::ReadMain()
{
	_mainClassID = _file->ReadUnsigned();
	_mainMethodID = _file->ReadUnsigned();
}


void ProgramParser::ReadClassList()
{
	unsigned classCount = _file->ReadUnsigned();

	for (int i=0; i<classCount; i++) {
		unsigned id = _file->ReadUnsigned();
		string name = _file->ReadString();

		Class *c = new Class(id, name);
		_classList->AddClass(c, id);
	}

	for (int i=0; i<classCount; i++) {
		ReadClass();
	}
}

void ProgramParser::ReadClass()
{
	unsigned classID = _file->ReadUnsigned();
	unsigned superID = _file->ReadUnsigned();

	Class *c = _classList->GetClass(classID);

	if (superID != ID_UNDEFINED) {
		Class *super = _classList->GetClass(superID);
		c->SetSuper(super);
	}

	ReadFields(c);
	ReadMethods(c);
}

void ProgramParser::ReadFields(Class *c)
{
	unsigned count = 0;

	// Read object fields
	count = _file->ReadUnsigned();
	for (int i=0; i<count; i++) {
		TypeDesc type = ReadField();
		c->AddFieldTemplate(type);
	}

	// Read static fields
	count = _file->ReadUnsigned();
	for (int i=0; i<count; i++) {
		TypeDesc type = ReadField();
		c->AddStaticField(type);
	}
}

void ProgramParser::ReadMethods(Class *c)
{
	unsigned count = 0;

	// Read instance methods
	count = _file->ReadUnsigned();
	for (int i=0; i<count; i++) {
		Method *method = ReadMethod(METHOD_NORMAL, c);
		c->AddMethod(method);
	}

	// Read static methods
	count = _file->ReadUnsigned();
	for (int i=0; i<count; i++) {
		Method *method = ReadMethod(METHOD_STATIC, c);
		c->AddStaticMethod(method);
	}
}

TypeDesc ProgramParser::ReadField()
{
	TypeDesc type = ReadTypeDesc(true);
	return type;
}

Method* ProgramParser::ReadMethod(MethodType methodType, Class *c)
{
	string name = _file->ReadString();
	byte isVirtual = _file->ReadByte();
	
	vector<TypeDesc> args;
	TypeDesc retType;

	unsigned argcount = _file->ReadUnsigned();	
	for (int i=0; i<argcount; i++) {
		 TypeDesc tdesc = ReadTypeDesc(true);
		 args.push_back(tdesc);
	}

	retType = ReadTypeDesc(false);

	MethodType type = (isVirtual) ? METHOD_VIRTUAL : methodType;
	MethodAttributes attrs(retType, args, name);
	MethodBody body = ReadMethodBody();

	Method *method = new Method(type, c, &body, attrs);
	delete[] body.code;

	return method;
}

MethodBody ProgramParser::ReadMethodBody() 
{
	MethodBody body;
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
		case OBJECT:
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
	
	if (typeDesc.type == OBJECT) {
		typeDesc.classID = _file->ReadUnsigned();
	}

	if (readName) {
		typeDesc.name = name;
	}

	return typeDesc;
}

}

