#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Split(char* string, char* del, char** tokens, size_t* tokensCount)
{
	string = strtok(string, del);

	while (string != NULL)
	{
		tokens[*tokensCount] = string;
		(*tokensCount)++;

		string = strtok(NULL, del);
	}
}