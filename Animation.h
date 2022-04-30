#ifndef _ANIMATION_
#define _ANIMATION_

#include <ciso646>
#include <Windows.h>
#include <string>

#define SEC 1000

#define AnimationTimer 1001

#define DarkerColor(SourceColor, DarkerTo) RGB(GetRValue(SourceColor) - DarkerTo, GetGValue(SourceColor) - DarkerTo, GetBValue(SourceColor) - DarkerTo) // returns DarkerColor

#define MAX_ANIMATION_CHAR_STRING 256
#define HInstance() GetModuleHandle(NULL)

class Animation {

private:

	#pragma region AnimationStaticMembers
	static HDC AnimationDC;
	static PAINTSTRUCT ps;

	static HDC MemoryDC;
	static HBITMAP Bitmap;

	static HFONT AnimationFont;
	static RECT Dimensions;
	#pragma endregion

	#pragma region Functions
	static VOID CreateAnimationFont();
	static VOID drawAnimationFrame(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH = 100, INT HEIGHT = 100, CONST CHAR Symbol = '+', USHORT Proportion = 4, COLORREF SymbolColor = RGB(255, 255, 255));
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
		USHORT Proportion;
		CHAR Symbol;
	}AStyle, *LPAStyle, *LPAnimationStyle;

	#pragma region InitAnimation
	static BOOL InitAnimation();
	#pragma endregion

};

#endif // _ANIMATION_
