#include "control.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>

Brick g_brick;


const int g_iInitBackGround[GAME_ROW] =
{
    0x80100000, 0x80100000, 0x80100000, 0x80100000,
    0x80100000, 0x80100000, 0x80100000, 0x80100000,
    0x80100000, 0x80100000, 0x80100000, 0x80100000,
    0x80100000, 0x80100000, 0x80100000, 0x80100000,
    0x80100000, 0x80100000, 0x80100000, 0x80100000,
    0x80100000, 0x80100000, 0x80100000, 0xFFF00000
};

const int g_iBrickPool[BRICK_TYPES_NUM][4] =
{
    0xF0000000, 0x80808080, 0xF0000000, 0x80808080,     //I型
    0xE0400000, 0x40C04000, 0x40E00000, 0x40604000,     //T型
    0x40406000, 0x00E08000, 0x60202000, 0x0020E000,     //L型
    0x4040C000, 0x80E00000, 0x60404000, 0x00E02000,     //J型
    0xC0600000, 0x40C08000, 0xC0600000, 0x40C08000,     //Z型
    0x60c00000, 0x80C04000, 0x60c00000, 0x80C04000,     //S型
    0xC0C00000, 0xC0C00000, 0xC0C00000, 0xC0C00000      //O型
};

int g_iScene[GAME_ROW] = { 0 };
int g_iBackGround[GAME_ROW] = { 0 };

void CreateNewBrick()
{
    g_brick.iType = rand() % BRICK_TYPES_NUM;
    g_brick.iRotate = rand() % 4;
    g_brick.iData[0] = g_iBrickPool[g_brick.iType][g_brick.iRotate] >> 24 & 0x000000FF;
    g_brick.iData[1] = g_iBrickPool[g_brick.iType][g_brick.iRotate] >> 16 & 0x000000FF;
    g_brick.iData[2] = g_iBrickPool[g_brick.iType][g_brick.iRotate] >> 8 & 0x000000FF;
    g_brick.iData[3] = g_iBrickPool[g_brick.iType][g_brick.iRotate] & 0x000000FF;
    g_brick.iAxisRow = 0;
    g_brick.iAxisCol = GAME_COL / 2;
}

int UpdateScene()
{
    int i = -1;
    for (i = 0; i < GAME_ROW; i++)
    {
        g_iScene[i] = g_iBackGround[i];
    }
    for (i = 0; i < 4; i++)
    {
        if(g_brick.iAxisRow + i < GAME_ROW)
        {
            g_iScene[g_brick.iAxisRow + i] = g_iBackGround[g_brick.iAxisRow + i]
                | (g_brick.iData[i] << (24 - g_brick.iAxisCol));
        }
    }
    return 0;
}

int InitGame()
{
    srand((unsigned)time(NULL));
    CreateNewBrick();
    memcpy(g_iBackGround, g_iInitBackGround, GAME_ROW * sizeof(int));
    UpdateScene();
    return 0;
}

int TryRotate()
{
    Brick tmpBrick = { 0 };
    int i = 0;
    memcpy(&tmpBrick, &g_brick, sizeof(g_brick));
    
    g_brick.iRotate = (g_brick.iRotate + 1) % 4;
    g_brick.iData[0] = g_iBrickPool[g_brick.iType][g_brick.iRotate] >> 24 & 0x000000FF;
    g_brick.iData[1] = g_iBrickPool[g_brick.iType][g_brick.iRotate] >> 16 & 0x000000FF;
    g_brick.iData[2] = g_iBrickPool[g_brick.iType][g_brick.iRotate] >> 8 & 0x000000FF;
    g_brick.iData[3] = g_iBrickPool[g_brick.iType][g_brick.iRotate] & 0x000000FF;

    for (i = 0; i < 4; i++)
    {
        if (g_iBackGround[g_brick.iAxisRow + i]
            & (g_brick.iData[i] << (24 - g_brick.iAxisCol)))
        {
            memcpy(&g_brick, &tmpBrick, sizeof(g_brick));
            return 1;
        }
    }
    return 0;
}

int TryMove(int x, int y)
{
    Brick tmpBrick = { 0 };
    int i = 0;
    memcpy(&tmpBrick, &g_brick, sizeof(g_brick));

    g_brick.iAxisCol += x;
    g_brick.iAxisRow += y;

    for (i = 0; i < 4; i++)
    {
        if (g_iBackGround[g_brick.iAxisRow + i]
            & (g_brick.iData[i] << (24 - g_brick.iAxisCol)))
        {
            memcpy(&g_brick, &tmpBrick, sizeof(g_brick));
            return 1;
        }
    }

    return 0;
}

int TryDeleteLines()
{
    int i = -1;
    for (i = GAME_ROW - 2; i > 0; i--)
    {
        if (g_iBackGround[i] == 0xFFF00000)
        {
            int j = 0;
            for (j = i; j > 0; j--)
            {
                g_iBackGround[j] = g_iBackGround[j - 1];
            }
            g_iBackGround[0] = g_iInitBackGround[0];
            i++;
        }
    }
    return 0;
}

int FixBrick()
{
    int i = -1;
    for (i = 0; i < 4; i++)
    {
        g_iBackGround[g_brick.iAxisRow + i] = g_iBackGround[g_brick.iAxisRow + i]
            | (g_brick.iData[i] << (24 - g_brick.iAxisCol));
    }
    TryDeleteLines();

    return 0;
}

int OnUp()
{
    TryRotate();
    return 0;
}

int OnDown()
{
    if(TryMove(0, 1))
    {
        FixBrick();
        if (g_iBackGround[0] != g_iInitBackGround[0])
        {
            return -1;
        }
        CreateNewBrick();
    }
    return 0;
}

int OnLeft()
{
    TryMove(-1, 0);
    return 0;
}

int OnRight()
{
    TryMove(1, 0);
    return 0;
}

int CheckInput()
{
    char chInput = '\0';
    int iFlag = 0;
    while (_kbhit())
    {
        iFlag = 1;
        chInput = (char)_getch();
        switch (chInput)
        {
        case 'w':
            OnUp();
            break;
        case 's':
            OnDown();
            break;
        case 'a':
            OnLeft();
            break;
        case 'd':
            OnRight();
            break;
        }
        UpdateScene();
    }

    return iFlag;
}
