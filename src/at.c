#include <stdio.h>
#include <stdlib.h>

#include "../at.h"

static int state = 0;

void init() {
	result.line_count = 0;
	result.crt_line_size = 0;
}

int at_parse(char ch) {
	switch (state) {
		case 0: {
			if (ch == CARRIAGE_RETURN) {
				state = 1;
				init();
				break;
			}
			else {
				print_error(ch, "[CARRIAGE RETURN]");
				return 0;
			}
		}
		case 1: {
			if (ch == LINE_FEED) {
				state = 2;
				break;
			}
			else {
				print_error(ch, "[LINE FEED]");
				return 0;
			}
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
			else {
				print_error(ch, "\'+\', \'O\' or \'E\'");
				return 0;
			}
		}
		case 30: {
			if (ch >= 32 || ch <= 126) {
				state = 31;
				return save_char(ch);
			}
			else {
				print_error(ch, "ASCII between 32 and 126");
				return 0;
			}
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
			else {
				print_error(ch, "ASCII between 32 and 126 or [CARRIAGE RETURN]");
				return 0;
			}
		}
		case 32: {
			if (ch == LINE_FEED) {
				state = 33;
				break;
			}
			else {
				print_error(ch, "[LINE FEED]");
				return 0;
			}
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
			else {
				print_error(ch, "\'+\' or [CARRIAGE RETURN]");
				return 0;
			}
		}
		case 34: {
			if (ch == LINE_FEED) {
				state = 35;
				break;
			}
			else {
				print_error(ch, "[LINE FEED]");
				return 0;
			}
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
			else {
				print_error(ch, "\'O\' or \'E\'");
				return 0;
			}
		}
		case 4: {
			if (ch == 'K') {
				state = 6;
				result.isOK = true;
				break;
			}
			else {
				print_error(ch, "\'K\'");
				return 0;
			}
		}
		case 50: {
			if (ch == 'R') {
				state = 51;
				break;
			}
			else {
				print_error(ch, "\'R\'");
				return 0;
			}
		}
		case 51: {
			if (ch == 'R') {
				state = 52;
				break;
			}
			else {
				print_error(ch, "\'R\'");
				return 0;
			}
		}
		case 52: {
			if (ch == 'O') {
				state = 53;
				break;
			}
			else {
				print_error(ch, "\'O\'");
				return 0;
			}
		}
		case 53: {
			if (ch == 'R') {
				state = 6;
				result.isOK = false;
				break;
			}
			else {
				print_error(ch, "\'R\'");
				return 0;
			}
		}
		case 6: {
			if (ch == CARRIAGE_RETURN) {
				state = 7;
				break;
			}
			else {
				print_error(ch, "[CARRIAGE RETURN]");
				return 0;
			}
		}
		case 7: {
			if (ch == LINE_FEED) {
				state = 0;
				print_result();
				break;
			}
			else {
				print_error(ch, "[LINE FEED]");
				return 0;
			}
		}
	}
	return 1;
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

void print_result() {
	for (int i = 0; i < result.line_count; i++) {
		printf("%s\n", result.data[i]);
	}
	result.isOK ? printf("OK") : printf("ERROR");
	printf("\n\n");
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

void print_error(char recieved, char *expected) {
	printf("Error at state %d: Recieved: \'%c\' Expected: %s\n", state, recieved, expected);
}