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

protected:
	virtual void ReadMagicNumber();
	virtual void ReadVersionNumber();
	virtual void ReadEndian();
	virtual void ReadMain();

	virtual void ReadClassList();
	virtual void ReadClass();
	virtual void ReadFields(Class *c);
	virtual void ReadMethods(Class *c);
	virtual TypeDesc ReadField();
	virtual Method* ReadMethod(MethodType methodType, Class *c);
	virtual MethodBody ReadMethodBody();
	virtual TypeDesc ReadTypeDesc(bool readName);

private:
	BinaryFile *_file;
	ClassList *_classList;

	unsigned _mainClassID;
	unsigned _mainMethodID;
};

}

