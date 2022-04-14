#include "AnimationStars.h"

// INIT ANIMATION STARS STATIC MEMBERS

#pragma region InitAnimationStarsStaticMembers

HDC AnimationStars::AnimationStarsDC = { 0 };
PAINTSTRUCT AnimationStars::ps = { 0 };

HDC AnimationStars::MemoryDC = { 0 };
HBITMAP AnimationStars::Bitmap = { 0 };

HBRUSH AnimationStars::AnimationStarsBackgroundBrush = CreateSolidBrush(AnimationStarsBackgroundColor);

HFONT AnimationStars::StarFont = { 0 };

RECT AnimationStars::Dimensions = { 0 };

COLORREF AnimationStars::StarColor = { 0 };
INT AnimationStars::Proportion = { 0 };
CHAR AnimationStars::StarSymbol[2] = "+";

#pragma endregion

// INIT ANIMATION STARS

#pragma region InitAnimationStars

/// <summary>
/// Optional Function - Creates Class "ANIMATION STARS"
/// </summary>
/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
BOOL AnimationStars::InitAnimationStars() {
	
	CreateStarFont();

	WNDCLASSEX AnimationStarsEx = { 0 };

	AnimationStarsEx.cbClsExtra = 0;
	AnimationStarsEx.cbWndExtra = 0;
	AnimationStarsEx.cbSize = sizeof(WNDCLASSEX);
	AnimationStarsEx.hbrBackground = AnimationStarsBackgroundBrush;
	AnimationStarsEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	AnimationStarsEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	AnimationStarsEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	AnimationStarsEx.hInstance = HInstance();
	AnimationStarsEx.lpfnWndProc = AnimationStarsProcedure;
	AnimationStarsEx.lpszClassName = L"ANIMATION STARS";
	AnimationStarsEx.lpszMenuName = NULL;
	AnimationStarsEx.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;

	if (!RegisterClassEx(&AnimationStarsEx)) {
		return FALSE;
	}

    return TRUE;

}

#pragma endregion

// FUNCTIONS

#pragma region Funtions

VOID AnimationStars::CreateStarFont() {

	StarFont = CreateFont(20, 0, 0, 0,
		FW_BOLD,
		false,
		false,
		false,
		ANSI_FIXED_FONT,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		VARIABLE_PITCH,
		L"Segoe UI");

}

VOID AnimationStars::drawStars(HDC hdc, INT X, INT Y, INT W, INT H, COLORREF StarColor, CONST CHAR StarSymbol[], UINT Proportion) {

	if (Proportion == 0) Proportion++;

	if (W && H != 0) {

		SIZE size = { 0 };
		SYSTEMTIME st = { 0 };
		INT CURRENTCELL = 0, XS = 0, XE = W, YS = 0, YE = H, XCELL = W / Proportion, YCELL = H / Proportion;

		COLORREF DefaultColor = GetTextColor(hdc);
		SetTextColor(hdc, StarColor);

		GetTextExtentPointA(hdc, StarSymbol, (int)strlen(StarSymbol), &size);

		GetSystemTime(&st);
		srand(st.wMilliseconds);

		for (UINT i = 0; i < Proportion * Proportion; i++) {

			INT STARX = rand() % XCELL + XS; // XS - (XS + XCELL)
			INT STARY = rand() % YCELL + YS; // YS - (YS + YCELL)

			TextOutA(hdc, X + STARX - size.cx / 2, Y + STARY - size.cy / 2, StarSymbol, (int)strlen(StarSymbol));

			///////////////////////////
			//// -->               ////
			//// +---+---+---+---+ ////
			//// | 0 | 1 | 2 | 3 | ////
			//// +---+---+---+---+ ////
			///////////////////////////

			XS = XS + XCELL;
			CURRENTCELL++;

			/////////////////
			//// +---+ | ////
			//// | 0 | | ////
			//// +---+ V ////
			//// | 1 |   ////
			//// +---+   ////
			//// | 2 |   ////
			//// +---+   ////
			//// | 3 |   ////
			//// +---+   ////
			/////////////////

			if (CURRENTCELL == Proportion) {
				CURRENTCELL = 0;
				XS = 0;
				YS = YS + YCELL;
			}

		}

		SetTextColor(hdc, DefaultColor);

	}
	else {

		OutputDebugString(L"ERROR [AnimationStars::drawStars] - Width or Height Must be non Zero Value!\r\n");

	}

}

#pragma endregion

// EVENTS

#pragma region Events

