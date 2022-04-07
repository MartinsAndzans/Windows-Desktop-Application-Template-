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

INT AnimationStars::Proportion = { 0 };

COLORREF AnimationStars::StarColor = { 0 };

#pragma endregion

// INIT ANIMATION STARS

#pragma region InitAnimationStars

BOOL AnimationStars::InitAnimationStars() {

	/// <summary>
	/// OPTIONAL FUNCTION
	/// CREATES CLASS "ANIMATION STARS"
	/// </summary>
	/// <returns>If Function Succeeded Returns True, but If not False</returns>
	
	StarFont = createStarFont();

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

HFONT AnimationStars::createStarFont() {

	HFONT Font = CreateFont(20, 0, 0, 0,
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

	return Font;

}

VOID AnimationStars::drawStars(HDC hdc, INT X, INT Y, INT W, INT H, COLORREF StarColor, const wchar_t StarSymbol[], INT Proportion) {

	/// <summary>
	/// This Function Draws Stars
	/// </summary>
	/// <param name="hdc">Device Context</param>
	/// <param name="X">X Coordinate</param>
	/// <param name="Y">Y Coordinate</param>
	/// <param name="W">Width</param>
	/// <param name="H">Height</param>
	/// <param name="StarColor">Star Color</param>
	/// <param name="StarSymbol">Star Symbol</param>
	/// <param name="Proportion">Proportion</param>

	if (W && H != 0) {

		SIZE size = { 0 };
		SYSTEMTIME st = { 0 };
		INT CELL = 0;
		INT XS = 0, XE = W, YS = 0, YE = H, XCELL = W / Proportion, YCELL = H / Proportion;

		COLORREF DefaultColor = GetTextColor(hdc);
		SetTextColor(hdc, StarColor);

		GetTextExtentPoint(hdc, StarSymbol, lstrlenW(StarSymbol), &size);

		GetSystemTime(&st);
		srand(st.wMilliseconds);

		for (int i = 0; i < Proportion * Proportion; i++) {

			INT STARX = rand() % XCELL + XS; // XS - (XS + XCELL)
			INT STARY = rand() % YCELL + YS; // YS - (YS + YCELL)

			TextOut(hdc, X + STARX - size.cx / 2, Y + STARY - size.cy / 2, StarSymbol, lstrlenW(StarSymbol));

			///////////////////////////
			//// -->               ////
			//// +---+---+---+---+ ////
			//// | 0 | 1 | 2 | 3 | ////
			//// +---+---+---+---+ ////
			///////////////////////////

			XS = XS + XCELL;
			CELL++;

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

			if (CELL == Proportion) {
				CELL = 0;
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

		LPASSTYLES parameters = (LPASSTYLES)window->lpCreateParams;

		ASSTYLES *Styles = new ASSTYLES;
		ZeroMemory(Styles, sizeof(ASSTYLES));

		if (parameters != NULL) {
			Styles->StarColor = parameters->StarColor;
			Styles->Proportion = parameters->Proportion;
		}

		SetWindowLongPtr(hAnimationStars, GWLP_USERDATA, (LONG_PTR)Styles);

		if (window->cx != 0 && window->cy != 0) {
			SetTimer(hAnimationStars, ASTIMER, SEC / 10, (TIMERPROC)NULL);
		}

	}
	else {

		DestroyWindow(hAnimationStars);

	}

}

VOID AnimationStars::onWindowPosChanging(HWND hAnimationStars, LPARAM lParam) {

	LPWINDOWPOS window = LPWINDOWPOS(lParam);

	if (window->cx != 0 && window->cy != 0) {
		KillTimer(hAnimationStars, ASTIMER);
		SetTimer(hAnimationStars, ASTIMER, SEC / 10, (TIMERPROC)NULL);
	}
	else {
		KillTimer(hAnimationStars, ASTIMER);
	}

}

VOID AnimationStars::onTimer(HWND hAnimationStars, WPARAM wParam, LPARAM lParam) {

	if (wParam == ASTIMER) {

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

	LPASSTYLES Styles = (LPASSTYLES)GetWindowLongPtr(hAnimationStars, GWLP_USERDATA);

	(Styles->StarColor != NULL) ? StarColor = Styles->StarColor : StarColor = RGB(255, 255, 255);
	(Styles->Proportion != NULL) ? Proportion = Styles->Proportion : Proportion = 6;
	
	///////////////////////////////////////////////
	//// +-----------------------------------+ ////
	//// |                                   | ////
	//// | [in] struct ASSTYLES - StarColor  | ////
	//// | [in] struct ASSTYLES - Proportion | ////
	//// |                                   | ////
	//// +-----------------------------------+ ////
	///////////////////////////////////////////////
	
	SetTextColor(MemoryDC, StarColor);

	WCHAR WindowTitle[MAX_ASTITLE_CHAR] = { 0 };
	GetWindowText(hAnimationStars, WindowTitle, ARRAYSIZE(WindowTitle));

	drawStars(MemoryDC, Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom, StarColor, L"*", Proportion);

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
		KillTimer(hAnimationStars, ASTIMER);
		LPASSTYLES Styles = (LPASSTYLES)GetWindowLongPtr(hAnimationStars, GWLP_USERDATA);
		delete[] Styles;
		return 0;
	}
	}

	return DefWindowProc(hAnimationStars, Msg, wParam, lParam);

}

#pragma endregion
