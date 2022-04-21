#include "Animation.h"

#pragma region InitAnimationStarsStaticMembers
HDC Animation::AnimationDC = { 0 };
PAINTSTRUCT Animation::ps = { 0 };

HDC Animation::MemoryDC = { 0 };
HBITMAP Animation::Bitmap = { 0 };

HFONT Animation::StarFont = { 0 };
RECT Animation::Dimensions = { 0 };
#pragma endregion

#pragma region InitAnimationStars
/// <summary>
/// Optional Function - Creates Class "ANIMATION"
/// </summary>
/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
BOOL Animation::InitAnimation() {
	
	CreateStarFont();

	WNDCLASSEX AnimationEx = { 0 };

	AnimationEx.cbClsExtra = 0;
	AnimationEx.cbWndExtra = 0;
	AnimationEx.cbSize = sizeof(WNDCLASSEX);
	AnimationEx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	AnimationEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	AnimationEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	AnimationEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	AnimationEx.hInstance = HInstance();
	AnimationEx.lpfnWndProc = AnimationProcedure;
	AnimationEx.lpszClassName = L"ANIMATION";
	AnimationEx.lpszMenuName = NULL;
	AnimationEx.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;

	if (!RegisterClassEx(&AnimationEx)) {
		return FALSE;
	}

    return TRUE;

}
#pragma endregion

