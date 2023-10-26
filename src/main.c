#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "../include/at.h"

int main(int argc, char** argv) {

	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	FILE* fptr = fopen(argv[1], "rb");
	if (fptr == NULL) {
		perror("Error opening the file");
		return 1;
	}

	char ch;
	while ((ch = fgetc(fptr)) != EOF) {
		at_parse(ch);
	}

	fclose(fptr);
	return 0;
}