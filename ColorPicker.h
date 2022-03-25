#pragma once

#ifndef _COLOR_PICKER_
#define _COLOR_PICKER_

#include <Windows.h>
#include <string>

#define MAX_CPTITLE_CHAR 40
#define HInstance() GetModuleHandle(NULL)

#define ColorPickerBackgroundColor RGB(255, 255, 255)

class ColorPicker {

private:

// ATTRIBUTES

#pragma region ColorPickerStaticMembers

	static HDC ColorPickerDC;
	static PAINTSTRUCT ps;

	static HDC MemoryDC;
	static HBITMAP Bitmap;

	static CONST POINT DimensionsSmall;
	static CONST POINT DimensionsLarge;

	static RECT Dimensions;

	static HPEN PBorder;
	static HBRUSH BColor;

	static RECT SelectedColor;

	static POINT mousePosition;

#pragma endregion

	#pragma region Functions

	static INT drawGradientSmall(HDC hdc, INT X = 0, INT Y = 0, INT W = 420, INT H = 40, INT BorderWidth = 2, COLORREF BorderColor = RGB(0, 0, 0));
	static INT drawGradientLarge(HDC hdc, INT X = 0, INT Y = 0, INT W = 420, INT H = 100, INT BorderWidth = 2, COLORREF BorderColor = RGB(0, 0, 0));

	#pragma endregion

	#pragma region Events

	static void onCreate(HWND hColorPicker, LPARAM lParam);
	static void onWindowPosChanged(HWND hColorPicker, LPARAM lParam);
	static void onMouseMove(HWND hColorPicker, WPARAM wParam, LPARAM lParam);
	static void onPaint(HWND hColorPicker);

	#pragma endregion

	#pragma region ColorPickerProcedure

	static LRESULT ColorPickerProcedure(HWND hColorPicker, UINT msg, WPARAM wParam, LPARAM lParam);

	#pragma endregion

public:

	#pragma region InitColorPicker

	static BOOL InitColorPicker();

	#pragma endregion

};

#endif
