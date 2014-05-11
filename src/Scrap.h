#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <stdexcept>

using std::string;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::vector;
using std::list;
using std::deque;
using std::map;
using std::runtime_error;



namespace scrap {

typedef unsigned char uchar;
typedef unsigned char byte;
typedef unsigned long ulong;
typedef unsigned int  ClassID;
typedef unsigned int  MethodID;

// Primitive types
enum VarType {
	a 		= 0x00,
	OBJECT 	= 0x00,		

	i 		= 0x01,
	INT 	= 0x01,

	f 		= 0x02,
	FLOAT 	= 0x02,

	d 		= 0x04,
	DOUBLE 	= 0x04,

	l 		= 0x08,
	LONG 	= 0x08,

	c 		= 0x10,
	CHAR 	= 0x10,

	b 		= 0x20,
	BOOL 	= 0x20,

	v 		= 0x40,
	VOID	= 0x40,
};


// Descriptor of return types, argument types and field types.
struct TypeDesc {
	VarType type;

	// Must contain the class name in the case of "type == OBJECT".
	const char *className;
};


// Arithmetic operations 
enum AritOp {
	ADD,
	SUB,
	MUL,
	DIV,
	SHR,
	SHL,
	MOD,
	XOR,
	AND,
	OR,
};

string VarTypeToString(VarType t);
string AritOpToString(AritOp op);

#ifdef _SCRAP_TEST_
// Scrap.cpp is not compiled in the test-binary, include placeholder
// stubs for the test binary.
inline string VarTypeToString(VarType t) {
	return "[not available during tests]";
}

inline string AritOpToString(AritOp op) {
	return "[not available during tests]";
}
#endif /* _SCRAP_TEST_ */


// Superclass for all exceptions thrown in Scrap
// during both compilation and execution.
class GenericException : public runtime_error {
public:
	GenericException(string file, 
					 int line, 
					 string prettyfunc, 
					 string msg)
	:	runtime_error(msg),
		file(file),
		line(line),
		prettyfunc(prettyfunc),
		msg(msg)
	{
		
	}

	~GenericException() throw() 
	{

	}

	string file;
	int line;
	string prettyfunc;
	string msg;
};

#define EXCEPTION_DECL(_CLASS) 								\
	class _CLASS : public GenericException {				\
	public:													\
		_CLASS(string f, int l, string pf, string m)		\
			: GenericException(f, l, pf, m) {}				\
	};

/* Macro for throwing an exception type which inherits
 * from GenericException (which includes them all, lol).
 */
#define THROW(_EXCLASS, _MSG)								\
	{														\
		stringstream _ss_;									\
		_ss_ << "File: " << __FILE__;						\
		_ss_ << "\nLine: " << __LINE__;						\
		_ss_ << "\nException: " << #_EXCLASS;				\
		_ss_ << "\nFunction: " << __PRETTY_FUNCTION__; 		\
		_ss_ << "\nMessage: " << _MSG;						\
		throw _EXCLASS(__FILE__, __LINE__, 					\
						__PRETTY_FUNCTION__, _ss_.str());	\
	}
		

EXCEPTION_DECL(InternalInconsistencyException)
EXCEPTION_DECL(InvalidTypeException)
EXCEPTION_DECL(InvalidCastException)
EXCEPTION_DECL(InvalidOperationException)
EXCEPTION_DECL(InvalidArgumentException)
EXCEPTION_DECL(InvalidClassException)
EXCEPTION_DECL(NotImplementedException)
EXCEPTION_DECL(DivisionByZeroException)
EXCEPTION_DECL(StackUnderflowException)
EXCEPTION_DECL(StackOverflowException)
EXCEPTION_DECL(StackNotEmptyException)
EXCEPTION_DECL(ObjectRetainException)
EXCEPTION_DECL(NullPointerException)

}