VOID AnimationStars::onCreate(HWND hAnimationStars, LPARAM lParam) {

	LPCREATESTRUCT window = LPCREATESTRUCT(lParam);

	if (window->hwndParent != NULL && (window->style & WS_CHILD) != NULL &&
		(window->style & WS_POPUP) == NULL && (window->style & WS_DLGFRAME) == NULL &&
		(window->style & WS_OVERLAPPED) == NULL && (window->style & WS_SYSMENU) == NULL &&
		(window->style & WS_THICKFRAME) == NULL) {

		LpAnimationStyle Parameters = (LpAnimationStyle)window->lpCreateParams;

		AnimationStyle *Style = new AnimationStyle;
		ZeroMemory(Style, sizeof(AnimationStyle));

		if (Parameters != NULL) {
			Style->StarColor = Parameters->StarColor;
			Style->Proportion = Parameters->Proportion;
			strcpy_s(Style->StarSymbol, Parameters->StarSymbol);
		}

		SetWindowLongPtr(hAnimationStars, GWLP_USERDATA, (LONG_PTR)Style);

		if (window->cx != 0 && window->cy != 0) {
			SetTimer(hAnimationStars, AnimationTimer, SEC / 10, (TIMERPROC)NULL);
		}

	}
	else {

		DestroyWindow(hAnimationStars);

	}

}

VOID AnimationStars::onWindowPosChanging(HWND hAnimationStars, LPARAM lParam) {

	LPWINDOWPOS window = LPWINDOWPOS(lParam);

	if (window->cx != 0 && window->cy != 0) {
		KillTimer(hAnimationStars, AnimationTimer);
		SetTimer(hAnimationStars, AnimationTimer, SEC / 10, (TIMERPROC)NULL);
	}
	else {
		KillTimer(hAnimationStars, AnimationTimer);
	}

}

VOID AnimationStars::onTimer(HWND hAnimationStars, WPARAM wParam, LPARAM lParam) {

	if (wParam == AnimationTimer) {
		GetClientRect(hAnimationStars, &Dimensions);
		RedrawWindow(hAnimationStars, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
	}

}

VOID AnimationStars::onPaint(HWND hAnimationStars) {

	GetClientRect(hAnimationStars, &Dimensions);

	AnimationStarsDC = BeginPaint(hAnimationStars, &ps);

	MemoryDC = CreateCompatibleDC(AnimationStarsDC);
	Bitmap = CreateCompatibleBitmap(AnimationStarsDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	FillRect(MemoryDC, &Dimensions, AnimationStarsBackgroundBrush);

	SelectObject(MemoryDC, StarFont);

	LpAnimationStyle Style = (LpAnimationStyle)GetWindowLongPtr(hAnimationStars, GWLP_USERDATA);

	(Style->StarColor != NULL) ? StarColor = Style->StarColor : StarColor = RGB(255, 255, 255); // DEFAULT
	(Style->Proportion != NULL) ? Proportion = Style->Proportion : Proportion = 6; // DEFAULT
	(Style->StarSymbol[0] != '\0') ? strcpy_s(StarSymbol, Style->StarSymbol) : strcpy_s(StarSymbol, "+"); // DEFAULT
	
	/////////////////////////////////////////////////////
	//// +-----------------------------------------+ ////
	//// |                                         | ////
	//// | [in] struct AnimationStyle - StarColor  | ////
	//// | [in] struct AnimationStyle - Proportion | ////
	//// | [in] struct AnimationStyle - StarSymbol | ////
	//// |                                         | ////
	//// +-----------------------------------------+ ////
	/////////////////////////////////////////////////////
	
	SetTextColor(MemoryDC, StarColor);

	WCHAR WindowTitle[MAX_ASTITLE_CHAR] = { 0 };
	GetWindowText(hAnimationStars, WindowTitle, ARRAYSIZE(WindowTitle));

	drawStars(MemoryDC, Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom, StarColor, StarSymbol, Proportion);

	TextOut(MemoryDC, Dimensions.left + 2, Dimensions.top + 2, WindowTitle, lstrlenW(WindowTitle));

	BitBlt(AnimationStarsDC, 0, 0, Dimensions.right, Dimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	DeleteDC(MemoryDC);
	DeleteObject(Bitmap);

	EndPaint(hAnimationStars, &ps);

}

#pragma endregion

// ANIMATION STARS PROCEDURE

#pragma region AnimationStarsProcedure

LRESULT CALLBACK AnimationStars::AnimationStarsProcedure(HWND hAnimationStars, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{
		onCreate(hAnimationStars, lParam);
		return 0;
	}
	case WM_WINDOWPOSCHANGING:
	{
		onWindowPosChanging(hAnimationStars, lParam);
		return 0;
	}
	case WM_TIMER:
	{
		onTimer(hAnimationStars, wParam, lParam);
		return 0;
	}
	case WM_PAINT:
	{
		onPaint(hAnimationStars);
		return 0;
	}
	case WM_DESTROY:
	{
		KillTimer(hAnimationStars, AnimationTimer);
		LpAnimationStyle Style = (LpAnimationStyle)GetWindowLongPtr(hAnimationStars, GWLP_USERDATA);
		delete[] Style;
		return 0;
	}
	}

	return DefWindowProc(hAnimationStars, Msg, wParam, lParam);

}

#pragma endregion
