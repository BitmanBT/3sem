#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SEND_MESSAGE 1
#define RECIEVE_MESSAGE 2

int main()
{
	int msqid = 0;
	char pathname[] = "ForKey.txt";
	key_t key = 0;
	size_t len = 0;
	struct MyMsgBuf
	{
		long mtype;
		int mnumbers[1];
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

	MyBuf.mtype = SEND_MESSAGE;
	len = 1;
	printf("Enter your number: ");
	scanf("%d", &MyBuf.mnumbers[0]);
		
	if ((msgsnd(msqid, &MyBuf, len, 0)) < 0)
	{
		printf("Can\'t send message to queue\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds*) NULL);
		exit(-1);
	}
	
	printf("It was sent\n");
	
	size_t check = 0;
	
	while (1)
	{
		check = msgrcv(msqid, &MyBuf, len, 0, 0);
		
		if (check < 0)
		{
			printf("Can\'t recieve message from queue\n");
			exit(-1);
		}
		
		if (MyBuf.mtype == RECIEVE_MESSAGE)
		{
			printf("Answer: %d\n", MyBuf.mnumbers[0]);
			return 0;
		}
	}
}
