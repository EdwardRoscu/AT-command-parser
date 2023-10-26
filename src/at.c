#include <stdio.h>
#include <stdlib.h>
#include "../include/at.h"

#define MAX_LINE_COUNT 1000
#define MAX_LINE_SIZE 1000
#define CARRIAGE_RETURN 13
#define LINE_FEED 10

struct {
	int line_count, crt_line_size;
	char data[MAX_LINE_COUNT][MAX_LINE_SIZE + 1];
	int isOK;
} result;

void init();
int save_char(char ch);
int end_str();
int increase_line_count();
void print_result();
int unexpected_char_error(char received, char* expected);

static int state = 0;

int at_parse(char ch) {
	switch (state) {
	case 0: {
		if (ch == CARRIAGE_RETURN) {
			state = 1;
			init();
			return 0;
		}
		else return unexpected_char_error(ch, "[CARRIAGE RETURN]");
	}
	case 1: {
		if (ch == LINE_FEED) {
			state = 2;
			return 0;
		}
		else return unexpected_char_error(ch, "[LINE FEED]");
	}
	case 2: {
		if (ch == '+') {
			state = 30;
			return increase_line_count();
		}
		else if (ch == 'O') {
			state = 4;
			return 0;
		}
		else if (ch == 'E') {
			state = 50;
			return 0;
		}
		else return unexpected_char_error(ch, "\'+\', \'O\' or \'E\'");
	}
	case 30: {
		if (ch >= 32 || ch <= 126) {
			state = 31;
			return save_char(ch);
		}
		else return unexpected_char_error(ch, "ASCII between 32 and 126");
	}
	case 31: {
		if (ch >= 32 && ch <= 126) {
			state = 31;
			return save_char(ch);
		}
		else if (ch == CARRIAGE_RETURN) {
			state = 32;
			return end_str();
		}
		else return unexpected_char_error(ch, "ASCII between 32 and 126 or [CARRIAGE RETURN]");
	}
	case 32: {
		if (ch == LINE_FEED) {
			state = 33;
			return 0;
		}
		else return unexpected_char_error(ch, "[LINE FEED]");
	}
	case 33: {
		if (ch == '+') {
			state = 30;
			return increase_line_count();
		}
		else if (ch == CARRIAGE_RETURN) {
			state = 34;
			return 0;
		}
		else return unexpected_char_error(ch, "\'+\' or [CARRIAGE RETURN]");
	}
	case 34: {
		if (ch == LINE_FEED) {
			state = 35;
			return 0;
		}
		else return unexpected_char_error(ch, "[LINE FEED]");
	}
	case 35: {
		if (ch == 'O') {
			state = 4;
			return 0;
		}
		else if (ch == 'E') {
			state = 50;
			return 0;
		}
		else return unexpected_char_error(ch, "\'O\' or \'E\'");
	}
	case 4: {
		if (ch == 'K') {
			state = 6;
			result.isOK = 1;
			return 0;
		}
		else return unexpected_char_error(ch, "\'K\'");
	}
	case 50: {
		if (ch == 'R') {
			state = 51;
			return 0;
		}
		else return unexpected_char_error(ch, "\'R\'");
	}
	case 51: {
		if (ch == 'R') {
			state = 52;
			return 0;
		}
		else return unexpected_char_error(ch, "\'R\'");
	}
	case 52: {
		if (ch == 'O') {
			state = 53;
			return 0;
		}
		else return unexpected_char_error(ch, "\'O\'");
	}
	case 53: {
		if (ch == 'R') {
			state = 6;
			result.isOK = 0;
			return 0;
		}
		else return unexpected_char_error(ch, "\'R\'");
	}
	case 6: {
		if (ch == CARRIAGE_RETURN) {
			state = 7;
			return 0;
		}
		else return unexpected_char_error(ch, "[CARRIAGE RETURN]");
	}
	case 7: {
		if (ch == LINE_FEED) {
			state = 0;
			print_result();
			return 0;
		}
		else return unexpected_char_error(ch, "[LINE FEED]");
	}
	}
	printf("Unexpected error! Possible unhandled state!\n");
	return 1;
}

int max_line_count_was_not_printed;

void init() {
	result.line_count = 0;
	result.crt_line_size = 0;
	max_line_count_was_not_printed = 1;
}

int max_line_size_was_not_printed = 1;

int is_max_line_size_reached() {
	if (result.crt_line_size == MAX_LINE_SIZE) {
		if (max_line_size_was_not_printed) {
			printf("Maximum line size reached! There wont be any more writtings on line %d!\n", result.line_count);
			printf("The maximum line size is set to: %d\n\n", MAX_LINE_SIZE);
			max_line_size_was_not_printed = 0;
		}
		return 1;
	}
	return 0;
}

int save_char(char ch) {
	if (is_max_line_size_reached() || (max_line_count_was_not_printed == 0)) {
		return 1;
	}
	int line = result.line_count - 1;
	int pos = result.crt_line_size;
	result.data[line][pos] = ch;
	++result.crt_line_size;
	max_line_size_was_not_printed = 1;
	return 0;
}

int end_str() {
	if (max_line_count_was_not_printed == 0) {
		return 1;
	}
	int line = result.line_count - 1;
	int pos = result.crt_line_size;
	result.data[line][pos] = '\0';
	result.crt_line_size = 0;
	return 0;
}

int is_max_line_count_reached() {
	if (result.line_count == MAX_LINE_COUNT) {
		if (max_line_count_was_not_printed) {
			printf("Maximum line count reached! There wont be any more writtings!\n");
			printf("The maximum line count is set to: %d\n\n", MAX_LINE_COUNT);
			max_line_count_was_not_printed = 0;
		}
		return 1;
	}
	return 0;
}

int increase_line_count() {
	if (is_max_line_count_reached()) {
		return 1;
	}
	++result.line_count;
	return 0;
}

void print_result() {
	for (int i = 0; i < result.line_count; i++) {
		printf("%s\n", result.data[i]);
	}
	result.isOK ? printf("OK\n\n") : printf("ERROR\n\n");
}

int print_unexpected_char_error(char received, char* expected) {
	char* message = (received == CARRIAGE_RETURN) ? "[CARRIAGE RETURN]" :
		(received == LINE_FEED) ? "[LINE FEED]" :
		(char[]) {
		'\'', received, '\'', '\0'
	};
	printf("Error at state %d: Received: %s Expected: %s\n\n", state, message, expected);
}

int unexpected_char_error(char received, char* expected) {
	print_unexpected_char_error(received, expected);
	state = received == CARRIAGE_RETURN;
	return 1;
}