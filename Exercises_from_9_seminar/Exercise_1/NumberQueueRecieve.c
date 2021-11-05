#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_MESSAGE 3
#define MAX_MASSIVE_SIZE 3
#define RECIEVE_MESSAGE 1
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
	
	len = MAX_MASSIVE_SIZE;
	
	while (1)
	{
		if ((msgrcv(msqid, (struct msgbuf*) &MyBuf, len, 0, 0)) < 0)
		{
			printf("Can\'t recieve message from queue\n");
			exit(-1);
		}
		
		if (MyBuf.mtype == LAST_MESSAGE)
		{
			msgctl(msqid, IPC_RMID, (struct msqid_ds*) NULL);
			exit(0);
		}
		
		printf("Message type = %ld, info = %f %f %f\n", MyBuf.mtype, (float)MyBuf.mnumbers[0], (float)MyBuf.mnumbers[1], (float)MyBuf.mnumbers[2]);
	}
	
	return 0;
}
