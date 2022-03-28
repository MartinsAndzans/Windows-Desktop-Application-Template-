#pragma once

#ifndef _COLOR_PICKER_
#define _COLOR_PICKER_

#include <Windows.h>
#include <string>

#define GWL_USERDATA -21

/*

#define MAKEDWORD(w1, w2) (((DWORD)(BYTE)(w1)) | ((DWORD)(BYTE)(w2) << 8))

#define LODWORD(dw) ((DWORD)(BYTE)(dw))
#define HIDWORD(dw) ((DWORD)(BYTE)((dw) >> 8))

*/

#define CP_SHOW 10001

#define MAX_CPTITLE_CHAR 40
#define HInstance() GetModuleHandle(NULL)

#define ColorPickerBackgroundColor RGB(0, 0, 0)

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

	static POINT mousePosition;

	static BOOL LMButtonPressed;

#pragma endregion

	#pragma region Functions

	static VOID drawCross(HDC hdc, INT X = 0, INT Y = 0, INT W = 23, INT H = 23, COLORREF Color = RGB(0, 0, 0));

	static INT drawGradientSmall(HDC hdc, INT X = 0, INT Y = 0, INT W = 420, INT H = 40, INT BorderWidth = 2, COLORREF BorderColor = RGB(0, 0, 0));
	static INT drawGradientLarge(HDC hdc, INT X = 0, INT Y = 0, INT W = 420, INT H = 100, INT BorderWidth = 2, COLORREF BorderColor = RGB(0, 0, 0));

	#pragma endregion

	#pragma region Events

	static VOID onCreate(HWND hColorPicker, LPARAM lParam);
	static VOID onWindowPosChanging(HWND hColorPicker, LPARAM lParam);
	static VOID onMouseMove(HWND hColorPicker, WPARAM wParam, LPARAM lParam);
	static VOID onPaint(HWND hColorPicker);

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
