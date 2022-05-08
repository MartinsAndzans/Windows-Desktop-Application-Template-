#ifndef _COLOR_PICKER_
#define _COLOR_PICKER_

#include <ciso646>
#include <Windows.h>

class ColorPicker {

private:

	#pragma region ColorPickerStaticMembers
	static CONST SIZE DimensionsSmall;
	static CONST SIZE DimensionsLarge;
	#pragma endregion

	#pragma region Functions
	static VOID drawCross(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH, INT HEIGHT);
	static VOID drawSmallGradient(HDC hdc, INT COORD_X, INT COORD_Y);
	static VOID drawLargeGradient(HDC hdc, INT COORD_X, INT COORD_Y);
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

	static CONST USHORT SHOW = 1010; // Small Color Picker -/Width 420 - Height 40/- : Large Color Picker -/Width 420 - Height 100/-

	#pragma region RegisterColorPickerClass
	static BOOL RegisterColorPickerClass();
	#pragma endregion

};

#endif // _COLOR_PICKER_
