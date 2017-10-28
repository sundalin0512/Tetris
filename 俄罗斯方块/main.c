#include <stdio.h>  
#include <stdlib.h>
#include <time.h>

#include "control.h"
#include "view.h"

int main1()
{
    InitGame();
    DrawScene();
    clock_t clkStart1 = clock();
    clock_t clkEnd1 = clock();
    clock_t clkStart2 = clock();
    clock_t clkEnd2 = clock();
    while (1)
    {
        clkEnd1 = clock();
        if (clkEnd1 - clkStart1 > 300)
        {
            clkStart1 = clkEnd1;
            if (OnDown() == -1)
            {
                system("cls");
                printf("Game Over!\r\n");
                break;
            }
            UpdateScene();
            
        }
        CheckInput();
        clkEnd2 = clock();
        if (clkEnd2 - clkStart2 > 30)
        {
            clkStart2 = clkEnd2;
            DrawScene();

        }
    }
    system("pause");
    return 0;
}