#include "ToWash.h"

int main()
{
	struct ForMsg msgto;
	strcpy(msgto.pathname, "ForFirstKey.txt");

	GetAccessToMsgBuffer(&msgto);

	msgto.MyBuf.mtype = SEND_MESSAGE;

	msgto.len = sizeof(struct MyMsgBuf);
	msgto.MyBuf.mnumbers[0] = 1;

	struct ForMsg msgfrom;
	strcpy(msgfrom.pathname, "ForSecondKey.txt");

	GetAccessToMsgBuffer(&msgfrom);

	FILE* towash = fopen("ToWash.txt", "r");
	FILE* dirty = fopen("Dirty.txt", "r");

	char* dishes[4]; int time[4];
	GetTime(&dishes, &time, towash);

	int number[4];
	int i = 0;
	for (i = 0; i < 4; i++)
		number[i] = 0;
	GetNumber(&dishes, &number, dirty);

	int table = TABLE_LIMIT;
	int j = 0; int check = 0;
	for (i = 0; i < 4; i++)
	{
		j = 0;

		while (j != number[i])
		{
			if ((check = msgrcv(msgfrom.msqid, &msgfrom.MyBuf, msgfrom.len, 0, 0)) < 0)
			{
				printf("Can\'t recieve message from queue\n");
				exit(-1);
			}
			else {
				if (msgfrom.MyBuf.mtype == RECIEVE_MESSAGE)
				{
					table++;
				}
			}

			sleep(time[i]);

			if (table > 0)
			{
				if ((check = msgsnd(msgto.msqid, &msgto.MyBuf, msgto.len, 0)) < 0)
				{
					printf("Can\'t send message to queue\n");
					msgctl(msgto.msqid, IPC_RMID, (struct msqid_ds*)NULL);
					exit(-1);
				}
				else {
					j++;
					printf("Washed a %s and put it on the table. It was %d", dishes[i], j);
					table--;
				}
			}
		}
	}

	return 0;
}
