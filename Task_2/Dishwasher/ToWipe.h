#ifndef TOWIPE_H
#define TOWIPE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>
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

struct Threads
{
	char* dishes[4];
	int time[4];
	int number[4];
	int i; int j;
	int table;
	struct ForMsg msgto;
	struct ForMsg msgfrom;
};

void GetAccessToMsgBuffer(struct ForMsg* msg)
{
	if ((msg->key = ftok(msg->pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if ((msg->msqid = msgget(msg->key, 0666 | IPC_CREAT)) < 0)
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

void* Wipe(struct Threads* info)
{
	int check = 0;

	while (info->j != info->number[info->i])
	{
		sleep(info->time[info->i]);

		if (info->table > 0)
		{
			if ((check = msgsnd((info->msgto).msqid, &(info->msgto).MyBuf, (info->msgto).len, 0)) < 0)
			{
				printf("Can\'t send message to queue\n");
				msgctl((info->msgto).msqid, IPC_RMID, (struct msqid_ds*)NULL);
				exit(-1);
			}
			else {
				(info->j)++;
				printf("Wiped a %s and removed it from the table. It was %d\n", info->dishes[info->i], info->j);
				(info->table)--;
			}
		}
	}
}

void* WaitForMessage(struct Threads* info)
{
	int check = 0;

	while (info->j != info->number[info->i])
	{
		if ((check = msgrcv((info->msgfrom).msqid, &(info->msgfrom).MyBuf, (info->msgfrom).len, 0, 0)) < 0)
		{
			printf("Can\'t recieve message from queue\n");
			exit(-1);
		}
		else {
			if ((info->msgfrom).MyBuf.mtype == RECIEVE_MESSAGE)
			{
				(info->table)++;
			}
		}
	}
}

#endif // TOWIPE_H
