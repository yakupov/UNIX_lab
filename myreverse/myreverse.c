#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE 10

void reverseAndPrint (char* buffer, int sz) {
	for (int i = sz - 2; i >= 0; --i) {
		write (1, buffer + i, 1);
	}

	write (1, buffer + sz - 1, 1);
}

int main() {
	char buffer[BUFSIZE];
	int offset = 0;
	int badString = 0;

	while (1) {
		int oldOffset = offset;
		int readLastTime = read (0, buffer + oldOffset, BUFSIZE - oldOffset);
		int stringBeginning = 0;

		for (int i = oldOffset; i < oldOffset + readLastTime; ++i) {
			if (buffer[i] == '\n') {
				if (!badString) {
					reverseAndPrint (buffer + stringBeginning, i - stringBeginning + 1);
				}
				
				badString = 0;
				stringBeginning = i + 1;
				offset = 0;
			}
		}

		if (readLastTime < BUFSIZE - oldOffset) {
			if (!badString) {
				reverseAndPrint (buffer + stringBeginning, oldOffset + readLastTime - stringBeginning);
			}
			return 0;
		}

		if (offset > 0 || stringBeginning == 0) {
			offset = 0;
			badString = 1;
		} else {
			offset = BUFSIZE - stringBeginning;
			memmove (buffer, buffer + stringBeginning, offset);
		}
	}

	return 0;
}

