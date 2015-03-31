#include "myLib.h"
#include "jiidanLink.h"


int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	u16 colors[] = {RED, YELLOW, BLUE, MAGENTA, GREEN};
	u16 prevcolor = WHITE;
	int i;
	DANMAKU danma[DIFFICULTY];
	DANMAKU olddanma[DIFFICULTY];
	DANMAKU* cur;
	struct jiidan* curr;
	struct jiidan* head = 0;
	struct jiidan* tail = 0;
	int boss_HP = BOSS_MAX_HP;
	char str1[6];
	sprintf(str1, "HP:%d", BOSS_MAX_HP);
	char str2[6];
	sprintf(str1, "LIFE:%d", MAX_LIFE);
	for (i = 0; i < DIFFICULTY; i++) {
		danma[i].launchrow = 30 * (i + 1);
		danma[i].launchcol = 60;
		danma[i].row = 80;
		danma[i].col = 0;
		danma[i].rs = 0;
		danma[i].cs = 3;
		danma[i].size = 5;
		danma[i].launch = false;
		danma[i].onfire = false;
		danma[i].delay = 10;
		danma[i].targetrow = danma[i].launchrow;
		danma[i].targetcol = danma[i].launchcol;
		danma[i].color = colors[i];
	}
	REIMU player1;
	player1.row = 80;
	player1.col = 220;
	player1.rs = 5;
	player1.cs = 5;
	player1.life = MAX_LIFE;
	player1.size = 4;
	REIMU oldplayer1 = player1;
	while(1) {
		for (i = 0; i < DIFFICULTY; i++) {
			cur = danma + i;
			danmakuLifecycle(cur, player1);
		}
		curr = head;
		while (curr) {
			curr->col = curr->col - JIIDAN_SPEED;
			curr = next(curr);
		}
		if (KEY_DOWN_NOW(BUTTON_UP)) {
			if (player1.row > (player1.size + UPPER_BOUND)) {
				player1.row = player1.row - player1.rs;
			} else if (player1.row < (player1.size + player1.rs + UPPER_BOUND)) {
				player1.row = player1.size + UPPER_BOUND;
			}
		}
		if (KEY_DOWN_NOW(BUTTON_DOWN)) {
			if (player1.row < (LOWER_BOUND - (player1.size))) {
				player1.row = player1.row + player1.rs;
			} else if (player1.row - (LOWER_BOUND - (player1.size)) <= player1.rs) {
				player1.row = LOWER_BOUND - (player1.size);
			}
		}
		if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
			if (player1.col < (RIGHT_BOUND - (player1.size))) {
				player1.col = player1.col + player1.cs;
			} else if (player1.row - (RIGHT_BOUND - (player1.size)) <= player1.cs) {
				player1.col = RIGHT_BOUND - (player1.size);
			}
		}
		if (KEY_DOWN_NOW(BUTTON_LEFT)) {
			if (player1.col > (player1.size + LEFT_BOUND)) {
				player1.col = player1.col - player1.cs;
			} else if (player1.col < (player1.size + player1.cs + LEFT_BOUND)) {
				player1.col = player1.size + LEFT_BOUND;
			}
		}
		if (KEY_DOWN_NOW(BUTTON_A)) {
			if (!head) {
				//initJiidan(head, player1.row - player1.size, player1.row + player1.size, player1.col);
				head = (struct jiidan*) malloc(sizeof(struct jiidan));
				head->row1 = player1.row;
				head->col = player1.col;
				head->after = 0;
				tail = (struct jiidan*) malloc(sizeof(struct jiidan));
				tail = head;
			} else {
				attachNext(tail, player1.row, player1.col);
				tail = next(tail);
			}
		}
		//--------------------------------------------------------------------------------------------------
		sprintf(str2, "LIFE:%d", player1.life);
		for (i = 0; i < DIFFICULTY; i++) {
			cur = danma + i;
			if (reimuHitCheck(player1, cur)) {
				drawRect(UPPER_BOUND, 0, 7, 7, cur->color);
				player1.life--;
			}
		}
		sprintf(str1, "HP:%d", boss_HP);
		//---------------------------------------------------------------------------------------------------
		waitForVblank();
		for (i = 0; i < DIFFICULTY; i++) {
			drawCircle(olddanma[i].row, olddanma[i].col, olddanma[i].size, BLACK);
		}
		for (i = 0; i < DIFFICULTY; i++) {
			cur = danma + i;
			drawCircle(cur->row, cur->col, cur->size, cur->color);
			olddanma[i] = danma[i];
		}
		curr = head;
		while (curr) {
			if (curr->col <= player1.col) {
				drawRect(curr->row1,curr->col,1,JIIDAN_SIZE,WHITE);
				drawRect(curr->row1,curr->col + JIIDAN_SPEED,1,JIIDAN_SIZE,BLACK);
			}
			curr = next(curr);
		}
		while (head && (head->col < 0)) {
			if ((head->row1 > 60) && (head->row1 < 100)) {
				// drawCircle(head->row1, (int)(rand() % 20), 15 + rand() % 10, RED);
				drawRect(head->row1,head->col,1,JIIDAN_SIZE,BLACK);
				boss_HP--;
			}
			drawRect(head->row1,head->col,1,JIIDAN_SIZE,BLACK);
			head = nextAndDelete(head);
		}
		drawCircle(oldplayer1.row, oldplayer1.col, oldplayer1.size, BLACK);
		drawCircle(player1.row, player1.col, player1.size, WHITE);
		drawString(3, 10, str1, BLACK);
		sprintf(str1, "HP:%d", boss_HP);
		drawString(3, 10, str1, WHITE);
		drawString(3, 200, str2, BLACK);
		sprintf(str2, "LIFE:%d", player1.life);
		drawString(3, 200, str2, WHITE);
		oldplayer1 = player1;
		//oldboss_HP = boss_HP;
		//-------------------------------------------------------------------------------------------
	}
}
