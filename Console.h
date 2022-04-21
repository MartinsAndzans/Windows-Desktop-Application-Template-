#pragma once

#ifndef _CONSOLE_
#define _CONSOLE_

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#include <Windows.h>
#include <iostream>
#include <conio.h>

class Console {

private:

	static DWORD Length;
	static DWORD Written;
	static HANDLE hOut;
	static HANDLE hIn;
	static HANDLE HErr;
	static CONSOLE_CURSOR_INFO info;
	static CONSOLE_SCREEN_BUFFER_INFO csbi;
	
public:

	static BOOL setConsoleTitle(CONST CHAR *Title);
	static BOOL setConsoleTextColor(WORD Color);
	static BOOL setCursorPosition(SHORT COORD_X, SHORT COORD_Y, BOOL MOVE_CURSOR = TRUE, BOOL FLUSH_BUFFER = FALSE, BOOL CLS = FALSE);
	static BOOL setCursorVisible(BOOL CURSOR_VISIBLE = TRUE);
	static BOOL setCursorSize(UINT CURSOR_SIZE);
	static RECT getConsoleSize();
	static BOOL setConsoleSize(INT WIDTH, INT HEIGHT);

};

#endif
