#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define RECIEVE_MESSAGE 1
#define SEND_MESSAGE 2

int factorial(int n)
{
	int i = 0;
	int res = 1;
	
	for (i = 2; i <= n; i++)
	{
		res *= i;
	}
	
	return res;
}

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
	
	len = 1;
	MyBuf.mtype = SEND_MESSAGE;
	size_t check = 0;
	
	printf("Wait\n");
	
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
			printf("Got it: %d\n", MyBuf.mnumbers[0]);
			
			int res = factorial(MyBuf.mnumbers[0]);
			
			MyBuf.mtype = SEND_MESSAGE;
			MyBuf.mnumbers[0] = res;
			
			if ((msgsnd(msqid, &MyBuf, len, 0)) < 0)
			{
				printf("Can\'t send message to queue\n");
				msgctl(msqid, IPC_RMID, (struct msqid_ds*) NULL);
				exit(-1);
			}
			
			return 0;
		}
	}
}
