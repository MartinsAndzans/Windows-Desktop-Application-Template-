#pragma once

#ifndef _DROP_FILES_
#define _DROP_FILES_

#include <Windows.h>

#define MAX_DROP_FILES_CHAR_STRING 256
#define HInstance() GetModuleHandle(NULL)

#define DropFilesBackroundColor RGB(255, 255, 255)
#define DropFilesForegroundColor RGB(0, 0, 0)

class DropFiles {

private:

	#pragma region CalculatorStaticMembers
	static HDC DropFilesDC;
	static PAINTSTRUCT ps;

	static HDC MemoryDC;
	static HBITMAP Bitmap;

	static HFONT DropFilesFont;
	static BOOL FileDroped;
	static RECT Dimensions;
	#pragma endregion

	#pragma region Functions
	static VOID CreateDropFilesFont();
	static VOID drawDashedRectangle(HDC hdc, RECT &Rectangle, SIZE_T Width, COLORREF Color);
	static VOID FillRectOpacity50(HDC hdc, RECT &Rectangle,COLORREF Color);
	static VOID drawArrow(HDC hdc, INT X = 0, INT Y = 0, INT W = 20, INT H = 20, COLORREF Color = RGB(0, 0, 0));
	#pragma endregion

	#pragma region Events
	static VOID onCreate(HWND hDropFiles, LPARAM lParam);
	static VOID onPaint(HWND hDropFiles);
	static VOID onDropFiles(HWND hDropFiles, WPARAM wParam);
	#pragma endregion

	#pragma region DropFilesProcedure
	static LRESULT CALLBACK DropFilesProcedure(HWND hDropFiles, UINT Msg, WPARAM wParam, LPARAM lParam);
	#pragma endregion

public:

	typedef struct DropFilesStyle {
		COLORREF BackgroundColor;
		COLORREF ForegroundColor;
	}DFStyle, *LPDFStyle, *LPDropFilesStyle;

	#pragma region InitDropFiles
	static BOOL InitDropFiles();
	#pragma endregion
};

#endif
