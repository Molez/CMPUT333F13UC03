#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vinegere.h"
#include "debug.h"

char printASCII(const char * description, const char * string)
{
	int i;
	printf("\n%s ASCII:\n", description);
	for (i = 0; i < strlen(string); i++)
	{
		printf("0x%02x ", string[i]);
		if (string[i] == ' ')
			printf("\n");
	}
	printf("\n");
}

void test_encryptdecrypt()
{
	char * keyword = "EDMONTON";
	char * plaintext = "UNIVERSITY OF ALBERTA";
	int len = sizeof(plaintext);
	printf("keyword: %s\nplaintext: %s\n", keyword, plaintext);

	char cyphertext[len];
	printASCII("Keyword", keyword);

	encryptstr(cyphertext, keyword, plaintext);
	printASCII("Plaintext", plaintext);
	printcypher(cyphertext);

	char decrpyted[len];
	decryptstr(decrpyted, keyword, cyphertext);
	printplain(decrpyted);
	printASCII("Decrypted", decrpyted);
}

void debug()
{
	FILE * file;
	char * text;
	size_t size = 0;
	char buffer[256];

	file = fopen("ciphertext1", "r");
	if (file == NULL) {
		err(1, "Unable to open file");
		exit(1);
		return;
	}
	memset(buffer, 0, 256 * sizeof(char));
	
	//while (!feof(file)) {
		//text = realloc(text, size + (256 * sizeof(char)));
		//if (text == NULL)
		//	err(1, "realloc failed.");
		fgets(buffer, 256, file);
	//}

	fclose(file);

	printf("%s\n", buffer);
	printf("\n");

	while (1)
	{
		char key[80], decrypted[256];
		memset(key, 0, sizeof(char));
		printf("enter a key: ");
		fgets(key, sizeof(key), stdin);
		decryptstr(decrypted, key, buffer);
		printplain(decrypted);
	}


	free(text);

}
