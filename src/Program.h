#pragma once

#include "Scrap.h"

namespace scrap {

class ClassList;
class Method;
class Class;


/* Program
 * The highest level execution unit.
 */
class Program {
public:
	Program();
	~Program();

	/* Before executing the program, the ClassList and the unique
	 * ID of the start method (and start class) must be defined. The class
	 * list must be set for the main method to be found!
	 */
	void SetClassList(ClassList *classList);
	ClassList* GetClassList();
	const ClassList* GetClassList() const;
	void SetMainMethod(unsigned classID, unsigned stMethodID);

	void Execute();

private:
	ClassList *_classList;
	const Class *_mainClass;
	Method *_mainMethod;

};

}
