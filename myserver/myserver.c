#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE 20
const char endl = '\n';

typedef struct client client;
struct client {
	char buffer[BUFSIZE];
	char queue[BUFSIZE]; //item queue w. length == 1

	struct sockaddr_in addr;
	int sock;
	int queue_item_size;
	int offset;
	int badString;
};


/*
void reverseAndPrint (char* buffer, int sz) {
	int wrote = 0;

	for (int i = sz - 1; i >= 0; --i) {
//	for (int i = 0; i < sz; ++i) {
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
*/

int main(int argc, char **argv) {
	client clients[9000];
	int count = 0;
	int * ports = malloc (argc * sizeof (int));

	for (int i = 0; i < argc; ++i) {
		ports[i] = socket(AF_INET, SOCK_STREAM, 0);
		if(ports[i] < 0) {
			perror("socket");
			exit(1);
		}

		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(atoi (argv[i]));
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if(bind(ports[i], (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("bind");
			exit(2);
		}

		listen(ports[i], 1);
	}


	while(1) {
		for (int i = 0; i < argc; ++i) {
			struct sockaddr_in addr;
			int sz;
			int sock = accept(ports[i], (struct sockaddr *)&addr, &sz);
			if(sock >= 0) {
				//add new socket
				memcpy (&(clients[count].addr), &addr, sizeof (struct sockaddr_in));
				clients[count].queue_item_size = 0;
				clients[count].offset = 0;
				clients[count].badString = 0;
				clients[count].sock = sock;
				++count;
				
				char addr_str[100];
				sprintf(addr_str, "Connection from %d", clients[count - 1].addr.sin_addr);

				for (int ii = 0; ii < count - 1; ++ii) {
					send(clients[ii].sock, addr_str, strlen(addr_str), 0);
					send(clients[ii].sock, &endl, 1, 0);
				}
			}
		}

		for (int i = 0; i < count; ++i) {
			int stringBeginning = 0;
			int readLastTime = recv(clients[i].sock, clients[i].buffer + clients[i].offset, BUFSIZE - clients[i].offset, 0);
			if(readLastTime < 0) {
				//delete socket
				memmove (clients + i * sizeof (struct client), clients + (i + 1) * sizeof (struct client), sizeof (struct client) * (count - i));
				--count;
			}


			for (int ii = clients[i].offset; ii < clients[i].offset + readLastTime; ++ii) {
				if (clients[i].buffer[ii] == '\n') {
					if (!clients[i].badString) {
						//pop from queue
						if (clients[i].queue_item_size > 0) {
							for (int j = 0; j < argc; ++j) {
								if (j == i)
									continue;
								send(clients[j].sock, clients[i].queue, clients[i].queue_item_size, 0);
								send(clients[j].sock, &endl, 1, 0);
							}							
						}

						clients[i].queue_item_size = ii - stringBeginning + 1;
						memcpy (clients[i].queue, clients[i].buffer + stringBeginning, ii - stringBeginning + 1);
						//reverseAndPrint (clients[i].buffer + stringBeginning, ii - stringBeginning + 1);
					}
					clients[i].badString = 0;
					stringBeginning = ii + 1;
				}
			}

			if (stringBeginning == 0) { //no string was found
				if (readLastTime == BUFSIZE - clients[i].offset) {
					clients[i].badString = 1;
					clients[i].offset = 0;
				} else {
					clients[i].offset += readLastTime;
				}
			} else {
				clients[i].offset = clients[i].offset + readLastTime - stringBeginning;
				memmove (clients[i].buffer, clients[i].buffer + stringBeginning, clients[i].offset);
			}
		}	
	}

	free (ports);
	return 0;
}