#pragma region Funtions
VOID Animation::CreateStarFont() {

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

VOID Animation::drawStars(HDC hdc, RECT &Rectangle, COLORREF SymbolColor, CONST CHAR Symbol, UINT Proportion) {

	if (Rectangle.right != 0 && Rectangle.bottom != 0) {

		SIZE size = { 0 };
		SYSTEMTIME st = { 0 };
		INT XS = 0, YS = 0, XCELL = (Rectangle.right - Rectangle.left) / Proportion, YCELL = (Rectangle.bottom - Rectangle.top) / Proportion;

		COLORREF PreviousColor = GetTextColor(hdc);
		SetTextColor(hdc, SymbolColor);

		GetTextExtentPointA(hdc, &Symbol, 1, &size); // Size Symbol In Pixels

		GetSystemTime(&st); // Gets System Time
		srand(st.wMilliseconds); // Random Sead

		for (UINT I = 0; I < Proportion * Proportion; I++) {

			INT STARX = rand() % XCELL + XS; // XS - (XS + XCELL)
			INT STARY = rand() % YCELL + YS; // YS - (YS + YCELL)

			TextOutA(hdc, Rectangle.left + STARX - size.cx / 2, Rectangle.top + STARY - size.cy / 2, &Symbol, 1);

			///////////////////////////
			//// -->               ////
			//// +---+---+---+---+ ////
			//// | 0 | 1 | 2 | 3 | ////
			//// +---+---+---+---+ ////
			///////////////////////////

			XS = XS + XCELL;

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

			if (XS == XCELL * Proportion) {
				XS = 0;
				YS = YS + YCELL;
			}

		}

		SetTextColor(hdc, PreviousColor);

	}
	else {

		OutputDebugString(L"ERROR [AnimationStars::drawStars] - Width or Height Must be non Zero Value!\r\n");

	}

}
#pragma endregion

#pragma region Events
VOID Animation::onCreate(HWND hAnimation, LPARAM lParam) {

	LPCREATESTRUCT window = LPCREATESTRUCT(lParam);

	if (window->hwndParent != NULL && (window->style & WS_CHILD) != NULL &&
		(window->style & WS_THICKFRAME) == NULL && (window->style & WS_DLGFRAME) == NULL &&
		(window->style & WS_OVERLAPPED) == NULL && (window->style & WS_SYSMENU) == NULL) {

		AnimationStyle *Style = new AnimationStyle{ AnimationSymbolColor, 4, '+' }; // Default Initialization

		// Move Style Data To Heap Memory Structure | if "DropFilesStyle" Structure is Passed To lpParam
		if (window->lpCreateParams != NULL) {
			if (((LPAnimationStyle)window->lpCreateParams)->SymbolColor != NULL) Style->SymbolColor = ((LPAnimationStyle)window->lpCreateParams)->SymbolColor;
			if (((LPAnimationStyle)window->lpCreateParams)->Proportion != 0) Style->Proportion = ((LPAnimationStyle)window->lpCreateParams)->Proportion;
			if (((LPAnimationStyle)window->lpCreateParams)->Symbol > 32) Style->Symbol = ((LPAnimationStyle)window->lpCreateParams)->Symbol;
		}
		////

		SetWindowLongPtr(hAnimation, GWLP_USERDATA, (LONG_PTR)Style);

		if (window->cx != 0 && window->cy != 0) {
			SetTimer(hAnimation, AnimationTimer, SEC / 10, (TIMERPROC)NULL);
		}

	} else {

		OutputDebugString(L"ERROR [Animation Stars] - \"hwndParent\" Must Be Non Zero Value\r\n");
		DestroyWindow(hAnimation);

	}

}

VOID Animation::onWindowPosChanging(HWND hAnimation, LPARAM lParam) {

	LPWINDOWPOS window = LPWINDOWPOS(lParam);

	if (window->cx != 0 && window->cy != 0) {
		KillTimer(hAnimation, AnimationTimer);
		SetTimer(hAnimation, AnimationTimer, SEC / 10, (TIMERPROC)NULL);
	}
	else {
		KillTimer(hAnimation, AnimationTimer);
	}

}

VOID Animation::onTimer(HWND hAnimation, WPARAM wParam, LPARAM lParam) {

	if (wParam == AnimationTimer) {
		RedrawWindow(hAnimation, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
	}
}

VOID Animation::onPaint(HWND hAnimation) {

	GetClientRect(hAnimation, &Dimensions);

	AnimationDC = BeginPaint(hAnimation, &ps);

	MemoryDC = CreateCompatibleDC(AnimationDC);
	Bitmap = CreateCompatibleBitmap(AnimationDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	SetDCBrushColor(MemoryDC, AnimationBackgroundColor);
	FillRect(MemoryDC, &Dimensions, (HBRUSH)GetStockObject(DC_BRUSH));
	SelectObject(MemoryDC, StarFont);

	LPAnimationStyle Style = (LPAnimationStyle)GetWindowLongPtr(hAnimation, GWLP_USERDATA);
	
	/////////////////////////////////////////////////////
	//// +-----------------------------------------+ ////
	//// |                                         | ////
	//// | [in] struct AnimationStyle - StarColor  | ////
	//// | [in] struct AnimationStyle - Proportion | ////
	//// | [in] struct AnimationStyle - StarSymbol | ////
	//// |                                         | ////
	//// +-----------------------------------------+ ////
	/////////////////////////////////////////////////////

	WCHAR WindowTitle[MAX_ANIMATION_CHAR_STRING] = { 0 };
	GetWindowText(hAnimation, WindowTitle, ARRAYSIZE(WindowTitle));

	drawStars(MemoryDC, Dimensions, Style->SymbolColor, Style->Symbol, Style->Proportion);

	// Text Shadow
	SetTextColor(MemoryDC, DarkerColor(Style->SymbolColor, 0x20)); // 0x20 / 32
	TextOut(MemoryDC, Dimensions.left + 4, Dimensions.top + 4, WindowTitle, lstrlenW(WindowTitle));
	// Text
	SetTextColor(MemoryDC, Style->SymbolColor);
	TextOut(MemoryDC, Dimensions.left + 2, Dimensions.top + 2, WindowTitle, lstrlenW(WindowTitle));

	BitBlt(AnimationDC, 0, 0, Dimensions.right, Dimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	DeleteDC(MemoryDC);
	DeleteObject(Bitmap);

	EndPaint(hAnimation, &ps);

}
#pragma endregion

#pragma region AnimationStarsProcedure
LRESULT CALLBACK Animation::AnimationProcedure(HWND hAnimation, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{
		onCreate(hAnimation, lParam);
		return 0;
	}
	case WM_WINDOWPOSCHANGING:
	{
		onWindowPosChanging(hAnimation, lParam);
		return 0;
	}
	case WM_TIMER:
	{
		onTimer(hAnimation, wParam, lParam);
		return 0;
	}
	case WM_PAINT:
	{
		onPaint(hAnimation);
		return 0;
	}
	case WM_DESTROY:
	{
		KillTimer(hAnimation, AnimationTimer);
		LPAnimationStyle Style = (LPAnimationStyle)GetWindowLongPtr(hAnimation, GWLP_USERDATA);
		delete[] Style;
		SetWindowLongPtr(hAnimation, GWLP_USERDATA, NULL);
		return 0;
	}
	}

	return DefWindowProc(hAnimation, Msg, wParam, lParam);

}
#pragma endregion
