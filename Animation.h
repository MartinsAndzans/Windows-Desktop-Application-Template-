#ifndef _ANIMATION_
#define _ANIMATION_

#include <ciso646>
#include <Windows.h>
#include <string>

#define DarkerColor(SourceColor, DarkerTo) RGB(GetRValue(SourceColor) - DarkerTo, GetGValue(SourceColor) - DarkerTo, GetBValue(SourceColor) - DarkerTo) // returns DarkerColor

class Animation {

private:

	enum Update {
		Second = 1000,
		AnimationTimer = 1010
	};

	#pragma region AnimationStaticMembers
	static HFONT AnimationFont;
	#pragma endregion

	#pragma region Functions
	static VOID CreateAnimationFont();
	static VOID DrawAnimationFrame(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH, INT HEIGHT, CONST CHAR Symbol, USHORT Proportion, COLORREF SymbolColor);
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

	#pragma region RegisterAnimationClass
	static BOOL RegisterAnimationClass();
	#pragma endregion

};

#endif // _ANIMATION_
