#include <stdio.h>
#include <stdlib.h>

#include "../at.h"

int main(int argc, char **argc) {
	f = fopen(argv[1], "rb");

	while ((ch = fgetc(f)) != EOF) {
		if (at_parse(ch) == 1) {
			//print rez ok
		}
	}
}