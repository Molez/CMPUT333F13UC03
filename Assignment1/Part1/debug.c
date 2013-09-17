#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vinegere.h"
#include "debug.h"

#define KEYCHARS 62
#define MAXKEYSIZE 20
#define BUFFLEN 256

typedef struct {
	char key;
	int hitcount;
	int hits[BUFFLEN];
} keyhitpair;

typedef struct {
	char key;
	char plain;
} keyplainpair;

typedef struct {
	char c;
	int hits;
	keyplainpair pairs[KEYCHARS];
} cypherchar;

const char * OUTPUTPATH = "output.debug";
int hits[KEYCHARS];
keyhitpair keyhits[KEYCHARS];

/**Prints the ASCII values of a string**/
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

/**A comparison function for sorting key hit pairs by the highest hit count.**/
int most_hits(const void * a, const void * b)
{
	return ((keyhitpair*)b)->hitcount - ((keyhitpair*)a)->hitcount;
}

/**A comparison function for sorting key hit pairs in the order of their first hit.**/
int first_hit(const void * a, const void * b)
{
	keyhitpair *  paira = ((keyhitpair*)a);
	keyhitpair *  pairb = ((keyhitpair*)b);
	int afirst = paira->hits[0];
	int bfirst = pairb->hits[0];

	if (afirst == bfirst)
	{
		return paira->hits[1] - pairb->hits[1];
	}

	return afirst - bfirst;
}

/**A general test to make sure the encrypting/decrypting schemes are correct.**/
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

/**Look for patterns in the key-hit histogram.**/
int findpattern(keyhitpair keyhits)
{
	// Implementation needed.
}

/**Tries all possible keys against the cypher char and collects a list of hits.**/
void test(cypherchar * cypher, int index)
{
	int i, j = 0;
	size_t len = sizeof(keych);
	char keyValid[len], plainValid[len];

	for (i = 0; i < len; i++) {
		char d = decrypt(keych[i], cypher->c);
#if TRUE
		/* Checking for any valid plaintext */
		if (d >= 0x20 && d <= 0x7e) {
#else
		/* Checking for only alphanumeric plaintext */
		if ((d == 0x20) || (d >= 0x30 && d <= 0x39) 
			|| (d >= 0x41 && d <= 0x5a) 
			|| (d >= 97 && d <= 0x7a)) {
#endif
			cypher->pairs[j].key = keych[i];
			cypher->pairs[j].plain = d;
			keyhits[i].hits[keyhits[i].hitcount] = index;
			keyhits[i].hitcount++;
			j++;
			cypher->hits++;
		}
	}
}

/**Prints the cypher character's key hits and their decrypted plaintext char.**/
void printpairs(FILE * file, cypherchar * cypher)
{
	int i, count = cypher->hits;
#if FALSE
	fprintf(file, "plain:\t");

	for (i = 0; i < count; i++) {
		fprintf(file, "%c ", cypher->pairs[i].plain);
	}

	fprintf(file, "\n");
#endif
	fprintf(file, "key:\t");
	for (i = 0; i < count; i++) {
		fprintf(file, "%c ", cypher->pairs[i].key);
	}

	fprintf(file, "\n");
}

/** Prints the possible keys and their hit counts **/
void printkeyhits(const keyhitpair * keyhits, int count)
{
	FILE * file;

	fopen(OUTPUTPATH, "W+");
	if (file == NULL)
	{
		fprintf(stderr, "unable to open file");
		exit(-1);
	}

	int i;
	fprintf(file, "\nkey\thits\n");
	for (i = 0; i < count; i++)
	{
		// print the keys and their values, max first.
		if (keyhits[i].hitcount > 0){
			fprintf(file, "%c\t%d\n", keyhits[i].key, keyhits[i].hitcount);
		}
	}

	fclose(file);
}

/* Prints a historam of the key hits*/
void printkeyhitstats(const keyhitpair * keyhits, int count)
{
	FILE * file;
	int i, j, a;

	file = fopen("keyhitstats.debug", "w");
	if (file == NULL)
	{
		fprintf(stderr, "unable to open file");
		exit(-1);
	}

	for (i = 0; i < count; i++)
	{
		if (keyhits[i].hitcount > 0)
			fprintf(file, "\n%c:", keyhits[i].key);

		j = 0;
		for (a = 0; a < BUFFLEN; a++)
		{
			if (j < keyhits[i].hitcount)
			{
				if (a == keyhits[i].hits[j])
				{
					fprintf(file, " %2d", keyhits[i].hits[j]);
					j++;	
				}
				else
				{
					fprintf(file, "   "); 
				}				
			}
			else
			{
				break;
			}
		}
	}

	fclose(file);
}

/**Called by the main function to run debug mode.**/
void debug()
{
	FILE * file, * output;
	char * text;
	size_t size = 0;
	char buffer[BUFFLEN];

	file = fopen("ciphertext1", "r");
	if (file == NULL) {
		err(1, "Unable to open file");
		exit(1);
		return;
	}
	memset(buffer, 0, sizeof(buffer));
	
#ifdef DYNAMIC
	while (!feof(file)) {
		text = realloc(text, size + (256 * sizeof(char)));
		if (text == NULL)
			err(1, "realloc failed.");
		fgets(buffer, BUFFLEN, file);
	}
#endif
		fgets(buffer, BUFFLEN, file);

	fclose(file);

	cypherchar cyphers[BUFFLEN];

	int i;
	for (i = 0; i < KEYCHARS; i++) {
		keyhits[i].key = keych[i];
		keyhits[i].hitcount = 0;
		memset(keyhits[i].hits, 0, sizeof(keyhits[i].hits));
	}

	file = fopen(OUTPUTPATH, "w");
	if (file == NULL)
	{
		fprintf(stderr, "unable to open file");
		exit(-1);
	}

	for (i = 0; i < BUFFLEN - 1; i++)
	{
		cyphers[i].c = buffer[i];
		cyphers[i].hits = 0;
		test(&cyphers[i], i);
		//printpairs(file, &cyphers[i]);
	}

	fclose(file);

	//qsort(keyhits, KEYCHARS, sizeof(keyhitpair), most_hits);
	//printkeyhits(keyhits, KEYCHARS);
	qsort(keyhits, KEYCHARS, sizeof(keyhitpair), first_hit);
	printkeyhitstats(keyhits, KEYCHARS);
	

#if DYNAMIC
	free(text);
#endif

}
