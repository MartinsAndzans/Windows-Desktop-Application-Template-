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

	static VOID setConsoleTitle(const char* Title);
	static VOID setCursorPosition(short int x, short int y, bool buffer_flush, bool move_cursor, bool cls);
	static RECT getConsoleRect();
	static VOID setCursorVisible(bool cursor_visible);
	static VOID setCursorSize(int cursor_size);

};

#endif
