#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(){
	syscall(0x01, 1, "Hello world\n", 13);
	return 0;
}
