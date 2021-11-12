#ifndef TOWIPE_H
#define TOWIPE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <stdio.h>

#define TABLE_LIMIT 3
#define SEND_MESSAGE 2
#define RECIEVE_MESSAGE 1

struct MyMsgBuf
{
	long mtype;
	int mnumbers[1];
};

struct ForMsg
{
	int msqid;
	char pathname[20];
	key_t key;
	size_t len;
	struct MyMsgBuf MyBuf;
};

void GetAccessToMsgBuffer(struct ForMsg* msg)
{
	key_t key;
	if ((key = ftok(msg->pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}

	int msqid;
	if ((msqid = msgget(msg->key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can\'t get msqid\n");
		exit(-1);
	}
}

void GetTime(char** dishes, int* time, FILE* towipe)
{
	int i = 0;
	while (fscanf(towipe, "%s : %d", dishes[i], &time[i]) != EOF)
		i++;
}

void GetNumber(char** dishes, int* number, FILE* dirty)
{
	int i = 0; int j = 0;
	int letter; int count = 0;
	
	while ((letter = fgetc(dirty)) != EOF)
	{
		if (letter == '\n')
			count++;
	}
	fseek(dirty, 0, SEEK_SET);
	
	char* get_dishes[count]; int get_number[count];
	for (i = 0; i < count; i++)
	{
		get_dishes[i] = (char*) malloc(10*sizeof(char));
		get_number[i] = 0;
	}
	
	i = 0;
	while (fscanf(dirty, "%s : %d", get_dishes[i], &get_number[i]) != EOF)
		i++;
	
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < count; j++)
		{
			if (strcmp(dishes[i], get_dishes[j]) == 0)
				number[i] += get_number[j];
		}
	}
}

#endif // TOWIPE_H
