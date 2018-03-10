#pragma warning(disable: 4996)
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

#define SIZE_PLAIN 21
#define SIZE_CIPHER 32 
#define SIZE_KEY 16
#define SIZE_IV 16
#define MODE_ENCRYPTION 1

void main() {
	FILE * file_plain, *file_cipher, *file_word;
	EVP_CIPHER_CTX *evp_ctx;
	char string_input[SIZE_PLAIN + 1], string_output[SIZE_CIPHER + 1], string_check[SIZE_CIPHER + 1];
	char string_iv[SIZE_IV+1];
	char string_key[SIZE_KEY + 1];
	int int_plainLength, int_cipherLength, int_finalLength, int_wordLength;

	/* init memory */
	memset(string_input, 0, SIZE_PLAIN + 1);
	memset(string_output, 0, SIZE_CIPHER + 1);
	memset(string_iv, 0, SIZE_IV + 1);

	/* open file */
	file_plain = fopen("plain.txt", "rb");
	file_cipher = fopen("cipher.txt", "rb");
	file_word = fopen("word.txt", "rb");

	/* check whether its length is 21 */
	fseek(file_plain, 0, SEEK_END);
	int_plainLength = ftell(file_plain);
	if (int_plainLength != SIZE_PLAIN)
		return;

	/* read text from file */
	fseek(file_plain, 0, SEEK_SET);
	fgets(string_input, SIZE_PLAIN + 1, file_plain);
	fgets(string_check, SIZE_CIPHER + 1, file_cipher);
	printf("Input > %s\n\n", string_input);

	while (1)
	{
		/* fetch word from word.txt */
		memset(string_key, 0, SIZE_KEY + 1);
		fgets(string_key, sizeof(string_key), file_word);
		int_wordLength = strlen(string_key);
		if (int_wordLength <= 0) break;	// if end of file then break
		for(int i=int_wordLength-1;i<SIZE_KEY;i++)
		string_key[i] = 0x20;
		printf("Checking key... > %s\n", string_key);

		/* make empty key & check size of iv and key */
		evp_ctx = EVP_CIPHER_CTX_new();
		EVP_CipherInit_ex(evp_ctx, EVP_aes_128_cbc(), NULL, NULL, NULL, MODE_ENCRYPTION);
		OPENSSL_assert(EVP_CIPHER_CTX_key_length(evp_ctx) == SIZE_KEY);
		OPENSSL_assert(EVP_CIPHER_CTX_iv_length(evp_ctx) == SIZE_IV);

		/* insert iv and key into empty key */
		EVP_CipherInit_ex(evp_ctx, NULL, NULL, (const unsigned char*)string_key, (unsigned char *)string_iv, MODE_ENCRYPTION);

		/* process of encryption */
		if (!EVP_CipherUpdate(evp_ctx, (unsigned char*)string_output, &int_cipherLength, (const unsigned char*)string_input, int_plainLength)) {
			EVP_CIPHER_CTX_free(evp_ctx);
			return;
		}
		int_finalLength = int_cipherLength;
		if (!EVP_CipherFinal_ex(evp_ctx, (unsigned char*)string_output + int_cipherLength, &int_cipherLength)) {
			/* Error */
			EVP_CIPHER_CTX_free(evp_ctx);
			return;
		}
		int_finalLength += int_cipherLength;

		if (!strcmp(string_output, string_check)) {
			EVP_CIPHER_CTX_free(evp_ctx);
			printf("\nI find Key! Key is %s\n", string_key);
			break;
		}

		EVP_CIPHER_CTX_free(evp_ctx);
	}

	fclose(file_plain);
	fclose(file_cipher);
	fclose(file_word);
}