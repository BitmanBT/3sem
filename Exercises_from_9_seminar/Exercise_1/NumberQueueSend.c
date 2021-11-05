#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_MESSAGE 3
#define MAX_MASSIVE_SIZE 3
#define SEND_MESSAGE 1
#define LAST_MESSAGE 255

int main()
{
	int msqid = 0;
	char pathname[] = "ForKey.txt";
	key_t key = 0;
	size_t len = 0;
	struct MyMsgBuf
	{
		long mtype;
		float mnumbers[MAX_MASSIVE_SIZE];
	} MyBuf;
	
	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}
	
	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can\'t get msqid\n");
		exit(-1);
	}
	
	int i = 0, j = 0;
	MyBuf.mtype = SEND_MESSAGE;
	len = MAX_MASSIVE_SIZE;
	
	for (i = 0; i <= NUMBER_OF_MESSAGE; i++)
	{
		for (j = 0; j < MAX_MASSIVE_SIZE; j++)
			MyBuf.mnumbers[j] = j;
		
		if ((msgsnd(msqid, (struct msgbuf*) &MyBuf, len, 0)) < 0)
		{
			printf("Can\'t send message to queue\n");
			msgctl(msqid, IPC_RMID, (struct msqid_ds*) NULL);
			exit(-1);
		}
	}
	
	MyBuf.mtype = LAST_MESSAGE;
	len = 0;
	
	if ((msgsnd(msqid, (struct msgbuf*) &MyBuf, len, 0)) < 0)
	{
		printf("Can\'t send message to queue\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds*) NULL);
		exit(-1);
	}
	
	printf("That's OK\n");
	
	return 0;
}
