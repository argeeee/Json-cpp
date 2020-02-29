#ifndef CONSOLE_API
#define CONSOLE_API

enum class ConsoleColor
{
    Black,
    DarkBlue,
    Green,
    Cyan,
    DarkRed,
    Purple,
    Yellow,
    LightGray,
    Gray,
    Blue,
    Lime,
    LightBlue,
    Red,
    Pink,
    LightYellow,
    White,
};

void SetConsoleColor(ConsoleColor color);
void ResetConsole();

#endif