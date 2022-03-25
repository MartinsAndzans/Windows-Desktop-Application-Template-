#include "AnimationStars.h"

// INIT ANIMATION STARS STATIC MEMBERS

#pragma region InitAnimationStarsStaticMembers

HDC AnimationStars::AnimationStarsDC;
PAINTSTRUCT AnimationStars::ps;

HDC AnimationStars::MemoryDC;
HBITMAP AnimationStars::Bitmap;

HFONT AnimationStars::StarFont;

RECT AnimationStars::Dimensions;

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

	WNDCLASSEX AnimationStarsEx = { 0 };

	AnimationStarsEx.cbClsExtra = 0;
	AnimationStarsEx.cbWndExtra = 0;
	AnimationStarsEx.cbSize = sizeof(WNDCLASSEX);
	AnimationStarsEx.hbrBackground = CreateSolidBrush(AnimationStarsBackgroundColor);
	AnimationStarsEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	AnimationStarsEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	AnimationStarsEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	AnimationStarsEx.hInstance = HInstance();
	AnimationStarsEx.lpfnWndProc = AnimationStarsProcedure;
	AnimationStarsEx.lpszClassName = L"ANIMATION STARS";
	AnimationStarsEx.lpszMenuName = NULL;
	AnimationStarsEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	if (!RegisterClassEx(&AnimationStarsEx)) {
		return FALSE;
	}

    return TRUE;

}

#pragma endregion

// FUNCTIONS

#pragma region Funtions

VOID AnimationStars::createStarFont(HFONT Font) {

	Font = CreateFont(32, 0, 0, 0,
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

VOID AnimationStars::drawStars(HDC hdc, INT X, INT Y, INT W, INT H, COLORREF StarColor, std::wstring StarSymbol, INT proportion) {

	SIZE size = { 0 };
	SYSTEMTIME st = { 0 };
	INT CELL = 0;
	INT XS = 0, XE = W, YS = 0, YE = H, XC = W / proportion, YC = H / proportion;

	COLORREF DefaultColor = GetTextColor(hdc);
	SetTextColor(hdc, StarColor);

	GetTextExtentPoint(hdc, StarSymbol.c_str(), (int)StarSymbol.length(), &size);

	GetSystemTime(&st);
	srand(st.wMilliseconds);

	for (int i = 0; i < proportion * proportion; i++) {

		INT SX = rand() % XC + XS; // XS - (XS + XC)
		INT SY = rand() % YC + YS; // YS - (YS + YC)

		TextOut(hdc, X + SX - size.cx / 2, Y + SY - size.cy / 2, StarSymbol.c_str(), (int)StarSymbol.length());

		XS = XS + XC;
		CELL++;

		if (CELL == proportion) {
			CELL = 0;
			XS = 0;
			YS = YS + YC;
		}

	}

	SetTextColor(hdc, DefaultColor);

}

#pragma endregion

// EVENTS

#pragma region Events

void AnimationStars::onCreate(HWND hAnimationStars, LPARAM lParam) {

	LPCREATESTRUCT window = LPCREATESTRUCT(lParam);

	if (window->hwndParent == NULL || (window->style & WS_CHILD) == NULL ||
		(window->style & WS_POPUP) != NULL || (window->style & WS_DLGFRAME) != NULL ||
		(window->style & WS_OVERLAPPED) != NULL || (window->style & WS_SYSMENU) != NULL ||
		(window->style & WS_THICKFRAME) != NULL) {
		DestroyWindow(hAnimationStars);
	}

	createStarFont(StarFont);

	StarColor = (COLORREF)window->lpCreateParams;

	Dimensions.right = window->cx;
	Dimensions.bottom = window->cy;

	if (window->cx != 0 || window->cy != 0) {
		SetTimer(hAnimationStars, ASTIMER, SEC / 10, (TIMERPROC)NULL);
	}

}

void AnimationStars::onWindowPosChanged(HWND hAnimationStars, LPARAM lParam) {

	LPWINDOWPOS window = LPWINDOWPOS(lParam);

	Dimensions.right = window->cx;
	Dimensions.bottom = window->cy;

	if (window->cx != 0 || window->cy != 0) {
		KillTimer(hAnimationStars, ASTIMER);
		SetTimer(hAnimationStars, ASTIMER, SEC / 10, (TIMERPROC)NULL);
	}
	else {
		KillTimer(hAnimationStars, ASTIMER);
	}

}

void AnimationStars::onTimer(HWND hAnimationStars, WPARAM wParam, LPARAM lParam) {

	if (wParam == ASTIMER) {

		//PlaySound(L"F:\\Multvide\\Lorija Vuda && Hardijs Madzulis - (04) Paliec.wma", NULL, SND_ASYNC | SND_FILENAME);

		RedrawWindow(hAnimationStars, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
	}

}

void AnimationStars::onPaint(HWND hAnimationStars) {

	AnimationStarsDC = BeginPaint(hAnimationStars, &ps);

	MemoryDC = CreateCompatibleDC(AnimationStarsDC);
	Bitmap = CreateCompatibleBitmap(AnimationStarsDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	FillRect(MemoryDC, &Dimensions, CreateSolidBrush(AnimationStarsBackgroundColor));

	SelectObject(MemoryDC, StarFont);

	if (StarColor < MIN_RGB || StarColor > MAX_RGB || StarColor == NULL) {
		StarColor = RGB(255, 255, 255);
	}
	
	SetTextColor(MemoryDC, StarColor);

	WCHAR WindowTitle[MAX_ASTITLE_CHAR] = { 0 };
	GetWindowText(hAnimationStars, WindowTitle, ARRAYSIZE(WindowTitle));
	std::wstring WWindowTitle = WindowTitle;

	drawStars(MemoryDC, Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom, StarColor);

	TextOut(MemoryDC, Dimensions.left + 10, Dimensions.top + 10, WWindowTitle.c_str(), (int)WWindowTitle.length());

	BitBlt(AnimationStarsDC, 0, 0, Dimensions.right, Dimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	EndPaint(hAnimationStars, &ps);

}

void AnimationStars::onDestroy(HWND hAnimationStars) {

	KillTimer(hAnimationStars, ASTIMER);

}

#pragma endregion

// ANIMATION STARS PROCEDURE

#pragma region AnimationStarsProcedure

LRESULT AnimationStars::AnimationStarsProcedure(HWND hAnimationStars, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_CREATE:
	{
		onCreate(hAnimationStars, lParam);
		return 0;
	}
	case WM_WINDOWPOSCHANGED:
	{
		onWindowPosChanged(hAnimationStars, lParam);
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
		onDestroy(hAnimationStars);
		return 0;
	}
	}
	return DefWindowProc(hAnimationStars, msg, wParam, lParam);

}

#pragma endregion
