#include <stdio.h>
#include <string.h>
#include <malloc.h>

void GenerateString(int n, char* string);
int CountNumber(int* n);

int main() {
	int n = 0;
	printf("Program name: abacaba. Made by Gainullin Ivan, 2021\n");
	printf("Enter number n: ");
	scanf("%d", &n);
	while (n < 1) {
		printf("Enter integer number n > 0, try one more time: ");
		scanf("%d", &n);
	}
	
	int number_of_elements = CountNumber(&n);
	
	char* string;
	string = (char*) malloc(number_of_elements * sizeof(char));
	
	GenerateString(n, string);
	
	printf("This is your line:\n");
	int i = 0;
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
	
	int number_of_elements = CountNumber(&n);
	
	support_string = (char*) malloc(number_of_elements * sizeof(char));
	strcat(support_string, string);
	support_string[strlen(support_string)] = to_place + n;
	strcat(support_string, string);
	strncpy(string, support_string, strlen(support_string));
	
	free(support_string);
}

int CountNumber(int* n) {
	int i = 1;
	int number_of_elements = 1;
	
	for (i = 1; i < *n; i++) {
		number_of_elements = 2 * number_of_elements + 1;
	}
	
	return number_of_elements;
}
