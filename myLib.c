#include "myLib.h"
#include "font.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawChar(int row, int col, char ch, u16 color){
	int r,c;
	for(r=0; r<8; r++){
		for(c=0; c<6; c++){
			if(fontdata_6x8[OFFSET(r,c,6) + 48*ch]){
				setPixel(r + row, c + col, color);
			}	
		}
	}
}

void drawString(int row, int col, char *str, u16 color){
	int chars_drawn = 0;
	while(*str){
		drawChar(row, col + 6*chars_drawn++, *str++, color);
	}
}

void waitForVblank(){
	while(SCANLINE_COUNTER > 160);
	while(SCANLINE_COUNTER < 160);
}

void drawRect(int row, int col, int height, int width, u16 color)
{
	for(int r=0; r<height; r++)
	{
		for(int c=0; c<width; c++)
		{
			if ((row + r > UPPER_BOUND) && (col + c > LEFT_BOUND)) {
				setPixel(row+r, col+c, color);
			}
		}
	}
}

void drawCircle(int crow, int ccol, int radius, u16 color) {
	for(int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			if ((i*i + j*j) < (radius*radius)) {
				if ((crow + i > UPPER_BOUND) && (ccol + j > LEFT_BOUND)) {
					setPixel(crow + i, ccol + j, color);
				}
			}
		}
	}
}

bool reimuHitCheck(REIMU player1, DANMAKU* cur) {
	return ((abs(player1.row - cur->row) < (player1.size + cur->size)) && (abs(player1.col - cur->col) < (player1.size + cur->size)));
}


// int boundsCheck(int *var, int bound, int *delta, int size)
// {
// 		if(*var < 0)
// 		{
// 			*var = 0;
// 			*delta = -*delta;
// 			return 1;
// 		}
// 		if(*var > bound-size+1)
// 		{
// 			*var = bound-size+1;
// 			*delta = -*delta;
// 		}
// 		return 0;

// }

void veloCorrectify(DANMAKU* cur) {
	int ratio;
	int ratio2;
	ratio = abs(cur->targetrow - cur->row) / abs(cur->targetcol - cur->col);
	ratio2 = abs(cur->targetcol - cur->col) / abs(cur->targetrow - cur->row);
	if (ratio <= 0) {
		if ((cur->targetcol - cur-> col) > 0) {
			if ((cur->targetrow - cur-> row) > 0) {
				if (ratio2 == 1) {
					cur->rs = 3;
					cur->cs = 3;
				} else if (ratio2 == 2) {
					cur->rs = 2;
					cur->cs = 4;
				} else if (ratio2 >= 3) {
					cur->rs = 0;
					cur->cs = 5;
				}
			} else {
				if (ratio2 == 1) {
					cur->rs = -3;
					cur->cs = 3;
				} else if (ratio2 == 2) {
					cur->rs = -2;
					cur->cs = 4;
				} else if (ratio2 >= 3) {
					cur->rs = 0;
					cur->cs = 5;
				}
			}
		} else {
			if ((cur->targetrow - cur-> row) > 0) {
				if (ratio2 == 1) {
					cur->rs = 3;
					cur->cs = -3;
				} else if (ratio2 == 2) {
					cur->rs = 2;
					cur->cs = -4;
				} else if (ratio2 >= 3) {
					cur->rs = 0;
					cur->cs = -5;
				}
			} else {
				if (ratio2 == 1) {
					cur->rs = -3;
					cur->cs = -3;
				} else if (ratio2 == 2) {
					cur->rs = -2;
					cur->cs = -4;
				} else if (ratio2 >= 3) {
					cur->rs = 0;
					cur->cs = -5;
				}
			}
		}
	} else {
		if ((cur->targetcol - cur-> col) > 0) {
			if ((cur->targetrow - cur-> row) > 0) {
				if (ratio == 1) {
					cur->rs = 3;
					cur->cs = 3;
				} else if (ratio == 2) {
					cur->rs = 4;
					cur->cs = 2;
				} else if (ratio >= 3) {
					cur->rs = 5;
					cur->cs = 0;
				}
			} else {
				if (ratio == 1) {
					cur->rs = -3;
					cur->cs = 3;
				} else if (ratio == 2) {
					cur->rs = -4;
					cur->cs = 2;
				} else if (ratio >= 3) {
					cur->rs = -5;
					cur->cs = 0;
				}
			}
		} else {
			if ((cur->targetrow - cur-> row) >= 0) {
				if (ratio == 1) {
					cur->rs = 3;
					cur->cs = -3;
				} else if (ratio == 2) {
					cur->rs = 4;
					cur->cs = -2;
				} else if (ratio >= 3) {
					cur->rs = 5;
					cur->cs = 0;
				}
			} else {
				if (ratio == 1) {
					cur->rs = -3;
					cur->cs = -3;
				} else if (ratio == 2) {
					cur->rs = -4;
					cur->cs = -2;
				} else if (ratio >= 3) {
					cur->rs = -5;
					cur->cs = 0;
				}
			}
		}
	}
}

bool checkOnTarget(DANMAKU* cur) {
	return ((abs(cur->row - cur->targetrow) < 5) && (abs(cur->col - cur->targetcol) < 5));
}

void danmakuLifecycle(DANMAKU* cur, REIMU player1) {
	if (boundsCheck(cur)) {
		cur->row = 80;
		cur->col = 0;
		cur->launch = false;
		cur->onfire = false;
		cur->targetrow = cur->launchrow;
		cur->targetcol = cur->launchcol;
	} else {
		if (!cur->launch && !cur->onfire) {
			if (checkOnTarget(cur)) {
				cur->launch = true;
				cur->charge = cur->delay;
			} else {
				veloCorrectify(cur);
				cur->row = cur->row + cur->rs;
				cur->col = cur->col + cur->cs;
			}
		}
		if (cur->launch && !cur->onfire) {
			if (cur->charge > 0) {
				cur->charge = cur->charge - 1;
			} else {
				cur->targetrow = player1.row;
				cur->targetcol = player1.col;
				cur->onfire = true;
			}
		}
		if (cur->launch && cur->onfire) {
			if (checkOnTarget(cur)) {
				cur->launch = false;
				cur->row = cur->row + cur->rs;
				cur->col = cur->col + cur->cs;
			} else {
				veloCorrectify(cur);
				cur->row = cur->row + cur->rs;
				cur->col = cur->col + cur->cs;
			}
		}
		if (!cur->launch && cur->onfire) {
			cur->row = cur->row + cur->rs;
			cur->col = cur->col + cur->cs;
		}
	}
}

bool boundsCheck(DANMAKU* cur) {
	return ((cur->row > LOWER_BOUND) || (cur->col > RIGHT_BOUND) || ((cur->row - cur->size - cur->rs) <= UPPER_BOUND) || (cur->col + cur->size <LEFT_BOUND));
}

void WaitForVblank()
{
	while(SCANLINE_COUNTER > 160);
	while(SCANLINE_COUNTER < 160);
}