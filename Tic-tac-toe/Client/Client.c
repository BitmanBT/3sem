#include "Client.h"

int main(int argc, char** argv)
{
	printf("Welcome to Tic-tac-toe!\n");
	
	int sockfd; int n, len;
	char sendline[1000], recvline[1000];
	struct sockaddr_in servaddr, cliaddr;

	if (argc != 2)
	{
		printf("Usage: a.out  <IP address>\n");
		exit(1);
	}

	char* toBind = (char*)malloc(strlen(argv[1])*sizeof(char));
	strcpy(toBind, argv[1]);
	Binding(&sockfd, &cliaddr, &servaddr, toBind);

	printf("Choose what are you going to play with: 0 - to play with zeros, 1 - to play with crosses.\n\
		Your choice: ");
	
	SendStartInfo(&sockfd, &servaddr, sendline, recvline);

	bool timeToFinish = false;

	char A[10];

	while (timeToFinish == false)
	{
		Ready(&sockfd, &servaddr, sendline);
		RcvInfo(&sockfd, recvline, A);
		CheckVic(&sockfd, recvline, &timeToFinish, &servaddr, sendline);
		if (timeToFinish == false)
		{
			SendInfo(&sockfd, &servaddr, sendline);
			RcvInfo(&sockfd, recvline, A);
			CheckVic(&sockfd, recvline, &timeToFinish, &servaddr, sendline);
		}
	}

	close(sockfd);

	return 0;
}