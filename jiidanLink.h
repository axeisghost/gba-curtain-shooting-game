struct jiidan {
	int row1;
	int row2;
	int col;
	struct jiidan* after;
};

void initJiidan(struct jiidan* newJ, int rrow1, int rcol);
void attachNext(struct jiidan* prev, int rrow1, int rcol);
struct jiidan* next(struct jiidan* prev);
struct jiidan* nextAndDelete(struct jiidan* prev);