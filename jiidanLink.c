#include "jiidanLink.h"
#include <stdlib.h>

void initJiidan(struct jiidan* newJ, int rrow1, int rcol) {
	newJ = (struct jiidan*) malloc(sizeof(struct jiidan));
	newJ->row1 = rrow1;
	newJ->col = rcol;
	newJ->after = 0;
}

void attachNext(struct jiidan* prev, int rrow1, int rcol) {
	struct jiidan* curr;
	prev->after = (struct jiidan*) malloc(sizeof(struct jiidan));
	curr = prev->after;
	curr->row1 = rrow1;
	curr->col = rcol;
	curr->after = 0;
}

struct jiidan* next(struct jiidan* prev) {
	return prev->after;
}

struct jiidan* nextAndDelete(struct jiidan* prev) {
	struct jiidan* curr;
	curr = prev->after;
	free(prev);
	return curr;
}