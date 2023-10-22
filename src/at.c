#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/at.h"

#define MAX_LINE_NUMBER 1000
#define MAX_LINE_SIZE 1000
#define CARRIAGE_RETURN 13
#define LINE_FEED 10

struct {
	int line_count, crt_line_size;
	char data[MAX_LINE_NUMBER][MAX_LINE_SIZE + 1];
	bool isOK;
} result;

FILE* fptr;
static int state = 0;

void validate_arguments(int argc, char** argv);
void open_file(char* file);
void parse_file();
void close_file();

void run_AT_command_parser(int argc, char** argv) {
	validate_arguments(argc, argv);
	open_file(argv[1]);
	parse_file();
	close_file();
}

void init();
int save_char(char ch);
void end_str();
int increase_line_count();
void print_result();
int handle_error(char received, char* expected);

int at_parse(char ch) {
	switch (state) {
	case 0: {
		if (ch == CARRIAGE_RETURN) {
			state = 1;
			init();
			break;
		}
		else exit(handle_error(ch, "[CARRIAGE RETURN]"));
	}
	case 1: {
		if (ch == LINE_FEED) {
			state = 2;
			break;
		}
		else exit(handle_error(ch, "[LINE FEED]"));
	}
	case 2: {
		if (ch == '+') {
			state = 30;
			return increase_line_count();
		}
		else if (ch == 'O') {
			state = 4;
			break;
		}
		else if (ch == 'E') {
			state = 50;
			break;
		}
		else exit(handle_error(ch, "\'+\', \'O\' or \'E\'"));
	}
	case 30: {
		if (ch >= 32 || ch <= 126) {
			state = 31;
			return save_char(ch);
		}
		else exit(handle_error(ch, "ASCII between 32 and 126"));
	}
	case 31: {
		if (ch >= 32 && ch <= 126) {
			state = 31;
			return save_char(ch);
		}
		else if (ch == CARRIAGE_RETURN) {
			state = 32;
			end_str();
			break;
		}
		else exit(handle_error(ch, "ASCII between 32 and 126 or [CARRIAGE RETURN]"));
	}
	case 32: {
		if (ch == LINE_FEED) {
			state = 33;
			break;
		}
		else exit(handle_error(ch, "[LINE FEED]"));
	}
	case 33: {
		if (ch == '+') {
			state = 30;
			return increase_line_count();
		}
		else if (ch == CARRIAGE_RETURN) {
			state = 34;
			break;
		}
		else exit(handle_error(ch, "\'+\' or [CARRIAGE RETURN]"));
	}
	case 34: {
		if (ch == LINE_FEED) {
			state = 35;
			break;
		}
		else exit(handle_error(ch, "[LINE FEED]"));
	}
	case 35: {
		if (ch == 'O') {
			state = 4;
			break;
		}
		else if (ch == 'E') {
			state = 50;
			break;
		}
		else exit(handle_error(ch, "\'O\' or \'E\'"));
	}
	case 4: {
		if (ch == 'K') {
			state = 6;
			result.isOK = true;
			break;
		}
		else exit(handle_error(ch, "\'K\'"));
	}
	case 50: {
		if (ch == 'R') {
			state = 51;
			break;
		}
		else exit(handle_error(ch, "\'R\'"));
	}
	case 51: {
		if (ch == 'R') {
			state = 52;
			break;
		}
		else exit(handle_error(ch, "\'R\'"));
	}
	case 52: {
		if (ch == 'O') {
			state = 53;
			break;
		}
		else exit(handle_error(ch, "\'O\'"));
	}
	case 53: {
		if (ch == 'R') {
			state = 6;
			result.isOK = false;
			break;
		}
		else exit(handle_error(ch, "\'R\'"));
	}
	case 6: {
		if (ch == CARRIAGE_RETURN) {
			state = 7;
			break;
		}
		else exit(handle_error(ch, "[CARRIAGE RETURN]"));
	}
	case 7: {
		if (ch == LINE_FEED) {
			state = 0;
			print_result();
			break;
		}
		else exit(handle_error(ch, "[LINE FEED]"));
	}
	}
	return 1;
}

void init() {
	result.line_count = 0;
	result.crt_line_size = 0;
}

int save_char(char ch) {
	if (result.crt_line_size == MAX_LINE_SIZE) {
		printf("Maximum line size reached!\n");
		return 0;
	}
	int line = result.line_count - 1;
	int pos = result.crt_line_size;
	result.data[line][pos] = ch;
	++result.crt_line_size;
	return 1;
}

void end_str() {
	int line = result.line_count - 1;
	int pos = result.crt_line_size;
	result.data[line][pos] = '\0';
	result.crt_line_size = 0;
}

int increase_line_count() {
	if (result.line_count < MAX_LINE_NUMBER) {
		++result.line_count;
		return 1;
	}
	else {
		printf("Maximum line count reached!\n");
		return 0;
	}
}

void print_result() {
	for (int i = 0; i < result.line_count; i++) {
		printf("%s\n", result.data[i]);
	}
	result.isOK ? printf("OK") : printf("ERROR");
	printf("\n\n");
}

void print_error(char received, char* expected) {
	char* message = (received == CARRIAGE_RETURN) ? "[CARRIAGE RETURN]" :
		(received == LINE_FEED) ? "[LINE FEED]" :
		(char[]) {
		'\'', received, '\'', '\0'
	};
	printf("Error at state %d: Received: %s Expected: %s\n", state, message, expected);
}

int handle_error(char received, char* expected) {
	print_error(received, expected);
	fclose(fptr);
	return 1;
}

void validate_arguments(int argc, char** argv) {
	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		exit(1);
	}
}

void open_file(char* file) {
	fptr = fopen(file, "rb");
	if (fptr == NULL) {
		perror("Error opening the file");
		exit(1);
	}
}

void parse_file() {
	char ch;
	while ((ch = fgetc(fptr)) != EOF) {
		if (at_parse(ch) == 0) {
			break;
		}
	}
}

void close_file() {
	fclose(fptr);
}