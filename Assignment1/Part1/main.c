#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vinegere.h"

#ifdef DEBUG
#include "debug.h"
#endif

#define true 1
#define false 0

typedef int bool;

void usage() {
	extern char * __progname;
	fprintf(stderr, "usage: %s filepath\n", __progname);
	exit(1);
}

int main(int argc, char * argv[])
{

#ifdef DEBUG
	debug();
	exit(0);
#endif

	FILE * file;
	
	if (argc != 2)
		usage();

	file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "Unable to open file");
		exit(1);
		return;
	}

	bool done = false;
	char keyword[80], plaintext[255];
	int len = 80;

	memset(keyword, 0, sizeof(char));
	printf("enter a keyword: ");
	fgets(keyword, 80, stdin);

	char cyphertext[len];

	while (!done)
	{
		printf("Press Q to quit.");
		char c = getchar();

		if (c == 'Q' || c == 'q')
		{
			done = true;
		}
		
	}

	
}
