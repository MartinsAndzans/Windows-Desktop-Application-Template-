#pragma once

#ifndef _CONSOLE_
#define _CONSOLE_

#include <Windows.h>
#include <iostream>
#include <string.h>
#include <conio.h>

class Console {

private:

	static DWORD length;
	static DWORD written;
	static SMALL_RECT consoleSize;
	static CONSOLE_CURSOR_INFO info;
	static CONSOLE_SCREEN_BUFFER_INFO csbi;
	
public:

	static VOID setConsoleTitle(std::string Title) {

		SetConsoleTitleA(Title.c_str());
		
	}

	static VOID setCursorPosition(short int x, short int y, bool buffer_flush, bool move_cursor, bool cls) {

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

	static RECT getConsoleRect() {

		RECT ConsoleSize{};

		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
			abort();
		}

		consoleSize = csbi.srWindow;

		ConsoleSize = { 0, 0, consoleSize.Right, consoleSize.Bottom };

		return ConsoleSize;

	}

	static VOID setCursorVisible(bool cursor_visible) {

		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		if (!GetConsoleCursorInfo(hOut, &info)) {
			abort();
		}

		info.bVisible = cursor_visible;

		SetConsoleCursorInfo(hOut, &info);

	}

	static VOID setCursorSize(int cursor_size) {

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

};

#endif
