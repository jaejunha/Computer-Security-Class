#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

#define SIZE_IV 24
#define SIZE_KEY 40
#define SIZE_BYTE_KEY SIZE_KEY/8

#define SIZE_BUFFER 100

#define SIZE_BYTE 256

typedef bool Bit;
typedef unsigned char Byte;

void makeIV(Bit *iv);
void inputKey(Bit *key);
void inputPlainText(char *plainText);
void initRC4(Byte *S, Byte *K, Bit *key);
void shuffle(Byte *S, Byte *K);
void encryption(char *plainText, Byte *S);

int main() {

	Bit iv[SIZE_IV];
	Bit key[SIZE_KEY];

	char plainText[SIZE_BUFFER];

	Byte S[SIZE_BYTE];
	Byte K[SIZE_BYTE];

	makeIV(iv);
	inputKey(key);

	inputPlainText(plainText);

	initRC4(S, K, key);
	shuffle(S, K);

	encryption(plainText, S);

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
			key[4 * i + 3 - j] = k % 2;
			k = k >> 1;
		}
	}

	printf("Key: ");
	for (int i = 0; i < SIZE_KEY; i++)
		key[i] ? printf("1") : printf("0");
	printf(" is created\n");
}

void inputPlainText(char *plainText) {
	printf("Please enter the plain text: ");
	scanf_s("%s", plainText, SIZE_BUFFER);
}

void initRC4(Byte *S, Byte *K, Bit *key) {
	int sum;
	for (int i = 0; i < SIZE_BYTE; i++) {
		sum = 0;
		S[i] = i;
		for (int j = 7, k = 1; j >= 0; j--, k *= 2)
			sum += k * (key[8 * (i % SIZE_BYTE_KEY) + j] ? 1 : 0);
		K[i] = sum;
	}
}

void shuffle(Byte *S, Byte *K) {
	for (int i = 0, j = 0; i < SIZE_BYTE; i++) {
		j = (j + S[i] + K[i]) % SIZE_BYTE;
		swap(S[i], S[j]);
	}
}

void encryption(char *plainText, Byte *S) {
	Byte keyStream;

	printf("Result is ");

	for (int i = 0, j = 0, k = 0; i < strlen(plainText); i++) {
		j = (j + 1) % SIZE_BYTE;
		k = (k + S[j]) % SIZE_BYTE;
		swap(S[j], S[k]);

		keyStream = S[(S[j] + S[k]) % SIZE_BUFFER];
		printf("%c", plainText[i] ^ S[keyStream]);
	}
}