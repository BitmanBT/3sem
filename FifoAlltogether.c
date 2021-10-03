#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	(void)umask(0);
	
	char a[] = "a.fifo";
	char b[] = "b.fifo";
	
	if (mknod(a, S_IFIFO | 0666, 0) < 0) {
		printf("Can't create FIFO\n");
	}
	
	if (mknod(b, S_IFIFO | 0666, 0) < 0) {
		printf("Can't create FIFO\n");
	}
	
	pid_t result = fork();
	 
	if (result < 0) {
		printf("Can't fork children\n");
		exit(-1);
	}
	
	int fd1, fd2;
	// TODO: куски кода с 31 строки по 49ю и с50 по 64ю почти не отличаются. необходимо избавиться от дублирования кода.
	if (atoi(argv[1]) == 0) {
		fd1 = open("a.fifo", O_WRONLY);
		fd2 = open("b.fifo", O_RDONLY);
		
		char buffer[100];
		char buffer_1[100];
		
		while (1) {
			if (result != 0) {
				fgets(buffer, 100, stdin);
				write(fd1, buffer, 100);
			} else {
				read(fd2, buffer_1, 100);
				printf("%s", buffer_1);
			}
		}
		
		return 0;
	} else if (atoi(argv[1]) == 1) {
		fd1 = open("a.fifo", O_RDONLY);
		fd2 = open("b.fifo", O_WRONLY);
		
		char buffer[100];
		char buffer_1[100];
		
		while (1) {
			if (result != 0) {
				fgets(buffer, 100, stdin);
				write(fd2, buffer, 100);
			} else {
				read(fd1, buffer_1, 100);
				printf("%s", buffer_1);
			}
		}
		
		return 0;
	}
}
