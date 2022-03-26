#pragma once

#ifndef _ANIMATION_STARS_
#define _ANIMATION_STARS_

#include <Windows.h>
#include <string>

#define SEC 1000

#define ASTIMER 1001

#define MIN_RGB 0x00000000
#define MAX_RGB 0x00FFFFFF

#define GWL_USERDATA -21

#define MAX_ASTITLE_CHAR 200
#define HInstance() GetModuleHandle(NULL)

#define AnimationStarsBackgroundColor RGB(0, 0, 0)

class AnimationStars {

private:

	#pragma region AnimationStarsStaticMembers

	static HDC AnimationStarsDC;
	static PAINTSTRUCT ps;

	static HDC MemoryDC;
	static HBITMAP Bitmap;

	static HFONT StarFont;

	static RECT Dimensions;

	static INT Proportion;

	static COLORREF StarColor;

	#pragma endregion

	#pragma region Functions

	static VOID createStarFont(HFONT Font);

	static VOID drawStars(HDC hdc, INT X, INT Y, INT W, INT H, COLORREF StarColor, const wchar_t StarSymbol[] = L"*", INT proportion = 6);

	#pragma endregion

	#pragma region Events

	static VOID onCreate(HWND hAnimationStars, LPARAM lParam);
	static VOID onWindowPosChanged(HWND hAnimationStars, LPARAM lParam);
	static VOID onPaint(HWND hAnimationStars);
	static VOID onTimer(HWND hAnimationStars, WPARAM wParam, LPARAM lParam);
	static VOID onDestroy(HWND hAnimationStars);

	#pragma endregion

	#pragma region AnimationStarsProcedure

	static LRESULT AnimationStarsProcedure(HWND hAnimationStars, UINT msg, WPARAM wParam, LPARAM lParam);

	#pragma endregion

public:

	#pragma region InitAnimationStars

	static BOOL InitAnimationStars();

	#pragma endregion

};

#endif
