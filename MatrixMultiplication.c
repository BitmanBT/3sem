/*
	If we launch program using AutoGenerate with 1 thread (n = 30) we get:
	real	0m3,968s
	If we launch program using AutoGenerate with 30 thread (n = 1) we get:
	real	0m2,379s
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <malloc.h>

struct information
{
	int* a;
	int* b;
	int* c;
	
	size_t n;
	size_t m;
	
	int i;
};

int pow(int x, int y)
{
	int i = 0;
	int res = 1;
	for (i = 0; i < y; i++)
	{
		res *= x;
	}
	
	return res;
}

void AutoGenerate(int* a, size_t n, size_t m)
{
	int i = 0;
	
	for (i = 0; i < pow((n * m), 2); i++)
	{
		*(a + i) = 1;
	}
}

void GetMatrix(int* ptr, size_t n, size_t m)
{
	int i = 0, j = 0;	
	
	for (i = 0; i < (n * m); i++)
	{
		for (j = 0; j < (n * m); j++)
		{
			scanf("%d", (ptr + (n * m) * i + j));
		}
	}
}

void PrintMatrix(int* ptr, size_t n, size_t m)
{
	int i = 0, j = 0;
	
	for (i = 0; i < (n * m); i++)
	{
		for (j = 0; j < (n * m); j++)
		{
			printf("%d ", *(ptr + (n * m) * i + j));
		}
		printf("\n");
	}
}

void* CountAndWrite(struct information* count)
{
	
	int j = 0, k = 0, z = 0;
	for (j = 0; j < (count -> n); j++)
	{	
		for (z = 0; z < ((count -> n) * (count -> m)); z++)
		{
			*((count -> c) + (count -> i) * (count -> n) * ((count -> n) * (count -> m)) + j * ((count -> n) * (count -> m)) + z) = 0;
			
			int term[(count -> n) * (count -> m)];
			
			for (k = 0; k < ((count -> n) * (count -> m)); k++)
			{	
				term[k] = 0;
				term[k] += *((count -> a) + (count -> i) * (count -> n) * ((count -> n) * (count -> m)) + j * ((count -> n) * (count -> m)) + k) * *((count -> b) + z + k * ((count -> n) * (count -> m)));
				*((count -> c) + (count -> i) * (count -> n) * ((count -> n) * (count -> m)) + j * ((count -> n) * (count -> m)) + z) += term[k];
			}
		}
	}
}

int main(int argc, char* argv[])
{
	printf("\t");
	printf("\tThis is a program called MatrixMultiplication.\n \
		It uses several threads to multiply two matrix.\n \
		Made by Gainullin Ivan, 2021\n");
		
	size_t m = 0;
	printf("Enter number of threads you want: ");
	scanf("%d", &m);
	
	size_t n = 0;
	printf("Enter number of columns(lines) for each thread to multiply: ");
	scanf("%d", &n);
	
	int* a = (int*) malloc(pow(n * m, 2) * sizeof(int));
	int* b = (int*) malloc(pow(n * m, 2) * sizeof(int));
	
	if (atoi(argv[1]) == 0)
	{
		printf("Enter the first matrix:\n");
		GetMatrix(a, n, m);
		
		printf("Enter the second matrix:\n");
		GetMatrix(b, n, m);
		
		printf("This is your first matrix:\n");
		PrintMatrix(a, n, m);
		
		printf("This is your second matrix:\n");
		PrintMatrix(b, n, m);
	} else if (atoi(argv[1]) == 1) {
		AutoGenerate(a, n, m);
		AutoGenerate(b, n, m);
	}
	
	int* c = (int*) malloc(pow(n * m, 2) * sizeof(int));
	
	int i = 0;
	for (i = 0; i < m; i++)
	{
		pthread_t thid;
		struct information* count;
		count -> a = a;
		count -> b = b;
		count -> c = c;
		count -> n = n;
		count -> m = m;
		count -> i = i;
			
		int result = pthread_create(&thid, (pthread_attr_t *)NULL, CountAndWrite, count);	
		if(result != 0) 
		{
			printf ("Error on thread create, return value = %d\n", result);
			exit(-1);
		}
		
		pthread_join(thid, (void **)NULL);
	}
	
	printf("That's the answer:\n");
	PrintMatrix(c, n, m);
	
	free(a);
	free(b);
	free(c);
	
	return 0;
}
