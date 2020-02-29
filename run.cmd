@echo off

cls && g++ Json\\Json.cpp Console\\Console.cpp Test\\Test.cpp Test\\app.cpp -o bin\\app && bin\\app.exe

echo.
pause