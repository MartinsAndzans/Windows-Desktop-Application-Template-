#pragma once

#ifndef _ANIMATION_
#define _ANIMATION_

#include <Windows.h>

#define SEC 1000

#define AnimationTimer 1001

#define LighterColor(SourceColor, LighterTo) RGB(GetRValue(SourceColor) + LighterTo, GetGValue(SourceColor) + LighterTo, GetBValue(SourceColor) + LighterTo) // Returns LighterColor
#define DarkerColor(SourceColor, DarkerTo) RGB(GetRValue(SourceColor) - DarkerTo, GetGValue(SourceColor) - DarkerTo, GetBValue(SourceColor) - DarkerTo) // Returns DarkerColor

#define MAX_ANIMATION_CHAR_STRING 256
#define HInstance() GetModuleHandle(NULL)

#define AnimationBackgroundColor RGB(0, 0, 0)
#define AnimationSymbolColor RGB(255, 255, 255)

class Animation {

private:

	#pragma region AnimationStaticMembers
	static HDC AnimationDC;
	static PAINTSTRUCT ps;

	static HDC MemoryDC;
	static HBITMAP Bitmap;

	static HFONT StarFont;
	static RECT Dimensions;
	#pragma endregion

	#pragma region Functions
	static VOID CreateStarFont();
	static VOID drawStars(HDC hdc, RECT &Rectangle, COLORREF StarColor, CONST CHAR StarSymbol, UINT Proportion);
	#pragma endregion

	#pragma region Events
	static VOID onCreate(HWND hAnimation, LPARAM lParam);
	static VOID onWindowPosChanging(HWND hAnimation, LPARAM lParam);
	static VOID onPaint(HWND hAnimation);
	static VOID onTimer(HWND hAnimation, WPARAM wParam, LPARAM lParam);
	#pragma endregion

	#pragma region AnimationProcedure
	static LRESULT CALLBACK AnimationProcedure(HWND hAnimation, UINT Msg, WPARAM wParam, LPARAM lParam);
	#pragma endregion

public:

	typedef struct AnimationStyle {
		COLORREF SymbolColor;
		SHORT Proportion;
		CHAR Symbol;
	}AStyle, *LPAStyle, *LPAnimationStyle;

	#pragma region InitAnimation
	static BOOL InitAnimation();
	#pragma endregion

};

#endif
