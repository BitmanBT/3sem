#ifndef TOWIPE_H
#define TOWIPE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TABLE_LIMIT 3
#define	SEND_MESSAGE 2
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
	if ((key = ftok(msg->pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if ((msqid = msgget(msg->key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can\'t get msqid\n");
		exit(-1);
	}
}

void GetTime(char*** dishes, int** time, FILE* towash)
{
	int i = 0;
	while (fscanf(towash, "%s: %d", dishes[i], time[i]) != NULL)
		i++;
}

void GetNumber(int** number, FILE* dirty)
{
	char letter; int j = 0; int check = 1;
	for (i = 0; i < 4; i++)
	{
		while ((letter = fgetc(dirty)) != EOF)
		{
			if (letter == dishes[i][j])
			{
				for (j = 1; j < strlen(dishes[i]); j++)
					check++;
				if (check == strlen(dishes[i]))
				{
					fseek(dirty, 2, SEEK_CUR);
					*number[i] += atoi(fgetc(dirty));
				}
			}
			j = 0;
		}
		fseek(dirty, 0, SEEK_SET);
	}
}

#endif / TOWIPE_H