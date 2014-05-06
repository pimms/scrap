#pragma once

#include "Scrap.h"

namespace scrap {

/* Class
 */
class Class {
public:
	Class(int classID, string className);

	int GetClassID() const;

private:
	string _className;
	int _classID;
};

}
