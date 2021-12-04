#include "Server.h"

int main()
{	
	int sockfd; int clilen, n; char line[1000];
	struct sockaddr_in servaddr, cliaddr;

	BindServer(&sockfd, &servaddr);

	struct chosen_side side;

	GetStartInfo(&sockfd, &clilen, &cliaddr, line, &side);

	bool cross_vic = false, zero_vic = false;

	char A[9]; int i;
	for (i = 0; i < 9; i++)
		A[i] = '-';

	while ((cross_vic == false) && (zero_vic == false))
	{
		StrokeFirst(&sockfd, &clilen, &cliaddr, line, &side, A, &cross_vic, &zero_vic);
		StrokeSecond(&sockfd, &clilen, &cliaddr, line, &side, A, &cross_vic, &zero_vic);
	}

	if (((side.second = 1) && (cross_vic = true)) || ((side.second = 0) && (zero_vic = true)))
	{
		RcvInfo(&sockfd, &clilen, &cliaddr, line); //Проверка связи

		SendInfo(&sockfd, &clilen, &cliaddr, line, A); //Отправка исходной матрицы

		RcvInfo(&sockfd, &clilen, &cliaddr, line); //Получение запроса на проверку победы-поражения

		CheckVic(&cross_vic, &zero_vic, A); //Проверка

		SendResCheckFirst(&sockfd, &clilen, &cliaddr, line, &side, A, &cross_vic, &zero_vic); //Отправка результатов
	}

	return 0;
}