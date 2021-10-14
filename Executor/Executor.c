#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUMBER_OF_SYMBOLS_FOR_BUFFER 100
#define MAX_NUMBER_OF_WORDS_IN_COMMANDS 4

int main() {
	printf("\t\tExecutor\n \
		This programm executes other programs written in the file\n \
		Called CommandText.txt\n \
		Made by Gainullin Ivan, 2021\n");
	
	FILE* ptrFile = fopen("CommandFile.txt", "r");
	
	char buffer[MAX_NUMBER_OF_SYMBOLS_FOR_BUFFER];
	
	if (ptrFile == 0) {
		printf("File opening error\n");
		return 0;
	}
	
	fgets(buffer, MAX_NUMBER_OF_SYMBOLS_FOR_BUFFER, ptrFile);
	
	int N = atoi(buffer);
	
	char** pointersToCommands = (char**) calloc(N, sizeof(char*));
	
	int i = 0;
	pointersToCommands[0] = strtok(buffer, "\n");
	for (i = 1; i < N; i++) {
		pointersToCommands[i] = strtok(NULL, "\n");
	}
	
	for (i = 1; i < N; i++) {
		char* ToExecute[MAX_NUMBER_OF_WORDS_IN_COMMANDS];
		char* pointersToWords = strtok(pointersToCommands[i], " ");
		ToExecute[0] = pointersToWords;
		
		int j = 1;
		while (pointersToWords != NULL) {
			pointersToWords = strtok(NULL, " ");
			ToExecute[j] = pointersToWords;
			j++;
		}
		// TODO: как только добъете ф-ю split, то здесь стоит её переиспользовать 
		
		pid_t pid = fork();
		
		if (pid == -1) {
			printf("There are some problems with creating child processes on the line %d.\n", __LINE__);
		} else if (pid == 0) {
			// TODO стоит воспользоваться вызовом execvp, т.к. мы не знает заранее, сколько входных аргументов есть у ф-и
			execlp(ToExecute[0], ToExecute[0], ToExecute[1], ToExecute[2], ToExecute[3], NULL);
		} else {
			// TODO: пробовали во входной файл писать несколько команд: 2 и более? кажется, что работать не должна 2я и последующие запуститься.
			exit(0);
		}
	}
	
	fclose(ptrFile);
	
	return 0;
}
