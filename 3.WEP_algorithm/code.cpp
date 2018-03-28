#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE_IV 24
#define SIZE_KEY 40

typedef bool Bit;

void makeIV(Bit *iv);
void inputKey(Bit *key);

int main() {

	Bit iv[SIZE_IV];
	Bit key[SIZE_KEY];

	makeIV(iv);
	inputKey(key);
	return 0;
}

void makeIV(Bit *iv) {
	srand(time(NULL));
	printf("Initial Vector: ");
	for (int i = 0; i < SIZE_IV; i++) {
		if (rand() % 2 == 0) {
			iv[i] = false;
			printf("0");
		}
		else {
			iv[i] = true;
			printf("1");
		}
	}
	printf(" is created\n");
}

void inputKey(Bit *key) {
	char input[SIZE_KEY / 4 + 1];
	bool typoChecker = false;
	char c;
	do {
		printf("Please enter the key (0~F, 10words): ");
		scanf_s("%s", input, sizeof(input));
		for (int i = 0; i < sizeof(input) - 1; i++) {
			c = tolower(input[i]);
			if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
				printf("Please check your input!\n");
				typoChecker = true;
				break;
			}
			typoChecker = false;
		}
	} while (typoChecker);

	for (int i = 0, k; i < sizeof(input) - 1; i++) {
		if (input[i] <= '9')
			k = input[i] - '0';
		else
			k = tolower(input[i]) - 'a' + 10;

		for (int j = 0; j < 4; j++) {
			key[4 * i + 3 - j] = !(k % 2);
			k = k >> 1;
		}
	}

	printf("Initial Vector: ");
	for (int i = 0; i < SIZE_KEY; i++) {
		if (key[i] == 0)
			printf("1");
		else
			printf("0");
	}
	printf(" is created\n");
}