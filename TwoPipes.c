#include <stdio.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd[2];
	int fd_second[2];
	int a[] = {2, 3};
	int sum = 0;
	int b[2];
	int sum_new;
	
	pipe(fd);
	pipe(fd_second);
	
	int ptr = fork();
	
	if (ptr == 0) {
		close(fd[0]);
		write(fd[1], a, 2 * sizeof(int));
		close(fd[1]);
		
		close(fd_second[1]);
		read(fd_second[0], &sum, 1 * sizeof(int));
		close(fd[0]);
		
		printf("%d\n", sum);
	} else {
		close(fd[1]);
		read(fd[0], b, 2 * sizeof(int));
		close(fd[0]);
		
		sum_new = b[0] + b[1];
		
		close(fd_second[0]);
		write(fd_second[1], &sum_new, 1 * sizeof(int));
		close(fd[1]);
	}
	
	return 0;
}
