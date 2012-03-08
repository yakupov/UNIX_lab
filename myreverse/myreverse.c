#include <unistd.h>

#define BUFSIZE 10

void reverseAndPrint (char* buffer, int sz) {
	for (int i = sz - 2; i >= 0; --i) {
		write (1, buffer + i, 1);
	}

	write (1, buffer + sz - 1, 1);
}

int main() {
	int readLastTime = 123;
	char buffer[BUFSIZE];
	char tailRemaining = 0;

	while (readLastTime > 1) {
		readLastTime = read (0, buffer, BUFSIZE);

		if (readLastTime < BUFSIZE || buffer[BUFSIZE - 1] == '\n') {
			if (!tailRemaining) {
				reverseAndPrint (buffer, readLastTime);
			} else {
				tailRemaining = 0;
				readLastTime = 123;
			}
		} else {
			tailRemaining = 1;
		}
	}

	return 0;
}

