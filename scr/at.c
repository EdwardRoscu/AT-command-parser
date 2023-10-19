#include <stdio.h>
#include <stdlib.h>

#include "../at.h"

int at_parse(char ch) {
	static int state = 0;
	switch (state) {
		case '0': {
			if (ch == 13) {
				state = 1;
				break;
			}
		}
		case '1': {
			if (ch == 10) {
				state = 2;
				break;
			}
			else {
				// error
					break;
			}
		}
	}
}