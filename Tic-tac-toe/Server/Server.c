#include "Server.h"

int main()
{	
	int sockfd;
	struct sockaddr_in servaddr;
	struct cliinfo first_player, second_player;

	BindServer(&sockfd, &servaddr);

	first_player.clilen = sizeof(first_player.cliaddr);
	second_player.clilen = sizeof(second_player.cliaddr);

	SendStartInfo(&sockfd, &first_player, &second_player);

	bool cross_vic = false, zero_vic = false, draw = false;

	char A[9]; int i;
	for (i = 0; i < 9; i++)
		A[i] = '-';

	while (1)
	{
		Stroke(&sockfd, &first_player, &second_player, A, &cross_vic, &zero_vic, &draw);
	}

	return 0;
}