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

void Binding(int* sockfd, struct sockaddr_in* cliaddr, struct sockaddr_in* servaddr, char* toBind)
{
	if ((*sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	}

	bzero(cliaddr, sizeof(*cliaddr));
	cliaddr->sin_family = AF_INET;
	cliaddr->sin_port = htons(0);
	cliaddr->sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(*sockfd, (struct sockaddr*) cliaddr, sizeof(*cliaddr)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}

	bzero(servaddr, sizeof(*servaddr));
	servaddr->sin_family = AF_INET;
	servaddr->sin_port = htons(51000);

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

void Ready(int* sockfd, struct sockaddr_in* servaddr, char* sendline)
{
	printf("Press a button if you are ready to stroke back\n");

	fgets(sendline, 1000, stdin);

	if (sendto(*sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr*)servaddr, sizeof(*servaddr)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void SendInfo(int* sockfd, struct sockaddr_in* servaddr, char* sendline)
{
	fgets(sendline, 1000, stdin);

	if (sendto(*sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr*) servaddr, sizeof(*servaddr)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void RcvSig(int* sockfd, char* recvline)
{
	int n;

	if ((n = recvfrom(*sockfd, recvline, 1000, 0, (struct sockaddr*)NULL, NULL)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}

	printf("%s\n", recvline);
}

void RcvInfo(int* sockfd, char* recvline, char* A)
{
	int n;

	if ((n = recvfrom(*sockfd, recvline, 1000, 0, (struct sockaddr*)NULL, NULL)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}

	strcpy(A, recvline);
	A[9] = '\0';
	
	PrintMatrix(A);
}

void SendStartInfo(int* sockfd, struct sockaddr_in* servaddr, char* sendline, char* recvline)
{
	SendInfo(sockfd, servaddr, sendline);

	RcvSig(sockfd, recvline);
}

void CheckVic(int* sockfd, char* recvline, bool* timeToFinish, struct sockaddr_in* servaddr, char* sendline)
{
	strcpy(sendline, "0");

	if (sendto(*sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr*)servaddr, sizeof(*servaddr)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
	
	int n;

	if ((n = recvfrom(*sockfd, recvline, 1000, 0, (struct sockaddr*)NULL, NULL)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}

	char num = recvline[0];

	if (num == '1')
	{
		*timeToFinish = true;
		printf("You lose\n");
	}
	else if (num == '2')
	{
		*timeToFinish = true;
		printf("You win\n");
	}
}

#endif // CLIENT_H