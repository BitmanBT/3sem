#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <dirent.h>

void findFile(char* filename, char* currentDir, int depth, int* foundFilesCount, char** buffer)
{
	DIR* direct = opendir(currentDir);
	struct dirent* dir;

	while (dir = readdir(direct))
	{
		if ((strcmp(".", dir->d_name) != 0) && (strcmp("..", dir->d_name) != 0) && (strcmp(".vs", dir->d_name)))
		{
			if ((dir->d_type == 4) && (depth != 0))
			{
				char* newDir = (char*)malloc((strlen(currentDir) + 1 + strlen(dir->d_name)) * sizeof(char));
				strcat(newDir, currentDir);
				strcat(newDir, "/");
				strcat(newDir, dir->d_name);

				findFile(filename, newDir, depth - 1, foundFilesCount, buffer);
			}
			else if (strcmp(filename, dir->d_name) == 0)
			{
				buffer[*foundFilesCount] = (char*)malloc((strlen(currentDir) + 1 + strlen(dir->d_name)) * sizeof(char));
				strcat(buffer[*foundFilesCount], currentDir);
				strcat(buffer[*foundFilesCount], "/");
				strcat(buffer[*foundFilesCount], dir->d_name);
				*foundFilesCount += 1;
			}
		}
	}
}

int main(int argc, char* argv[]) //First - enter current directory, then - depth, then - the name of the file
{
	printf("\t");
	printf("\tThis is a program called FileSeeker.\n \
		It is going to look through your directories in order to find the file you want.\n \
		Made by Gainullin Ivan, 2021\n");

	char* currentDir = (char*)malloc(strlen(argv[1]) * sizeof(char));
	strcpy(currentDir, argv[1]);
	int depth = atoi(argv[2]);
	char* filename = (char*)malloc(strlen(argv[3]) * sizeof(char));
	strcpy(filename, argv[3]);
	int foundFilesCount = 0;
	char** buffer = (char**)malloc(100000 * sizeof(char));

	findFile(filename, currentDir, depth, &foundFilesCount, buffer);

	int i = 0;
	for (i = 0; i < foundFilesCount; i++)
	{
		puts(buffer[i]);
		free(buffer[i]);
	}

	free(buffer);
	free(currentDir);
	free(filename);
	
	return 0;
}