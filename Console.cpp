#include "Console.h"

DWORD Console::Length = 0;
DWORD Console::Written = 0;
HANDLE Console::hOut = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE Console::hIn = GetStdHandle(STD_INPUT_HANDLE);
HANDLE Console::HErr = GetStdHandle(STD_ERROR_HANDLE);
CONSOLE_CURSOR_INFO Console::info = { 0 };
CONSOLE_SCREEN_BUFFER_INFO Console::csbi = { 0 };

BOOL Console::setConsoleTitle(CONST CHAR *Title) {
	return SetConsoleTitleA(Title);
}

BOOL Console::setCursorPosition(SHORT COORD_X, SHORT COORD_Y, BOOL MOVE_CURSOR, BOOL FLUSH_BUFFER, BOOL CLS) {

	COORD Coordinates = { COORD_X, COORD_Y };

	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return FALSE;

	if (MOVE_CURSOR == TRUE) {
		SetConsoleCursorPosition(hOut, Coordinates);
	}
	if (FLUSH_BUFFER == TRUE) {
		std::cout.flush();
	}
	if (CLS == TRUE) {
		Length = csbi.dwSize.X * csbi.dwSize.Y;
		FillConsoleOutputCharacterA(hOut, ' ', Length, Coordinates, &Written);
	}

	return TRUE;

}

RECT Console::getConsoleSize() {

	RECT ConsoleSize = { 0 };
	GetClientRect(GetConsoleWindow(), &ConsoleSize);
	return ConsoleSize;

}

BOOL Console::setConsoleSize(INT WIDTH, INT HEIGHT) {
	return SetWindowPos(GetConsoleWindow(), NULL, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, SWP_SHOWWINDOW | SWP_NOMOVE);
}

BOOL Console::setCursorVisible(BOOL CURSOR_VISIBLE) {

	if (!GetConsoleCursorInfo(hOut, &info)) return FALSE;
	info.bVisible = CURSOR_VISIBLE;
	SetConsoleCursorInfo(hOut, &info);

	return TRUE;

}

BOOL Console::setCursorSize(UINT CURSOR_SIZE) {

	if (!GetConsoleCursorInfo(hOut, &info)) return FALSE;

	if (CURSOR_SIZE >= 1 && CURSOR_SIZE <= 100) {
		info.dwSize = CURSOR_SIZE;
		SetConsoleCursorInfo(hOut, &info);
	} else {
		info.dwSize = 20;
		SetConsoleCursorInfo(hOut, &info);
	}

	return TRUE;

}
