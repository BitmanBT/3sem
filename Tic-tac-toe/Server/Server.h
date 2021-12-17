#ifndef SERVER_H
#define SERVER_H

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

#define SEND_MES "0"
#define SEND_INFO "1"
#define RCV_INFO "2"
#define FINISH "3"

struct cliinfo
{
	int clilen;
	char line[1000];
	struct sockaddr_in cliaddr;
};

void BindServer(int* sockfd, struct sockaddr_in* servaddr)
{
	bzero(servaddr, sizeof(*servaddr));
	servaddr->sin_family = AF_INET;
	servaddr->sin_port = htons(8888);
	servaddr->sin_addr.s_addr = htonl(INADDR_ANY);

	if ((*sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	}

	if (bind(*sockfd, (struct sockaddr*) servaddr, sizeof(*servaddr)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void SendMessage(int* sockfd, struct cliinfo* player, char* message)
{	
	strcpy(player->line, message);

	if (sendto(*sockfd, player->line, strlen(player->line), 0, (struct sockaddr*) &(player->cliaddr), player->clilen) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void SendInfo(int* sockfd, struct cliinfo* player, char* A)
{	
	strcpy(player->line, A);

	if (sendto(*sockfd, player->line, strlen(player->line), 0, &(player->cliaddr), player->clilen) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void SendSig(int* sockfd, struct cliinfo* player, char* sig)
{
	strcpy(player->line, sig);

	if (sendto(*sockfd, player->line, strlen(player->line), 0, &(player->cliaddr), player->clilen) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void RcvInfo(int* sockfd, struct cliinfo* player, char* A, char sym)
{
	bzero(player->line, 1000);

	int n;

	if ((n = recvfrom(*sockfd, player->line, 999, 0, &(player->cliaddr), &player->clilen)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}

	int num = atoi(player->line); int ln = 0, col = 0;

	while (num >= 10)
	{
		ln++; num -= 10;
	}
	col = num;

	if (A[ln * 3 + col] == '-')
	{
		A[ln * 3 + col] = sym;
	}
	else
	{
		SendSig(sockfd, player, SEND_MES);
		SendMessage(sockfd, player, "Already usen. Try again.");
		
		SendSig(sockfd, player, RCV_INFO);
		RcvInfo(sockfd, player, A, sym);
	}
}

void RcvSig(int* sockfd, struct cliinfo* player)
{
	bzero(player->line, 1000);

	int n;

	if ((n = recvfrom(*sockfd, player->line, 999, 0, &(player->cliaddr), &player->clilen)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void SendStartInfo(int* sockfd, struct cliinfo* first_player, struct cliinfo* second_player)
{
	RcvSig(sockfd, first_player);
	
	SendMessage(sockfd, first_player, "Welcome to Tic-Tac-Toe!\nYou are first player and you are going to play with crosses.");

	RcvSig(sockfd, second_player);

	SendMessage(sockfd, second_player, "Welcome to Tic-Tac-Toe!\nYou are second player and you are going to play with zeros.");
}

void CheckVic(bool* cross_vic, bool* zero_vic, bool* draw, char* A)
{
	if ((A[0] == '0') && (A[1] == '0') && (A[2] == '0'))
		*zero_vic = true;
	if ((A[3] == '0') && (A[4] == '0') && (A[5] == '0'))
		*zero_vic = true;
	if ((A[6] == '0') && (A[7] == '0') && (A[8] == '0'))
		*zero_vic = true;
	if ((A[0] == '0') && (A[3] == '0') && (A[6] == '0'))
		*zero_vic = true;
	if ((A[1] == '0') && (A[4] == '0') && (A[7] == '0'))
		*zero_vic = true;
	if ((A[2] == '0') && (A[5] == '0') && (A[8] == '0'))
		*zero_vic = true;
	if ((A[0] == '0') && (A[4] == '0') && (A[8] == '0'))
		*zero_vic = true;
	if ((A[2] == '0') && (A[4] == '0') && (A[6] == '0'))
		*zero_vic = true;
	if ((A[0] == '1') && (A[1] == '1') && (A[2] == '1'))
		*cross_vic = true;
	if ((A[3] == '1') && (A[4] == '1') && (A[5] == '1'))
		*cross_vic = true;
	if ((A[6] == '1') && (A[7] == '1') && (A[8] == '1'))
		*cross_vic = true;
	if ((A[0] == '1') && (A[3] == '1') && (A[6] == '1'))
		*cross_vic = true;
	if ((A[1] == '1') && (A[4] == '1') && (A[7] == '1'))
		*cross_vic = true;
	if ((A[2] == '1') && (A[5] == '1') && (A[8] == '1'))
		*cross_vic = true;
	if ((A[0] == '1') && (A[4] == '1') && (A[8] == '1'))
		*cross_vic = true;
	if ((A[2] == '1') && (A[4] == '1') && (A[6] == '1'))
		*cross_vic = true;

	int count = 0, i = 0;
	for (i = 0; i < 9; i++)
	{
		if (A[i] != '-')
			count++;
	}

	if ((count == 9) && (*cross_vic == false) && (*zero_vic == false))
		*draw = true;
}

void Stroke(int* sockfd, struct cliinfo* first_player, struct cliinfo* second_player, char* A, bool* cross_vic, bool* zero_vic, bool* draw)
{
	SendSig(sockfd, first_player, SEND_MES);
	SendMessage(sockfd, first_player, "Your turn:");
	
	SendSig(sockfd, first_player, RCV_INFO);
	RcvInfo(sockfd, first_player, A, '1');

	SendSig(sockfd, first_player, SEND_INFO);
	SendInfo(sockfd, first_player, A);

	SendSig(sockfd, second_player, SEND_INFO);
	SendInfo(sockfd, second_player, A);

	CheckVic(cross_vic, zero_vic, draw, A);

	if ((*cross_vic == false) && (*zero_vic == false) && (*draw == false))
	{
		SendSig(sockfd, second_player, SEND_MES);
		SendMessage(sockfd, second_player, "Your turn:");

		SendSig(sockfd, second_player, RCV_INFO);
		RcvInfo(sockfd, second_player, A, '0');

		SendSig(sockfd, first_player, SEND_INFO);
		SendInfo(sockfd, first_player, A);

		SendSig(sockfd, second_player, SEND_INFO);
		SendInfo(sockfd, second_player, A);

		CheckVic(cross_vic, zero_vic, draw, A);

		if (!((*cross_vic == false) && (*zero_vic == false) && (*draw == false)))
		{
			if (*draw == true)
			{
				SendSig(sockfd, first_player, SEND_MES);
				SendMessage(sockfd, first_player, "That's a draw!");

				SendSig(sockfd, second_player, SEND_MES);
				SendMessage(sockfd, second_player, "That's a draw!");

				SendSig(sockfd, first_player, FINISH);
				SendSig(sockfd, second_player, FINISH);

				exit(0);
			}

			if (*zero_vic == true)
			{
				SendSig(sockfd, first_player, SEND_MES);
				SendMessage(sockfd, first_player, "You lost!");

				SendSig(sockfd, second_player, SEND_MES);
				SendMessage(sockfd, second_player, "You won!");

				SendSig(sockfd, first_player, FINISH);
				SendSig(sockfd, second_player, FINISH);

				exit(0);
			}
		}
	}
	else
	{
		if (*draw == true)
		{
			SendSig(sockfd, first_player, SEND_MES);
			SendMessage(sockfd, first_player, "That's a draw!");

			SendSig(sockfd, second_player, SEND_MES);
			SendMessage(sockfd, second_player, "That's a draw!");

			SendSig(sockfd, first_player, FINISH);
			SendSig(sockfd, second_player, FINISH);

			exit(0);
		}

		if (*cross_vic == true)
		{
			SendSig(sockfd, first_player, SEND_MES);
			SendMessage(sockfd, first_player, "You won!");

			SendSig(sockfd, second_player, SEND_MES);
			SendMessage(sockfd, second_player, "You lost!");

			SendSig(sockfd, first_player, FINISH);
			SendSig(sockfd, second_player, FINISH);

			exit(0);
		}
	}
}

#endif // SERVER_H