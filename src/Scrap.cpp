#include "Scrap.h"

using namespace scrap;


int main(int argc, char *argv[]) 
{
	try {
		THROW(InvalidTypeException, "hey hey");
	} catch (GenericException ex) {
		printf("Exception caught:\n%s\n", ex.msg.c_str());
	}
}

