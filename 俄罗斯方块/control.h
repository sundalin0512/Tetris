#pragma once

#define GAME_ROW 24
#define GAME_COL 12
#define BRICK_TYPES_NUM 7

typedef struct
{
    int iData[4];
    int iType;
    int iRotate;
    int iAxisRow;
    int iAxisCol;
}Brick;

int InitGame();
int UpdateScene();
int CheckInput();
int OnDown();