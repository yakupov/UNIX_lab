#define BUFSIZE 10
#define size_t int

void reverseAndPrint (char* buffer, size_t sz) {
	for (size_t i = sz - 2; i >= 0; --i) {
		write (1, buffer + i, 1);
	}

	write (1, buffer + sz - 1, 1);
}

int main() {
	size_t readLastTime = 123;
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

		buffer[BUFSIZE - 1] = 'x';
	}

	return 0;
}

