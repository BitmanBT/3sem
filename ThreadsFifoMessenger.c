#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pthread.h>

char a[] = "a.fifo";
char b[] = "b.fifo";

void* ChooseTerminal_0_get (void* dummy) {
	int fd1 = open(a, O_WRONLY);
	char buffer[100];
		
	while (1) {
		fgets(buffer, 100, stdin);
		write(fd1, buffer, 100);
	}
}

void* ChooseTerminal_0_print (void* dummy) {
	int fd2 = open(b, O_RDONLY);
	char buffer_1[100];
	
	while (1) {
		read(fd2, buffer_1, 100);
		printf("%s", buffer_1);
	}
}

void* ChooseTerminal_1_get (void* dummy) {
	int fd1 = open(b, O_WRONLY);
	char buffer[100];
		
	while (1) {
		fgets(buffer, 100, stdin);
		write(fd1, buffer, 100);
	}
}

// TODO: у вас ChooseTerminal_0_get отличается от ChooseTerminal_1_get одним параметром. Нужно принять этот параметр в качестве входного аргумента и избавиться от дублирования кода тем самым

void* ChooseTerminal_1_print (void* dummy) {
	int fd2 = open(a, O_RDONLY);
	char buffer_1[100];
	
	while (1) {
		read(fd2, buffer_1, 100);
		printf("%s", buffer_1);
	}
}

void ChooseTerminal_0 () {
	pthread_t thid, thid_new;
	int result = pthread_create(&thid, (pthread_attr_t *)NULL, ChooseTerminal_0_get, NULL);
		
	if(result != 0) {
		printf ("Error on thread create, return value = %d\n", result);
		exit(-1);
	}
		
	printf("Thread created, thid = %d\n", thid);
	
	int result_new = pthread_create(&thid_new, (pthread_attr_t *)NULL, ChooseTerminal_0_print, NULL);
		
	if(result_new != 0) {
		printf ("Error on thread create, return value = %d\n", result);
		exit(-1);
	}
		
	printf("Thread created, thid = %d\n", thid_new);
	
	pthread_join(thid, (void **)NULL);
	pthread_join(thid_new, (void **)NULL);
}

void ChooseTerminal_1 () {
	pthread_t thid, thid_new;
	int result = pthread_create(&thid, (pthread_attr_t *)NULL, ChooseTerminal_1_get, NULL);
		
	if(result != 0) {
		printf ("Error on thread create, return value = %d\n", result);
		exit(-1);
	}
		
	printf("Thread created, thid = %d\n", thid);
	
	int result_new = pthread_create(&thid_new, (pthread_attr_t *)NULL, ChooseTerminal_1_print, NULL);
		
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
	
	if (mknod(a, S_IFIFO | 0666, 0) < 0) {
		printf("Can't create FIFO\n");
	}
	
	if (mknod(b, S_IFIFO | 0666, 0) < 0) {
		printf("Can't create FIFO\n");
	}
	
	if (atoi(argv[1]) == 0) {
		ChooseTerminal_0();
	} else if (atoi(argv[1]) == 1) {
		ChooseTerminal_1();
	}
	
	return 0;
}
