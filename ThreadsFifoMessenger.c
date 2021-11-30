#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct to_send
{
	char a[7];
	char b[7];
};

void* ChooseTerminal_get (struct to_send* FD)
{
	int fd1 = open(FD->a, O_WRONLY);
	char buffer[100];
		
	while (1) {
		fgets(buffer, 100, stdin);
		write(fd1, buffer, 100);
	}
}

void* ChooseTerminal_print (struct to_send* FD)
{
	int fd2 = open(FD->b, O_RDONLY);
	char buffer_1[100];
	
	while (1) {
		read(fd2, buffer_1, 100);
		printf("%s", buffer_1);
	}
}

void ChooseTerminal(struct to_send* FD)
{
	pthread_t thid, thid_new;
	int result = pthread_create(&thid, (pthread_attr_t *)NULL, ChooseTerminal_get, FD);
		
	if(result != 0) {
		printf ("Error on thread create, return value = %d\n", result);
		exit(-1);
	}
		
	printf("Thread created, thid = %d\n", thid);
	
	int result_new = pthread_create(&thid_new, (pthread_attr_t *)NULL, ChooseTerminal_print, FD);
		
	if(result_new != 0) {
		printf ("Error on thread create, return value = %d\n", result);
		exit(-1);
	}
		
	printf("Thread created, thid = %d\n", thid_new);
	
	pthread_join(thid, (void **)NULL);
	pthread_join(thid_new, (void **)NULL);
}

int main(int argc, char* argv[]) {
	(void)umask(0);

	struct to_send FD;

	strcpy(FD.a, "a.fifo");
	strcpy(FD.b, "b.fifo");
	
	if (mknod(FD.a, S_IFIFO | 0666, 0) < 0)
	{
		printf("Can't create FIFO\n");
	}
	
	if (mknod(FD.b, S_IFIFO | 0666, 0) < 0)
	{
		printf("Can't create FIFO\n");
	}

	if (atoi(argv[1]) == 0)
	{
		ChooseTerminal(&FD);
	} 
	else if (atoi(argv[1]) == 1)
	{
		strcpy(FD.a, "b.fifo");
		strcpy(FD.b, "a.fifo");
		ChooseTerminal(&FD);
	}
	
	return 0;
}
