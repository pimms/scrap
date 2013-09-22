# This is an example program showing the functionalities
# of SCRAP. This program will compile and run in its entirety.

# Includes other script files. This MUST be done
# prior to any other code.
# The included scripts are _NOT_ executed. However,
# all functions and classes are available in the
# including file.
include "helloworld.sc";

# Class definition
class Foo {
	var x;
	var y; 
	var name;
	var age;

	# No parameters are given into the constructor.
	# However, variables are assigned BEFORE this
	# method is called.
	func Foo() {
		print("I just got created, and my name is %name");
	}

	func sayHello(var callerName) {
		print("Hello %callerName!");
		print("My name is %name, and");
		print("I am %age years old.");
	}
}

# Dynamic creation of anonymous classes
var bar = (var name, var age);
bar.name = "ole pettersen";
bar.age = 17;

# Create a new instance of class 'foo'. 
# The following lines will output:
# 		I just got created, and my name is heidi
#		Hello ole pettersen!
#		My name is heidi, and
#		I am 29 years old.
var fooInstance = Foo(name = "heidi", age = 29);
fooInstance.sayHello(bar.name);

# Types are weak
var anotherFoo = Foo(13, 12, "probably something", "a non-zero amount of");
anotherFoo.sayHello("jello");

func multiply(var a, var b) {
	return a * b;
}

# The percent sign (%) can be used for function and variable
# substitution.
print("%multiply(3, 4) = 3*4");
print("%multiply(bar.age, 2) = 2*17");


func fibonacci(var n) {
	if (n <= 1) {
		return 1;
	}

	return fibonacci(n-1) + fibonacci(n-2);
}

for (var i=0; i<10; i++) {
	print("fib(%i) = %fibonacci(i)");
}