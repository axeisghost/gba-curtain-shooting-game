typedef unsigned short u16;
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


#define REG_DISPCTL *(unsigned short *)0x4000000
#define SCANLINE_COUNTER (*(unsigned short *) 0x4000006)

#define MODE3 3
#define BG2_ENABLE (1<<10) 
#define COLOR(r, g, b) ((r) | (g)<<5 | (b) <<10)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define CYAN COLOR(0, 31, 31)
#define MAGENTA COLOR(31, 0, 31)
#define YELLOW COLOR(31,31,0)
#define WHITE COLOR(31,31,31)
#define BLACK 0
#define UPPER_BOUND 16
#define LOWER_BOUND 160
#define LEFT_BOUND 0
#define RIGHT_BOUND 240
#define JIIDAN_SPEED 9
#define JIIDAN_SIZE 6
#define BOSS_MAX_HP 500
#define MAX_LIFE 5
#define OFFSET(r, c, numcols) ((r)*(numcols) + (c))

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
#define BUTTONS *(volatile unsigned int *)0x4000130
#define DIFFICULTY 5


extern unsigned short *videoBuffer;

//Flying Objects
typedef struct {
	int launchrow;
	int launchcol;
	int row;
	int col;
	int targetrow;
	int targetcol;
	int rs;
	int cs;
	int size;
	int delay;
	int charge;
	bool launch;
	bool onfire;
	u16 color;
} DANMAKU;

typedef struct {
	int row;
	int col;
	int rs;
	int cs;
	int life;
	int size;
} REIMU;

// Prototypes
void setPixel(int row, int col, u16 color);
void drawChar(int row, int col, char ch, u16 color);
void drawString(int row, int col, char *str, u16 color);
void waitForVblank();
void drawRect(int row, int col, int height, int width, u16 color);
bool boundsCheck(DANMAKU* cur);
void veloCorrectify(DANMAKU *cur);
bool checkOnTarget(DANMAKU* cur);
void danmakuLifecycle(DANMAKU* cur, REIMU player1);
void drawCircle(int crow, int ccol, int radius, u16 color);
bool reimuHitCheck(REIMU player1, DANMAKU* cur);

