#ifndef _COLOR_PICKER_
#define _COLOR_PICKER_

#include <ciso646>
#include <Windows.h>

#define GWL_USERDATA -21
#define CP_SHOW 1001 // SMALL = 420 - 40 | LARGE = 420 - 100

#define MAX_COLOR_PICKER_CHAR_STRING 20

class ColorPicker {

private:

	#pragma region ColorPickerStaticMembers
	static CONST SIZE DimensionsSmall;
	static CONST SIZE DimensionsLarge;
	#pragma endregion

	#pragma region Functions
	static VOID drawCross(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH, INT HEIGHT, COLORREF CrossColor);
	static SHORT drawSmallGradient(HDC hdc, INT COORD_X, INT COORD_Y, COLORREF BorderColor);
	static SHORT drawLargeGradient(HDC hdc, INT COORD_X, INT COORD_Y, COLORREF BorderColor);
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

#endif // _COLOR_PICKER_
