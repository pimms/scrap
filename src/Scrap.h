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


typedef unsigned char uchar;
typedef unsigned char byte;
typedef unsigned long ulong;


namespace scrap {

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

/* Generic macro for throwing an exception type taking a 
 * single std::string as it`s only parameter.
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
		

EXCEPTION_DECL(InvalidTypeException)
EXCEPTION_DECL(InvalidCastException)

}


