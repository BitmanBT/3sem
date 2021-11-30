#include "Split.h"

#define MAX_LENGTH 100

int main()
{
	char* string = (char*)malloc(MAX_LENGTH * sizeof(char));
	gets(string);

	char* del = " \t";

	char** tokens = (char**)malloc(MAX_LENGTH * sizeof(char));
	size_t tokensCount = 0;

	Split(string, del, tokens, &tokensCount);

	int i = 0;
	for (i = 0; i < tokensCount; i++)
	{
		printf("%s\n", tokens[i]);
	}

	return 0;
}