scrap
=====

The crappy scripting language Scrap. This is a hobby exercise for 
compiler development, and should for the foreseeable future  be 
disregarded on all grounds in terms of real world application.

Some major design flaws in the compiler and bytecode architecture made 
me decide to do a FULL rewrite. The new version is nothing like the 
previous version in any way. Why not just create a new project?
I like the name :) 


**PORTABILITY**

Scrap will run on all platforms eventually, but for now it is only
tested on Linux. It will probably work without a hitch on OS X, but
some reconfiguration of the CMakeLists.txt file will be required for
full Windows-support.


**BUILDING**

Scrap uses GoogleTest for unit testing, and this must be configured
and built for the configuration to work properly. The googletest
framework is found in the "gtest" directory, and the built library
is expected to be found in "gtest/build". Execute the following
commands:

	$ cd gtest

	$ mkdir build

	$ cd build

	$ cmake ..


If all went down successfully, you may now build the Scrap binaries.
Go back to the root folder of the project and execute the following
commands:

	$ cmake .

	$ make
	
	Alternatively:

	$ make test

	$ make scrap


The tests can be run by executing the "test" binary, and you may test
Scrap by executing the "scrap" binary.

