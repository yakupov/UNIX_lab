#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 9
const char endl = '\n';


typedef struct node node;
struct node {
	node *parent;
	char data[BUFSIZE];
	int weight;
	int len;
};


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
		//write (1, &endl, 1);
	}
}

node *newNode (char* data, int weight, int len, node *par) {
    if (weight < 0) weight = 0;
    if (len < 0) len = 0;

    node *res = (node*) malloc(sizeof(node));
    res->parent = par;
    res->weight = weight;
    res->len = len;
    memcpy (&(res->data), data, len * sizeof (char));

    return res;
}



void kill (node *nod) {
	if (!nod) return;

	node * tnod = nod;
	while (tnod != 0) {
		node * oldnod = tnod;
		tnod = tnod->parent;
		free (oldnod);
	}		
}


void killAndPrint (node *nod) {
	if (!nod) return;

	node * tnod = nod;
	while (tnod != 0) {
		node * oldnod = tnod;
		tnod = tnod->parent;
		reverseAndPrint (oldnod->data, oldnod->len);
		free (oldnod);
	}		
}


int main() {
	char buffer[BUFSIZE];
	int badString = 0;

	node *tail = 0;//(node *) malloc (sizeof (node));

	while (1) {
		int readLastTime = read (0, buffer, BUFSIZE);
		if (readLastTime <= 0) break;

//		tail = newNode (buffer, readLastTime, readLastTime, tail);

		int beforeEndl;
		int firstEndl = -1;

		for (beforeEndl = 0; beforeEndl < readLastTime; ++beforeEndl) {
			if (buffer[beforeEndl] == '\n') {
				firstEndl = beforeEndl;
				break;
			}
		}

		node * currNode = tail;
		while (currNode != 0) {
			beforeEndl += currNode->len;
			currNode = currNode->parent;
		}

		if (firstEndl < 0) { 
			tail = newNode (buffer, readLastTime, readLastTime, tail);
			continue;
		}

		if (beforeEndl > BUFSIZE) {
			kill (tail);
			tail = 0;
		} else {
			reverseAndPrint (buffer, firstEndl + 1);
			killAndPrint (tail);
			tail = 0;
			write (1, &endl, 1);
		}
		
		for (int i = firstEndl + 1; i < readLastTime; ++i) {
			if (buffer[i] == '\n') {
				reverseAndPrint (buffer + firstEndl + 1, i - firstEndl);
				write (1, &endl, 1);
				firstEndl = i;
			}
		}

		tail = newNode (buffer + (firstEndl + 1), readLastTime - (firstEndl + 1), readLastTime - (firstEndl + 1), tail);
	}

	return 0;
}

