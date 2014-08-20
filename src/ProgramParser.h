#pragma once

#include "Scrap.h"
#include "Function.h"

namespace scrap {

class BinaryFile;
class Program;
class Function;
class FunctionBody;
class FunctionList;


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

	// Read the functions into _funcList and return the index of the main function
	virtual unsigned ReadFunctions();
	virtual Function* ReadFunction();
	virtual FunctionBody ReadFunctionBody();
	virtual TypeDesc ReadTypeDesc(bool readName);

private:
	BinaryFile *_file;
	FunctionList *_funcList;
};

}

