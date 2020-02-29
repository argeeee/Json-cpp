#include "Test.h"
#include "../Console/Console.h"

/**
 * Default constructor
 */
TestAPI::TestAPI() { }

TestAPI::~TestAPI() { }



void TestAPI::TEST(const std::string& test) {
    SetConsoleColor(ConsoleColor::Gray);
    std::cout << "*************************************************\n";
    std::cout << " --- [TESTING] --- [";
    SetConsoleColor(ConsoleColor::Blue);
    std::cout << test;
    SetConsoleColor(ConsoleColor::Gray);
    std::cout << "]\n";
    ResetConsole();
}

void TestAPI::ASSERT(const bool& condition) {
    SetConsoleColor(ConsoleColor::Gray); std::cout << " >>> ";
    SetConsoleColor(condition ? ConsoleColor::Green : ConsoleColor::DarkRed);
    std::cout << ( condition ? "PASSED" : "FAILED") << "\n\n";
    ResetConsole();
}
