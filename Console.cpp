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

BOOL Console::setConsoleTextColor(WORD Color) {
	return SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY | Color);
}

VOID Console::drawRectangleInConsole(CONST SHORT Width, CONST SHORT Height, CONST CHAR Symbol, CONST WORD Color) {

	SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY | Color);

	for (SHORT H = 1; H <= Height; H++) {
		for (SHORT W = 1; W <= Width; W++) {
			if (H == 1 or H == Height) {
				std::cout << Symbol;
			} else {
				if (W == 1 or W == Width) {
					std::cout << Symbol;
				} else {
					std::cout << ' ';
				}
			}
		}
		std::cout << std::endl;
	}

}

VOID Console::drawTriangleInConsole(CONST SHORT Width, CONST SHORT Height, CONST CHAR Symbol, CONST WORD Color) {

	SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY | Color);

	for (SHORT H = 1; H < Height; H++) {
		for (SHORT W = 1; W < Width; W++) {

		}
		std::cout << std::endl;
	}

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

RECT Console::getConsoleSize() {

	RECT ConsoleSize = { 0 };
	GetClientRect(GetConsoleWindow(), &ConsoleSize);
	return ConsoleSize;

}

BOOL Console::setConsoleSize(INT WIDTH, INT HEIGHT) {
	return SetWindowPos(GetConsoleWindow(), NULL, 0, 0, WIDTH, HEIGHT, SWP_SHOWWINDOW | SWP_NOMOVE);
}
