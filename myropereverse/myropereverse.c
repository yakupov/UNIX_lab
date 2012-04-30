#include <unistd.h>
#include <string.h>

const int BUFSIZE = 9;
const char endl = '\n';

void reverseAndPrint (char* buffer, int sz) {
	int wrote = 0;

	for (int i = sz - 1; i >= 0; --i) {
		if (*(buffer + i) != 0) {
			++wrote;
			if (*(buffer + i) != '\n') {			
				write (1, buffer + i, 1);
			}
		}
	}

	if (wrote > 0) {
		write (1, &endl, 1);
	}
}

int main() {
	char buffer[BUFSIZE];
	int offset = 0;
	int badString = 0;

	while (1) {
		int readLastTime = read (0, buffer + offset, BUFSIZE - offset);
		int stringBeginning = 0;

		if (readLastTime <= 0) break;

		for (int i = offset; i < offset + readLastTime; ++i) {
			if (buffer[i] == '\n') {
				if (!badString) {
					reverseAndPrint (buffer + stringBeginning, i - stringBeginning + 1);
				}
				badString = 0;
				stringBeginning = i + 1;
			}
		}
/*
		if (readLastTime < BUFSIZE - offset) {
			if (!badString) {
				reverseAndPrint (buffer + stringBeginning, offset + readLastTime - stringBeginning);
			}
			stringBeginning = BUFSIZE;
		}
*/

		if (stringBeginning == 0) { //no string was found
			if (readLastTime == BUFSIZE - offset) {
				badString = 1;
				offset = 0;
			} else {
				offset += readLastTime;
			}
		} else {
			//badString = 0;
			//offset = BUFSIZE - stringBeginning;
			offset = offset + readLastTime - stringBeginning;
			memmove (buffer, buffer + stringBeginning, offset);
		}
	}

	return 0;
}

