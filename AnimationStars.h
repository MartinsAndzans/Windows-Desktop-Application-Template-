#pragma once

#ifndef _ANIMATION_STARS_
#define _ANIMATION_STARS_

#include <Windows.h>
#include <string>

#define SEC 1000

#define AnimationTimer 1001

#define LighterColor(SourceColor, LighterTo) RGB(GetRValue(SourceColor) + LighterTo, GetGValue(SourceColor) + LighterTo, GetBValue(SourceColor) + LighterTo) // Returns LighterColor
#define DarkerColor(SourceColor, DarkerTo) RGB(GetRValue(SourceColor) - DarkerTo, GetGValue(SourceColor) - DarkerTo, GetBValue(SourceColor) - DarkerTo) // Returns DarkerColor

#define MAX_ASTITLE_CHAR 200
#define HInstance() GetModuleHandle(NULL)

#define AnimationStarsBackgroundColor RGB(0, 0, 0)
#define AnimationStarsStarColor RGB(255, 255, 255)

class AnimationStars {

private:

	#pragma region AnimationStarsStaticMembers
	static HDC AnimationStarsDC;
	static PAINTSTRUCT ps;

	static HDC MemoryDC;
	static HBITMAP Bitmap;

	static HFONT StarFont;
	static RECT Dimensions;
	#pragma endregion

	#pragma region Functions
	static VOID CreateStarFont();
	static VOID drawStars(HDC hdc, RECT &Rectangle, COLORREF StarColor, CONST CHAR StarSymbol[], UINT Proportion);
	#pragma endregion

	#pragma region Events
	static VOID onCreate(HWND hAnimationStars, LPARAM lParam);
	static VOID onWindowPosChanging(HWND hAnimationStars, LPARAM lParam);
	static VOID onPaint(HWND hAnimationStars);
	static VOID onTimer(HWND hAnimationStars, WPARAM wParam, LPARAM lParam);
	#pragma endregion

	#pragma region AnimationStarsProcedure
	static LRESULT CALLBACK AnimationStarsProcedure(HWND hAnimationStars, UINT Msg, WPARAM wParam, LPARAM lParam);
	#pragma endregion

public:

	typedef struct AnimationStyle {
		COLORREF StarColor;
		SHORT Proportion;
		CHAR StarSymbol;
	}AStyle, *LpAStyle, *LpAnimationStyle;

	#pragma region InitAnimationStars
	static BOOL InitAnimationStars();
	#pragma endregion

};

#endif
