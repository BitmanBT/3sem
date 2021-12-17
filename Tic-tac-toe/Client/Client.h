#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

#define RCV_MES '0'
#define RCV_INFO '1'
#define SEND_INFO '2'
#define FINISH '3'

struct cliinfo
{
	int clilen;
	char line[1000];
	struct sockaddr_in cliaddr;
};

void BindServer(int* sockfd, struct cliinfo* player, struct sockaddr_in* servaddr, char* toBind)
{
	if ((*sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	}

	bzero(&(player->cliaddr), sizeof(player->cliaddr));
	(&(player->cliaddr))->sin_family = AF_INET;
	(&(player->cliaddr))->sin_port = htons(0);
	(&(player->cliaddr))->sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(*sockfd, &(player->cliaddr), sizeof(player->cliaddr)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}

	bzero(servaddr, sizeof(*servaddr));
	servaddr->sin_family = AF_INET;
	servaddr->sin_port = htons(8888);

	if (inet_aton(toBind, &servaddr->sin_addr) == 0)
	{
		printf("Invalid IP address\n");
		close(*sockfd);
		exit(1);
	}
}

void PrintMatrix(char* A)
{
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			printf("%c ", A[3*i+j]);
		}
		printf("\n");
	}
}

void RcvMessage(int* sockfd, struct cliinfo* player)
{
	bzero(player->line, 1000);
	
	int n;

	if ((n = recvfrom(*sockfd, player->line, 1000, 0, NULL, NULL)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}

	printf("%s\n", player->line);
}

void RcvInfo(int* sockfd, struct cliinfo* player, char* A)
{
	bzero(player->line, 1000);
	
	int n;

	if ((n = recvfrom(*sockfd, player->line, 1000, 0, NULL, NULL)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}

	strcpy(A, player->line);
	A[9] = '\0';

	PrintMatrix(A);
}

void SendInfo(int* sockfd, struct cliinfo* player, struct sockaddr_in* servaddr)
{
	fgets(player->line, 1000, stdin);

	if (sendto(*sockfd, player->line, strlen(player->line) + 1, 0, servaddr, sizeof(*servaddr)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void SendSig(int* sockfd, struct cliinfo* player, struct sockaddr_in* servaddr)
{
	strcpy(player->line, "0");

	if (sendto(*sockfd, player->line, strlen(player->line) + 1, 0, servaddr, sizeof(*servaddr)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void GetStartInfo(int* sockfd, struct cliinfo* player, struct sockaddr_in* servaddr)
{
	SendSig(sockfd, player, servaddr);
	
	RcvMessage(sockfd, player);
}

void GetSig(int* sockfd, struct cliinfo* player, struct sockaddr_in* servaddr, char* A)
{
	bzero(player->line, 1000);

	int n;

	if ((n = recvfrom(*sockfd, player->line, 1000, 0, NULL, NULL)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}

	char sig = (player->line)[0];

	if (sig == RCV_MES)
	{
		RcvMessage(sockfd, player);
	}

	if (sig == RCV_INFO)
	{
		RcvInfo(sockfd, player, A);
	}

	if (sig == SEND_INFO)
	{
		SendInfo(sockfd, player, servaddr);
	}

	if (sig == FINISH)
	{
		exit(0);
	}
}

#endif // CLIENT_H