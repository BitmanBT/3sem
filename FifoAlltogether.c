#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

void ChooseTerminal (char* a, char* b, char* buffer, char* buffer_1, int fd1, int fd2) {
	pid_t result = fork();
	
	if (result < 0) {
		printf("Can't fork children\n");
		exit(-1);
	}
		
	if (result > 0) {
		fd1 = open(a, O_WRONLY);
		
		while (1) {
			fgets(buffer, 100, stdin);
			write(fd1, buffer, 100);
		}
	} else {
		fd2 = open(b, O_RDONLY);
	
		while (1) {
			read(fd2, buffer_1, 100);
			printf("%s", buffer_1);
		}
	}
}

int main(int argc, char* argv[]) {
	(void)umask(0);
	
	char a[] = "a.fifo";
	char b[] = "b.fifo";
	
	char buffer[100];
	char buffer_1[100];
	
	int fd1, fd2;
	
	if (mknod(a, S_IFIFO | 0666, 0) < 0) {
		printf("Can't create FIFO\n");
	}
	
	if (mknod(b, S_IFIFO | 0666, 0) < 0) {
		printf("Can't create FIFO\n");
	}
	
<<<<<<< HEAD
=======
	pid_t result = fork();
	 
	if (result < 0) {
		printf("Can't fork children\n");
		exit(-1);
	}
	
	int fd1, fd2;
	// TODO: куски кода с 31 строки по 49ю и с50 по 64ю почти не отличаются. необходимо избавиться от дублирования кода.
>>>>>>> ce5e484ee8b1b032e9f85da67f1df6da65a25940
	if (atoi(argv[1]) == 0) {
		ChooseTerminal(a, b, buffer, buffer_1, fd1, fd2);
	} else if (atoi(argv[1]) == 1) {
		ChooseTerminal(b, a, buffer, buffer_1, fd2, fd1);
	}
	
	return 0;
}
