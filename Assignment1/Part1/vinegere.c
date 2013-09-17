#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vinegere.h"

char encrypt(char, char);
char decrypt(char, char);

char encrypt(char key, char plain)
{
	char pl = plain & 0xf;
	char ph = plain >> 4;
	char kl = key & 0xf;
	char kh = key >> 4;

	return map[ph][kl] << 4 | map[pl][kh];
}

char decrypt(char key, char cypher)
{
	char cl = cypher & 0xf;
	char ch = (cypher >> 4) & 0xf;
	char kl = key & 0xf;
	char kh = (key >> 4) & 0xf;
	char pl = '\0', ph = '\0';
	int i;

	for (i = 0; i < 16; i++)
	{
		if (map[i][kl] == ch)
		{
			ph = i;
		}

		if (map[i][kh] == cl)
		{
			pl = i;
		}
	}

	if (ph == '\0')
	{

#ifdef PRINTDEBUG
		printf("ph not found, kl: 0x%02x, ch: 0x%02x\n", kl, ch);
		int j;
		for (j = 0; j < 16; j++)
		{
			printf("%d ", map[j][kl]);
		}
		printf("\n");
#endif
#ifdef DEBUG
		return '_';
#endif
	}

	if (pl == '\0')
	{

#ifdef PRINTDEBUG
		printf("pl not found\n");
#endif
#ifdef DEBUG
		return '_';
#endif
	}

	return ph << 4 | (pl & 0xf);
}

void printcypher(const char * text)
{
	int i, len = strlen(text);
	printf("\ncyptertext:\n");
	for (i = 0; i < len; i++)
	{
		printf("0x%02x ", text[i] & 0xff);
		if (text[i] == 0x20)
		{
			printf("\n");
		}
	}

	printf("\n\n");
}

void printplain(const char * text)
{
	printf("decrpyted: %s\n", text);
}

void encryptstr(char * cyphertext, const char * keyword, const char * plaintext)
{
	int i, j = 0;
	int keylen = strlen(keyword);

	memset(cyphertext, 0, sizeof(char));

	for (i = 0; i < strlen(plaintext); i++)
	{
		if (plaintext[i] != ' ')
		{
			cyphertext[i] = encrypt(keyword[j], plaintext[i]);
			j = (j + 1) % keylen;
		}
		else
		{
			cyphertext[i] = ' ';
		}
	}
}

void decryptstr(char * plaintext, const char * keyword, const char * cyphertext)
{
	int i, j = 0;
	int keylen = strlen(keyword);
	int textlen = strlen(cyphertext);
	memset(plaintext, 0, sizeof(char));

	if (sizeof(plaintext) != sizeof(cyphertext))
	{
		printf("plaintext: %ld, cyphertext: %ld\n", sizeof(plaintext), sizeof(cyphertext));
		return;
	}

	for (i = 0; i < textlen; i++)
	{
		if (cyphertext[i] != ' ')
		{
			plaintext[i] = decrypt(keyword[j], cyphertext[i]);
			j = (j + 1) % keylen;
		}
		else
		{
			plaintext[i] = ' ';
		}
	}

	plaintext[textlen] = '\0';
}
