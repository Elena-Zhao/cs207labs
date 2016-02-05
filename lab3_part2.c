#include <stdio.h>
#include <stdint.h>

int fib(int n){
	if(n == 1){
		return 0;
	}
	if(n == 2){
		return 1;
	}
	return fib(n-1) + fib(n-2);
}
int main(){
	int n = 8;
	printf("fib=%d\n", fib(n));
}


