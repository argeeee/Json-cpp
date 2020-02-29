
#include <windows.h>
#include "./Console.h"


/**
 * Setting a console color 
 */
void SetConsoleColor(ConsoleColor color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (int)color);
}

/**
 * Resetting the default console color
 */
void ResetConsole()
{
    SetConsoleColor(ConsoleColor::White); 
}