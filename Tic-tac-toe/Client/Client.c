#include "Client.h"

int main(int argc, char** argv)
{	
	int sockfd;
	struct sockaddr_in servaddr;
	struct cliinfo player;

	if (argc != 2)
	{
		printf("Usage: a.out  <IP address>\n");
		exit(1);
	}

	char* toBind = (char*)malloc(strlen(argv[1])*sizeof(char));
	strcpy(toBind, argv[1]);
	BindServer(&sockfd, &player, &servaddr, toBind);
	
	GetStartInfo(&sockfd, &player, &servaddr);

	char A[9]; int i;
	for (i = 0; i < 9; i++)
		A[i] = '-';

	while (1)
	{
		GetSig(&sockfd, &player, &servaddr, A);
	}

	close(sockfd);

	return 0;
}