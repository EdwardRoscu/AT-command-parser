#define MAX_LINE_NUMBER 500
#define MAX_LINE_SIZE 500

struct {
	int line_count;
	char data[MAX_LINE_NUMBER][MAX_LINE_SIZE + 1];
	bool isOK;
};