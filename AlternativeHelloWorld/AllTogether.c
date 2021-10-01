#include <stdio.h>
#include <unistd.h>

int main() {
	printf("\t\tAlternativeHelloWorld\n \
		This program creates a process which complile the HelloWorld program \
		And then it is launching the compiled HelloWorld program \
		Made by Gainullin Ivan, 2021");	
	
	int pid = fork();
	
	if (pid == -1) {
		printf("There are some problems with creating a process on the line %d.\n", __LINE__);
	} else if (pid == 0) {
		execlp("gcc", "gcc", "HelloWorld.c", "-o", "HelloWorld", NULL);
	} else {
		execlp("./HelloWorld", NULL);
	}
	
	printf("Error\n");
	
	return 0;
}
