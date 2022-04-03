#pragma once

#ifndef _DROP_FILES_
#define _DROP_FILES_

#include <Windows.h>
#include <string>

#define MAX_DFTITLE_CHAR 256
#define HInstance() GetModuleHandle(NULL)

class DropFiles {

private:

#pragma region CalculatorStaticMembers

	static HDC DropFilesDC;
	static PAINTSTRUCT ps;

	static HDC MemoryDC;
	static HBITMAP Bitmap;

	static COLORREF TextColor;

	static COLORREF DropFilesBackgroundColor;
	static HBRUSH DropFilesBackroundBrush;

	static HFONT DropFilesFont;

	static BOOL FileDroped;

	static RECT Dimensions;

#pragma endregion

#pragma region Functions

	static HFONT createDropFilesFont();

	static VOID drawLineWithSpaces(HDC hdc, INT X, INT Y, INT LineLength, INT LineWidth, UINT Proportion, COLORREF Color, BOOL VerticalLine = FALSE);
	static VOID drawArrow(HDC hdc, INT X = 0, INT Y = 0, INT W = 20, INT H = 20, COLORREF Color = RGB(0, 0, 0));

#pragma endregion

#pragma region Events

	static VOID onCreate(HWND hDropFiles, LPARAM lParam);
	static VOID onPaint(HWND hDropFiles);
	static VOID onDropFiles(HWND hDropFiles, WPARAM wParam);

#pragma endregion

#pragma region CalculatorProcedure

	static LRESULT CALLBACK DropFilesProcedure(HWND hDropFiles, UINT Msg, WPARAM wParam, LPARAM lParam);

#pragma endregion

public:

	#pragma region InitDropFiles

	static BOOL InitDropFiles();

	#pragma endregion
};

#endif
