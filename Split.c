#include <stdio.h>
#include <string.h>
#include <malloc.h>

void Split(char* string, char* delimiters, char*** tokens, int* tokensCount);

int main() {
	char* string; char* delimiters;
	int tokensCount; char** tokens;
	
	printf("\t\tThis is program called Split.\n \
		It demonstrates how the function Split works.\n \
		Made by Ivan Gainullin, 2021\n");
	printf("Enter the string: ");
	
	string = (char*) malloc(sizeof(char));
	
	int i = 0; int mem;
	while ((mem = getchar()) != '\n') {
		if (i != 0)
			string = (char*) realloc(string, strlen(string) + sizeof(char));
		string[i] = mem;
		i++;
	}
	
	printf("Enter delimiters: ");
	
	delimiters = (char*) malloc(sizeof(char));
	
	i = 0;
	while ((mem = getchar()) != '\n') {
		if (i != 0)
			delimiters = (char*) realloc(delimiters, strlen(delimiters) + sizeof(char));
		delimiters[i] = mem;
		i++;
	}
	
	tokens = (char**) malloc(sizeof(char**));
	
	Split(string, delimiters, &tokens, &tokensCount);
	
	printf("Your splitted string:\n");
	int j;
	for (i = 0; i < tokensCount; i++) {
		for (j = 0; j < strlen(tokens[i]); j++) {
			printf("%c", *(tokens[i] + j));
		}
		printf("\n");
	}
	
	free(string);
	free(delimiters);
	free(tokens);
	
	return 0;
}

void Split(char* string, char* delimiters, char*** tokens, int* tokensCount) {
	int i = 0; *tokensCount = 0;
	
	char* savetokens = string;
	while (*(*tokens + i) = strtok_r(savetokens, delimiters, &savetokens)) {
		if (i != 0)
			*tokens = (char**) realloc(*tokens, i + 1);
		i++;
		(*tokensCount)++;
	}
}
