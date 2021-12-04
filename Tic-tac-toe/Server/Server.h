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

struct chosen_side
{
	int first;
	int second;
};

void BindServer(int* sockfd, struct sockaddr_in* servaddr)
{
	bzero(servaddr, sizeof(*servaddr));
	servaddr->sin_family = AF_INET;
	servaddr->sin_port = htons(51000);
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

void RcvInfo(int* sockfd, int* clilen, struct sockaddr_in* cliaddr, char* line)
{
	*clilen = sizeof(*cliaddr);

	int n;

	if ((n = recvfrom(*sockfd, line, 999, 0, (struct sockaddr*) cliaddr, clilen)) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void SendSig(int* sockfd, int* clilen, struct sockaddr_in* cliaddr, char* line)
{
	strcpy(line, "Got it!\n");

	if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*) cliaddr, *clilen) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void SendInfo(int* sockfd, int* clilen, struct sockaddr_in* cliaddr, char* line, char* A)
{
	strcpy(line, A);

	if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*)cliaddr, *clilen) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void GetStartInfo(int* sockfd, int* clilen, struct sockaddr_in* cliaddr, char* line, struct chosen_side* side)
{
	RcvInfo(sockfd, clilen, cliaddr, line);

	side->first = atoi(line);

	SendSig(sockfd, clilen, cliaddr, line);

	RcvInfo(sockfd, clilen, cliaddr, line);

	side->second = atoi(line);

	SendSig(sockfd, clilen, cliaddr, line);
}

void CheckVic(bool* cross_vic, bool* zero_vic, char* A)
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
}

void SendResCheckFirst(int* sockfd, int* clilen, struct sockaddr_in* cliaddr, char* line, struct chosen_side* side, char* A, bool* cross_vic, bool* zero_vic)
{
	if ((side->first == *cross_vic) && (*zero_vic == true))
	{
		strcpy(line, "1");

		if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*)cliaddr, *clilen) < 0)
		{
			perror(NULL);
			close(*sockfd);
			exit(1);
		}
	}

	if ((side->first == *zero_vic) && (*cross_vic == true))
	{
		strcpy(line, "1");

		if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*)cliaddr, *clilen) < 0)
		{
			perror(NULL);
			close(*sockfd);
			exit(1);
		}
	}

	if ((side->first == *cross_vic) && (*cross_vic == true))
	{
		strcpy(line, "2");

		if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*)cliaddr, *clilen) < 0)
		{
			perror(NULL);
			close(*sockfd);
			exit(1);
		}
	}

	if ((side->first == *zero_vic) && (*zero_vic == true))
	{
		strcpy(line, "2");

		if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*)cliaddr, *clilen) < 0)
		{
			perror(NULL);
			close(*sockfd);
			exit(1);
		}
	}

	strcpy(line, "0");

	if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*)cliaddr, *clilen) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void SendResCheckSecond(int* sockfd, int* clilen, struct sockaddr_in* cliaddr, char* line, struct chosen_side* side, char* A, bool* cross_vic, bool* zero_vic)
{
	if ((side->second == *cross_vic) && (*zero_vic == true))
	{
		strcpy(line, "1");

		if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*)cliaddr, *clilen) < 0)
		{
			perror(NULL);
			close(*sockfd);
			exit(1);
		}
	}

	if ((side->second == *zero_vic) && (*cross_vic == true))
	{
		strcpy(line, "1");

		if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*)cliaddr, *clilen) < 0)
		{
			perror(NULL);
			close(*sockfd);
			exit(1);
		}
	}

	if ((side->second == *cross_vic) && (*cross_vic == true))
	{
		strcpy(line, "2");

		if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*)cliaddr, *clilen) < 0)
		{
			perror(NULL);
			close(*sockfd);
			exit(1);
		}
	}

	if ((side->second == *zero_vic) && (*zero_vic == true))
	{
		strcpy(line, "2");

		if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*)cliaddr, *clilen) < 0)
		{
			perror(NULL);
			close(*sockfd);
			exit(1);
		}
	}

	strcpy(line, "0");

	if (sendto(*sockfd, line, strlen(line), 0, (struct sockaddr*)cliaddr, *clilen) < 0)
	{
		perror(NULL);
		close(*sockfd);
		exit(1);
	}
}

void StrokeFirst(int* sockfd, int* clilen, struct sockaddr_in* cliaddr, char* line, struct chosen_side* side, char* A, bool* cross_vic, bool* zero_vic)
{
	RcvInfo(sockfd, clilen, cliaddr, line); //Проверка связи

	SendInfo(sockfd, clilen, cliaddr, line, A); //Отправка исходной матрицы

	RcvInfo(sockfd, clilen, cliaddr, line); //Получение запроса на проверку победы-поражения

	CheckVic(cross_vic, zero_vic, A); //Проверка

	SendResCheckFirst(sockfd, clilen, cliaddr, line, side, A, cross_vic, zero_vic); //Отправка результатов
	
	RcvInfo(sockfd, clilen, cliaddr, line); //Получение изменений матрицы

	int num = atoi(line); int ln = 0, col = 0;
	while (num >= 10)
	{
		ln++; num -= 10;
	}
	col = num;

	if (side->first == 0)
	{
		A[ln * 3 + col] = '0';
	}
	else if (side->first == 1)
	{
		A[ln * 3 + col] = '1';
	}

	SendInfo(sockfd, clilen, cliaddr, line, A); //Отправка изменений

	RcvInfo(sockfd, clilen, cliaddr, line); //Получение запроса на проверку победы-поражения

	CheckVic(cross_vic, zero_vic, A); //Проверка

	SendResCheckFirst(sockfd, clilen, cliaddr, line, side, A, cross_vic, zero_vic); //Отправка результатов
}

void StrokeSecond(int* sockfd, int* clilen, struct sockaddr_in* cliaddr, char* line, struct chosen_side* side, char* A, bool* cross_vic, bool* zero_vic)
{
	RcvInfo(sockfd, clilen, cliaddr, line); //Проверка связи

	SendInfo(sockfd, clilen, cliaddr, line, A); //Отправка исходной матрицы

	RcvInfo(sockfd, clilen, cliaddr, line); //Получение запроса на проверку победы-поражения

	CheckVic(cross_vic, zero_vic, A); //Проверка

	SendResCheckSecond(sockfd, clilen, cliaddr, line, side, A, cross_vic, zero_vic); //Отправка результатов

	RcvInfo(sockfd, clilen, cliaddr, line); //Получение изменений матрицы

	int num = atoi(line); int ln = 0, col = 0;
	while (num >= 10)
	{
		ln++; num -= 10;
	}
	col = num;

	if (side->second == 0)
	{
		A[ln * 3 + col] = '0';
	}
	else if (side->second == 1)
	{
		A[ln * 3 + col] = '1';
	}

	SendInfo(sockfd, clilen, cliaddr, line, A); //Отправка изменений

	RcvInfo(sockfd, clilen, cliaddr, line); //Получение запроса на проверку победы-поражения

	CheckVic(cross_vic, zero_vic, A); //Проверка

	SendResCheckSecond(sockfd, clilen, cliaddr, line, side, A, cross_vic, zero_vic); //Отправка результатов
}

#endif // SERVER_H