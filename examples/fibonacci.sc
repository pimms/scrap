var num = 10;

fib(num);

func fib(var n) {
	if (n <= 1) {
		return 1;
	} else {
		return fib(n-1) + fib(n-2);
	}
}