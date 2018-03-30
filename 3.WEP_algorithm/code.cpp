#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

#define SIZE_IV 24
#define SIZE_KEY 40
#define SIZE_BYTE_IV SIZE_IV/8
#define SIZE_BYTE_KEY SIZE_KEY/8

#define SIZE_BUFFER 100

#define SIZE_BYTE 256

typedef bool Bit;
typedef unsigned char Byte;

void makeIV(Bit *iv);
void inputKey(Bit *key);
void makeSeed(Byte *seed, Bit *iv, Bit *key);
void inputPlainText(char *plainText);
void initRC4(Byte *S, Byte *K, Byte *seed);
void shuffle(Byte *S, Byte *K);
void makeKeystream(Byte *S, Byte* keyStream, int length);
void encryption(char *plainText, char * cyperText, Byte *keyStream);
void decryption(char *plainText, char * cyperText, Byte *keyStream);

int main() {

	Bit iv[SIZE_IV];
	Bit key[SIZE_KEY];
	Byte seed[SIZE_BYTE_IV + SIZE_BYTE_KEY + 1];

	char plainText[SIZE_BUFFER];
	char cipherText[SIZE_BUFFER];

	Byte S[SIZE_BYTE];
	Byte K[SIZE_BYTE];
	Byte keyStream[SIZE_BUFFER];

	makeIV(iv);
	inputKey(key);
	makeSeed(seed, iv, key);
	inputPlainText(plainText);

	initRC4(S, K, seed);
	shuffle(S, K);
	makeKeystream(S, keyStream, strlen(plainText));

	encryption(plainText, cipherText, keyStream);
	decryption(plainText, cipherText, keyStream);

	return 0;
}

void makeIV(Bit *iv) {
	srand(time(NULL));
	printf("Initial Vector: \"");
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
	printf("\" is created\n");
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

	printf("Key: \"");
	for (int i = 0; i < SIZE_KEY; i++)
		key[i] ? printf("1") : printf("0");
	printf("\" is created\n");
}

void makeSeed(Byte *seed, Bit *iv, Bit *key) {

	for (int i = 0, sum; i < SIZE_BYTE_IV; i++) {
		sum = 0;
		for (int j = 7, k = 1; j >= 0; j--, k *= 2) {
			sum += k * (iv[8 * i + j] ? 1 : 0);
		}
		seed[i] = sum;
	}
	for (int i = 3, sum; i < SIZE_BYTE_IV + SIZE_BYTE_KEY; i++) {
		sum = 0;
		for (int j = 7, k = 1; j >= 0; j--, k *= 2) {
			sum += k * (key[8 * i + j] ? 1 : 0);
		}
		seed[i] = sum;
	}

	printf("Seed: \"%s\" is created\n", seed);
}

void inputPlainText(char *plainText) {
	printf("Please enter the plain text: ");
	scanf_s("%s", plainText, SIZE_BUFFER);
}

void initRC4(Byte *S, Byte *K, Byte *seed) {
	int sum;
	for (int i = 0; i < SIZE_BYTE; i++) {
		S[i] = i;
		K[i] = seed[i % (SIZE_BYTE_IV + SIZE_BYTE_KEY)];
	}
}

void shuffle(Byte *S, Byte *K) {
	for (int i = 0, j = 0; i < SIZE_BYTE; i++) {
		j = (j + S[i] + K[i]) % SIZE_BYTE;
		swap(S[i], S[j]);
	}
}

void makeKeystream(Byte *S, Byte* keyStream, int length) {

	memset(keyStream, 0, SIZE_BUFFER);
	for (int i = 0, j = 0, k = 0; i < length; i++) {
		j = (j + 1) % SIZE_BYTE;
		k = (k + S[j]) % SIZE_BYTE;
		swap(S[j], S[k]);
		keyStream[i] = S[S[(S[j] + S[k]) % SIZE_BUFFER]];
	}
}

void encryption(char *plainText, char *cipherText, Byte *keyStream) {

	memset(cipherText, 0, SIZE_BUFFER);
	printf("Result(Encryption) is \"");
	for (int i = 0; i < strlen(plainText); i++) {
		cipherText[i] = plainText[i] ^ keyStream[i];
		printf("%c", cipherText[i]);
	}
	printf("\"\n");
}

void decryption(char *plainText, char *cipherText, Byte *keyStream) {

	memset(plainText, 0, SIZE_BUFFER);
	printf("Result(Decryption) is \"");
	for (int i = 0; i < strlen(cipherText); i++) {
		plainText[i] = cipherText[i] ^ keyStream[i];
		printf("%c", plainText[i]);
	}
	printf("\"\n");
}