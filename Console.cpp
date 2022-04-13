#include "Console.h"

DWORD Console::length = 0;
DWORD Console::written = 0;
SMALL_RECT Console::consoleSize = { 0 };
CONSOLE_CURSOR_INFO Console::info = { 0 };
CONSOLE_SCREEN_BUFFER_INFO Console::csbi = { 0 };

VOID Console::setConsoleTitle(const char* Title) {

	SetConsoleTitleA(Title);

}

VOID Console::setCursorPosition(short int x, short int y, bool buffer_flush, bool move_cursor, bool cls) {

	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD coordinates = { (SHORT)x, (SHORT)y };

	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		abort();
	}

	length = csbi.dwSize.X * csbi.dwSize.Y;

	if (buffer_flush == true) {
		std::cout.flush();
	}

	if (move_cursor == true) {
		SetConsoleCursorPosition(hOut, coordinates);
	}

	if (cls == true) {
		FillConsoleOutputCharacter(hOut, L' ', length, coordinates, &written);
	}

}

RECT Console::getConsoleRect() {

	RECT ConsoleSize{};

	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		abort();
	}

	consoleSize = csbi.srWindow;

	ConsoleSize = { 0, 0, consoleSize.Right, consoleSize.Bottom };

	return ConsoleSize;

}

VOID Console::setCursorVisible(bool cursor_visible) {

	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!GetConsoleCursorInfo(hOut, &info)) {
		abort();
	}

	info.bVisible = cursor_visible;

	SetConsoleCursorInfo(hOut, &info);

}

VOID Console::setCursorSize(int cursor_size) {

	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!GetConsoleCursorInfo(hOut, &info)) {
		abort();
	}

	if (cursor_size >= 1 && cursor_size <= 100) {
		info.dwSize = cursor_size;
		SetConsoleCursorInfo(hOut, &info);
	}
	else {
		info.dwSize = 20;
		SetConsoleCursorInfo(hOut, &info);
	}

}