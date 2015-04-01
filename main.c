#include "myLib.h"
#include "jiidanLink.h"
#include "gameStart.h"
#include "gameOver.h"
#include "gameWin.h"
#include "yuyuko.h"
#include "reimu.h"


int main()
{
	bool game = false;
	bool gameend = false;
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	u16 bk = BLACK;
	u16 wh = WHITE;
	while (1) {
		waitForVblank();
		DMA[3].src = gameStart;
		DMA[3].dst = videoBuffer;
		DMA[3].cnt = 38400|DMA_ON;
		while (!game) {
			if (KEY_DOWN_NOW(BUTTON_START)) {
				game = true;
			}
		}
		u16 colors[] = {RED, YELLOW, BLUE, MAGENTA, GREEN};
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
			danma[i].row = 70;
			danma[i].col = 19;
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
		player1.rs = 4;
		player1.cs = 4;
		player1.life = MAX_LIFE;
		player1.size = 4;
		player1.flash = false;
		player1.flashtime = 80;
		REIMU oldplayer1 = player1;
		OverLockdrawRect(0, 0, 160, 240, BLACK);
		while (game) {
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
				if (player1.row > (UPPER_BOUND + player1.size)) {
					player1.row = player1.row - player1.rs;
				}
			}
			if (KEY_DOWN_NOW(BUTTON_DOWN)) {
				if (player1.row < (LOWER_BOUND - player1.size)) {
					player1.row = player1.row + player1.rs;
				}
			}
			if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
				if (player1.col < (RIGHT_BOUND- player1.size)) {
					player1.col = player1.col + player1.cs;
				}
			}
			if (KEY_DOWN_NOW(BUTTON_LEFT)) {
				if (player1.col > (player1.size + LEFT_BOUND)) {
					player1.col = player1.col - player1.cs;
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
			if (!player1.flash) {
				for (i = 0; i < DIFFICULTY; i++) {
					cur = danma + i;
					if (reimuHitCheck(player1, cur)) {
						//drawRect(UPPER_BOUND, 0, 7, 7, cur->color);
						player1.life--;
						player1.flash = true;
					}
				}
			}
			sprintf(str1, "HP:%d", boss_HP);
			if (player1.flash) {
				if (player1.flashtime > 0) {
					player1.flashtime--;
				} else {
					player1.flash = false;
					player1.flashtime = 60;
				}
			}
			//---------------------------------------------------------------------------------------------------
			waitForVblank();
			// fixedDrawImage3(oldplayer1.row - 35, oldplayer1.col - 17, 34, 50,&bk);
			// drawImage3(player1.row - 35, player1.col - 17, 34, 50,reimu);
			drawCircle(oldplayer1.row, oldplayer1.col, oldplayer1.size, BLACK);
			if (player1.flash) {
				drawCircle(player1.row, player1.col, player1.size, CYAN);
			} else {
				drawCircle(player1.row, player1.col, player1.size, WHITE);
			}
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
				DMA[3].src = &bk;
				DMA[3].dst = videoBuffer + OFFSET(curr->row1, curr->col + JIIDAN_SPEED, 240);
				DMA[3].cnt = JIIDAN_SIZE|DMA_ON|DMA_SOURCE_FIXED;
				if (curr->col <= 0) {
					if ((curr->row1 > 50) && (curr->row1 < 100)) {
						//drawCircle(head->row1, (int)(rand() % 38), 5 + rand() % 10, RED);
						boss_HP--;
					}
					DMA[3].src = &bk;
					DMA[3].dst = videoBuffer + OFFSET(head->row1, head->col, 240);
					DMA[3].cnt = JIIDAN_SIZE|DMA_ON|DMA_SOURCE_FIXED;
					head = nextAndDelete(head);
					curr = head;
				} else {
					if (curr->col < player1.col) {
						DMA[3].src = &wh;
						DMA[3].dst = videoBuffer + OFFSET(curr->row1, curr->col, 240);
						DMA[3].cnt = JIIDAN_SIZE|DMA_ON|DMA_SOURCE_FIXED;
					}
					curr = next(curr);
				}
			}
			drawString(3, 10, str1, BLACK);
			sprintf(str1, "HP:%d", boss_HP);
			drawString(3, 10, str1, WHITE);
			drawString(3, 200, str2, BLACK);
			sprintf(str2, "LIFE:%d", player1.life);
			drawString(3, 200, str2, WHITE);
			drawImage3(50, 0, 38, 50, yuyuko);
			oldplayer1 = player1;
			//oldboss_HP = boss_HP;
			//-------------------------------------------------------------------------------------------
			if (player1.life < 0) {
				game = false;
				waitForVblank();
				DMA[3].src = gameOver;
				DMA[3].dst = videoBuffer;
				DMA[3].cnt = 38400|DMA_ON;
				gameend = true;
			}
			if (boss_HP < 0) {
				game = false;
				waitForVblank();
				DMA[3].src = gameWin;
				DMA[3].dst = videoBuffer;
				DMA[3].cnt = 38400|DMA_ON;
				gameend = true;
			}
			if (KEY_DOWN_NOW(BUTTON_SELECT)) {
				game = false;
				OverLockdrawRect(0, 0, 160, 240, BLACK);
			}
		}
		while (gameend) {
			if (KEY_DOWN_NOW(BUTTON_B)) {
				gameend = false;
			}
		}
	}
}
