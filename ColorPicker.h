#pragma once

#ifndef _COLOR_PICKER_
#define _COLOR_PICKER_

#include <Windows.h>
#include <string>

#define GWL_USERDATA -21

#define CP_SHOW 10001 // SMALL = 420 - 40 | LARGE = 420 - 100

#define MAX_COLOR_PICKER_CHAR_STRING 40
#define HInstance() GetModuleHandle(NULL)

#define ColorPickerBackgroundColor RGB(0, 0, 0)

class ColorPicker {

private:

	#pragma region ColorPickerStaticMembers
	static HDC ColorPickerDC;
	static PAINTSTRUCT ps;

	static HDC MemoryDC;
	static HBITMAP Bitmap;

	static CONST SIZE DimensionsSmall;
	static CONST SIZE DimensionsLarge;

	static POINT mousePosition;
	static BOOL LMButtonPressed;
	static RECT Dimensions;
	#pragma endregion

	#pragma region Functions
	static VOID drawCross(HDC hdc, INT X = 0, INT Y = 0, INT W = 23, INT H = 23, COLORREF Color = RGB(0, 0, 0));
	static SHORT drawGradientSmall(HDC hdc, INT X, INT Y, SHORT BorderWidth = 2, COLORREF BorderColor = RGB(0, 0, 0));
	static INT drawGradientLarge(HDC hdc, INT COORD_X, INT COORD_Y, SHORT BorderWidth = 2, COLORREF BorderColor = RGB(0, 0, 0));
	#pragma endregion

	#pragma region Events
	static VOID onCreate(HWND hColorPicker, LPARAM lParam);
	static VOID onWindowPosChanging(HWND hColorPicker, LPARAM lParam);
	static VOID onMouseMove(HWND hColorPicker, WPARAM wParam, LPARAM lParam);
	static VOID onPaint(HWND hColorPicker);
	#pragma endregion

	#pragma region ColorPickerProcedure
	static LRESULT CALLBACK ColorPickerProcedure(HWND hColorPicker, UINT Msg, WPARAM wParam, LPARAM lParam);
	#pragma endregion

public:

	#pragma region InitColorPicker
	static BOOL InitColorPicker();
	#pragma endregion

};

#endif
