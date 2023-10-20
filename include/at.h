#include <stdbool.h>

#define MAX_LINE_NUMBER 1000
#define MAX_LINE_SIZE 1000
#define CARRIAGE_RETURN 13
#define LINE_FEED 10

struct {
	int line_count, crt_line_size;
	char data[MAX_LINE_NUMBER][MAX_LINE_SIZE + 1];
	bool isOK;
} result;

int at_parse(char ch);
int save_char(char ch);
void end_str();
void print_result();
int increase_line_count();
void print_error(char recieved, char* expected);