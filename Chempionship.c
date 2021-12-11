#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

pid_t pid;

void my_handler_1(int nsig)
{
	printf("RT - ");
	kill(getpid(), SIGUSR2);
}

void my_handler_2(int nsig)
{
	printf("champion!\n");
	kill(pid, SIGUSR1);
}

int main()
{
	(void*)signal(SIGUSR1, my_handler_1);
	(void*)signal(SIGUSR2, my_handler_2);

	kill(pid, SIGUSR2);

	pid = fork();

	if (pid == 0)
	{
		while (1)
		{}
	}
	else
	{
		while (1)
		{}
	}

	return 0;
}