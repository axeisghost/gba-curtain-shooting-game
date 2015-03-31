#include "myLib.h"

void timeForward(int row, int col, int rd, int cd);
int size = 5;
int main()
{
	while (1) {
		REG_DISPCTL = MODE3 | BG2_ENABLE;
		int row = 80;
		int col = 120;	
		timeForward(row, col, 1, 1);
	}
}

void timeForward(int row, int col, int rd, int cd) {
	while(1) // Game Loop
	{
		int tempoldrow = row;
		int tempoldcol = col;
		if (KEY_DOWN_NOW(BUTTON_L)) {
			waitForVblank();
			row = row - rd;
			col = col - cd;
			boundsCheck(&row, 159, &rd, size);
			boundsCheck(&col, 239, &cd, size);
			waitForVblank();
			drawRect(tempoldrow, tempoldcol, size, size, BLACK);
			drawRect(row, col, size, size, RED);
			// drawRect(row, col, size, size, BLACK);
			// drawRect(oldrow, oldcol, size, size, RED);
			// return;
		} else {
			row = row + rd;
			col = col + cd;
			boundsCheck(&row, 159, &rd, size);
			boundsCheck(&col, 239, &cd, size);
			waitForVblank();
			drawRect(tempoldrow, tempoldcol, size, size, BLACK);
			drawRect(row, col, size, size, RED);
			//timeForward(row, col, tempoldrow, tempoldcol, rd, cd);
		}
	}
}

