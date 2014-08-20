#include "gtest/gtest.h"

int main(int argc, char **argv) 
{
	char **fakev = new char*[argc + 1];
	for (int i=0; i<argc; i++) {
		fakev[i] = argv[i];
	}

	fakev[argc] = new char[32];
	strcpy(fakev[argc++], "--gtest_color=yes");

	::testing::InitGoogleTest(&argc, fakev);
	return RUN_ALL_TESTS();
}
