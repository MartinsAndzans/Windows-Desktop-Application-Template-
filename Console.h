#ifndef _CONSOLE_
#define _CONSOLE_

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#include <ciso646>
#include <Windows.h>
#include <iostream>
#include <conio.h>

class Console {
	
public:

	static BOOL setConsoleTitle(LPCSTR Title) {
		return SetConsoleTitleA(Title);
	}

	static BOOL setConsoleTextColor(WORD Color = 0x0F) {
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		return SetConsoleTextAttribute(hOutput, FOREGROUND_INTENSITY | Color);
	}

	static VOID Print(LPCSTR Text, WORD Color = 0x0F) {
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOutput, FOREGROUND_INTENSITY | Color);
		std::cout << Text << std::endl;
	}

	static CHAR Pause(LPCSTR MessageToUser = "Press any key to continue . . .", WORD Color = 0x0F) {
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOutput, FOREGROUND_INTENSITY | Color);
		std::cout << MessageToUser << std::endl;
		return _getch();
	}

	static VOID drawRectangleInConsole(CONST USHORT Width, CONST USHORT Height, CONST CHAR Symbol = '*', CONST WORD Color = 0x0F) {

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOutput, FOREGROUND_INTENSITY | Color);

		for (USHORT Y = 1; Y <= Height; Y++) {
			for (USHORT X = 1; X <= Width; X++) {
				if (Y == 1 or Y == Height) {
					std::cout << Symbol;
				} else {
					if (X == 1 or X == Width) {
						std::cout << Symbol;
					} else {
						std::cout << ' ';
					}
				}
			}
			std::cout << std::endl;
		}

	}

	static BOOL setCursorPosition(SHORT COORD_X, SHORT COORD_Y, BOOL MOVE_CURSOR = TRUE, BOOL FLUSH_BUFFER = FALSE, BOOL CLS = FALSE) {

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

		COORD Coordinates = { COORD_X, COORD_Y };

		CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
		if (GetConsoleScreenBufferInfo(hOutput, &csbi) == FALSE)
			return FALSE;

		if (MOVE_CURSOR == TRUE) 
			SetConsoleCursorPosition(hOutput, Coordinates);

		if (FLUSH_BUFFER == TRUE)
			std::cout.flush();

		if (CLS == TRUE) {
			DWORD Length = 0, Written = 0;
			Length = csbi.dwSize.X * csbi.dwSize.Y;
			FillConsoleOutputCharacterA(hOutput, ' ', Length, Coordinates, &Written);
		}

		return TRUE;

	}

	static BOOL setCursorVisible(BOOL CURSOR_VISIBLE = TRUE) {

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_CURSOR_INFO ci = { 0 };
		if (!GetConsoleCursorInfo(hOutput, &ci))
			return FALSE;

		ci.bVisible = CURSOR_VISIBLE;
		SetConsoleCursorInfo(hOutput, &ci);

	}

	static BOOL setCursorSize(USHORT CURSOR_SIZE = 20) {

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_CURSOR_INFO ci = { 0 };
		if (!GetConsoleCursorInfo(hOutput, &ci))
			return FALSE;

		if (CURSOR_SIZE <= 100) {
			ci.dwSize = CURSOR_SIZE;
			SetConsoleCursorInfo(hOutput, &ci);
		} else {
			ci.dwSize = 20;
			SetConsoleCursorInfo(hOutput, &ci);
		}

	}

	static RECT getConsoleSize() {

		RECT ConsoleSize = { 0 };
		GetClientRect(GetConsoleWindow(), &ConsoleSize);
		return ConsoleSize;

	}

	static BOOL setConsoleSize(INT WIDTH, INT HEIGHT) {
		return SetWindowPos(GetConsoleWindow(), NULL, 0, 0, WIDTH, HEIGHT, SWP_SHOWWINDOW | SWP_NOMOVE);
	}

};

#endif // _CONSOLE_
