scrap
=====

The crappy scripting language Scrap. This is a hobby exercise for 
compiler development, and should for the foreseeable future  be 
disregarded on all grounds in other terms of real world application.

Scrap aims to integrate easily with C++ *(potentially C as well)*. The
syntax is not yet decided upon, but the functionality of the language
will be similar to that of Java and C#. Unlike these languages, Scrap
will be developed with ease of use from the command line while also
utilizing precompiled binaries.


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
	

The tests can be run by executing the *test* binary, and you may test
Scrap by executing the *scrap* binary.


**State of Development**

Currently, execution of manually created bytecode is being developed.
Once this is in place, the compiler will be developed. 


**Contribution**

Check out the issue tracker *(or send me an e-mail if I've been lazy
with updating it)*.
