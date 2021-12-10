#include "ToWash.h"

int main()
{
	struct Threads info;

	strcpy(info.msgto.pathname, "ForFirstKey.txt");

	GetAccessToMsgBuffer(&(info.msgto));

	info.msgto.MyBuf.mtype = SEND_MESSAGE;

	info.msgto.len = sizeof(struct MyMsgBuf);
	info.msgto.MyBuf.mnumbers[0] = 1;

	strcpy(info.msgfrom.pathname, "ForSecondKey.txt");

	GetAccessToMsgBuffer(&(info.msgfrom));

	info.msgfrom.MyBuf.mtype = RECIEVE_MESSAGE;

	info.msgfrom.len = sizeof(struct MyMsgBuf);
	info.msgfrom.MyBuf.mnumbers[0] = 1;

	FILE* towash = fopen("ToWash.txt", "r");
	FILE* dirty = fopen("Dirty.txt", "r");

	int i = 0;
	for (i = 0; i < 4; i++)
		info.dishes[i] = (char*) malloc(10*sizeof(char));
		
	GetTime(info.dishes, info.time, towash);

	for (i = 0; i < 4; i++)
		info.number[i] = 0;
	GetNumber(info.dishes, info.number, dirty);

	info.table = TABLE_LIMIT;

	pthread_t thid_first, thid_second;
	int result;

	for (info.i = 0; info.i < 4; (info.i)++)
	{
		info.j = 0;

		result = pthread_create(&thid_first, (pthread_attr_t*) NULL, Wash, &info);

		if (result != 0)
		{
			printf("Error on thread create, return value = %d\n", result);
			exit(-1);
		}

		result = pthread_create(&thid_second, (pthread_attr_t*)NULL, WaitForMessage, &info);

		if (result != 0)
		{
			printf("Error on thread create, return value = %d\n", result);
			exit(-1);
		}

		pthread_join(thid_first, (void**)NULL);
		pthread_join(thid_second, (void**)NULL);
	}

	return 0;
}
