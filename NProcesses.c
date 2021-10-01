#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
	printf("\t\tNProcesses\n \
		This program creates N different processes, each next is a child.\n \
		Each father waites for its child to die.\n \
		You are gonna see all the information.\n \
		Made by Gainullin Ivan, 2021\n\n");
	
	int N = 0;	
	printf("Enter N: ");
	scanf("%d", &N);
	
	int i = 0;
	
	for (i = 0; i < N; i++) {
		pid_t pid = fork();
		
		if (pid < 0) {
			printf("There are some problems with creating a child process on the line %d.\n", __LINE__);
		} else if (pid == 0) {
			printf("Hi! I am the child №%d. My pid is %d. My parent's pid is %d.\n", i, getpid(), getppid());
		} else {
			int wstatus = 0; int child_id = wait(&wstatus);
			
			if (child_id == -1)
				printf("There are some problems on the line %d.\n", __LINE__);
			
			printf("I was a parent and my child is dead :( His pid was %d. He was terminated with status %d.\n", child_id, wstatus);
			exit(0);
		}
	}
	
	return 0;
}
