#pragma once

#include "Scrap.h"
#include "Method.h"

namespace scrap {

class BinaryFile;
class Program;
class Class;
class ClassList;
class Method;


/* Program Parser
 * Parses a Scrap Binary File and creates the corresponding 
 * Program. 
 */
class ProgramParser {
public:
	ProgramParser();
	~ProgramParser();

	Program* ParseProgramFile(string fileName);

private:
	BinaryFile *_file;
	ClassList *_classList;

	unsigned _mainClassID;
	unsigned _mainMethodID;

	void ReadMagicNumber();
	void ReadVersionNumber();
	void ReadEndian();
	void ReadMain();

	void ReadClassList();
	void ReadClass();
	void ReadFields(Class *c);
	void ReadMethods(Class *c);
	TypeDesc ReadField();
	Method* ReadMethod(MethodType methodType, Class *c);
	MethodBody ReadMethodBody();
	TypeDesc ReadTypeDesc(bool readName);
};

}

