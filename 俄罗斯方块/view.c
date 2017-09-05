#include "view.h"
#include "control.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

extern const int g_iScene[GAME_ROW];

int DrawScene()
{
    HANDLE g_hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int i = -1;
    int j = -1;
    char szPrint[GAME_ROW * (GAME_COL * sizeof(int) + 2) + 1];
    char *pszPrint = szPrint;
    COORD cd = { 0,0 };

    
    
    for (i = 0; i < GAME_ROW; i++)
    {
        for (j = 0; j < GAME_COL; j++)
        {
            if ((g_iScene[i] << j) & 0x80000000)
                pszPrint += sprintf(pszPrint, "¡ö");
            else
            {
                pszPrint += sprintf(pszPrint, "¡õ");
            }
        }
        pszPrint += sprintf(pszPrint, "\r\n");
    }

    cd.X = 0;
    cd.Y = 0;
    SetConsoleCursorPosition(g_hConsoleOutput, cd);
    printf("%s", szPrint);
    cd.X = 0;
    cd.Y = GAME_ROW;
    SetConsoleCursorPosition(g_hConsoleOutput, cd);

    return 0;
}
