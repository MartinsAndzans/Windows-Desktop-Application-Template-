#include "Console.h"

DWORD Console::Length = 0;
DWORD Console::Written = 0;
HANDLE Console::hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE Console::hInput = GetStdHandle(STD_INPUT_HANDLE);
HANDLE Console::hError = GetStdHandle(STD_ERROR_HANDLE);
CONSOLE_CURSOR_INFO Console::info = { 0 };
CONSOLE_SCREEN_BUFFER_INFO Console::csbi = { 0 };

BOOL Console::setConsoleTitle(LPCSTR Title) {
	return SetConsoleTitleA(Title);
}

BOOL Console::setConsoleTextColor(WORD Color) {
	return SetConsoleTextAttribute(hOutput, FOREGROUND_INTENSITY | Color);
}

VOID Console::Print(LPCSTR Text, WORD Color) {
	SetConsoleTextAttribute(hOutput, FOREGROUND_INTENSITY | Color);
	std::cout << Text << std::endl;
}

VOID Console::drawRectangleInConsole(CONST SHORT Width, CONST SHORT Height, CONST CHAR Symbol, CONST WORD Color) {

	SetConsoleTextAttribute(hOutput, FOREGROUND_INTENSITY | Color);

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

	SetConsoleTextAttribute(hOutput, FOREGROUND_INTENSITY | Color);

	for (SHORT H = 1; H < Height; H++) {
		for (SHORT W = 1; W < Width; W++) {

		}
		std::cout << std::endl;
	}

}

BOOL Console::setCursorPosition(SHORT COORD_X, SHORT COORD_Y, BOOL MOVE_CURSOR, BOOL FLUSH_BUFFER, BOOL CLS) {

	COORD Coordinates = { COORD_X, COORD_Y };

	if (!GetConsoleScreenBufferInfo(hOutput, &csbi)) return FALSE;

	if (MOVE_CURSOR == TRUE) {
		SetConsoleCursorPosition(hOutput, Coordinates);
	}
	if (FLUSH_BUFFER == TRUE) {
		std::cout.flush();
	}
	if (CLS == TRUE) {
		Length = csbi.dwSize.X * csbi.dwSize.Y;
		FillConsoleOutputCharacterA(hOutput, ' ', Length, Coordinates, &Written);
	}

	return TRUE;

}

BOOL Console::setCursorVisible(BOOL CURSOR_VISIBLE) {

	if (!GetConsoleCursorInfo(hOutput, &info)) return FALSE;
	info.bVisible = CURSOR_VISIBLE;
	SetConsoleCursorInfo(hOutput, &info);

	return TRUE;

}

BOOL Console::setCursorSize(UINT CURSOR_SIZE) {

	if (!GetConsoleCursorInfo(hOutput, &info)) return FALSE;
	if (CURSOR_SIZE >= 1 && CURSOR_SIZE <= 100) {
		info.dwSize = CURSOR_SIZE;
		SetConsoleCursorInfo(hOutput, &info);
	} else {
		info.dwSize = 20;
		SetConsoleCursorInfo(hOutput, &info);
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
