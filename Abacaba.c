#include <stdio.h>
#include <string.h>
#include <malloc.h>

void GenerateString(int n, char* string);

int main() {
	int n = 0;
	printf("Program name: abacaba. Made by Gainullin Ivan, 2021\n");
	printf("Enter number n: ");
	scanf("%d", &n);
	while (n < 1) {
		printf("Enter integer number n > 0, try one more time: ");
		scanf("%d", &n);
	}
	
	// TODO: чтобы посчитать 2^n можно воспользовать либо готовой ф-ей pow, либо побитовым сдвигом 1 << n. Если вы хотите написать код сами, а не пользоваться чем-то существющим,
	// то вынесите это в отдельную ф-ю и соответствующим образом назовите. Пока вы дважды в программе продублировали код возведения двойки в степень.
	int number_of_elements = 1;
	int i;
	for (i = 1; i < n; i++) {
		number_of_elements = 2 * number_of_elements + 1;
	}
	
	char* string;
	string = (char*) malloc(number_of_elements * sizeof(char));
	
	GenerateString(n, string);
	
	printf("This is your line:\n");
	for (i = 0; i < number_of_elements; i++) {
		printf("%c", string[i]);
	}
	printf("\n");
	
	free(string);
	
	return 0;
}

void GenerateString(int n, char* string) {
	char* support_string; char to_place = 'a' - 1;
	
	if (n > 1)
		GenerateString(n-1, string);
	
	int number_of_elements = 1;
	int i;
	for (i = 1; i < n; i++) {
		number_of_elements = 2 * number_of_elements + 1;
	}
	
	support_string = (char*) malloc(number_of_elements * sizeof(char));
	strcat(support_string, string);
	support_string[strlen(support_string)] = to_place + n;
	strcat(support_string, string);
	strncpy(string, support_string, strlen(support_string));
	
	free(support_string);
}
