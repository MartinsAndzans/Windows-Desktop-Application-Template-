#include "Animation.h"

#pragma region InitAnimationStarsStaticMembers
HFONT Animation::AnimationFont = { 0 };
#pragma endregion

#pragma region RegisterAnimationClass
/// <summary>
/// Optional Function - Registers Class "Animation"
/// </summary>
/// <returns>If Function Succeeded returns TRUE, but If not returns FALSE</returns>
BOOL Animation::RegisterAnimationClass() {
	
	CreateAnimationFont();

	WNDCLASSEX AnimationEx = { 0 };

	AnimationEx.cbClsExtra = 0;
	AnimationEx.cbWndExtra = 0;
	AnimationEx.cbSize = sizeof(WNDCLASSEX);
	AnimationEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	AnimationEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	AnimationEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	AnimationEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	AnimationEx.hInstance = GetModuleHandle(NULL);
	AnimationEx.lpfnWndProc = AnimationProcedure;
	AnimationEx.lpszClassName = L"Animation";
	AnimationEx.lpszMenuName = NULL;
	AnimationEx.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;

	if (!RegisterClassEx(&AnimationEx)) {
		return FALSE;
	}

    return TRUE;

}
#pragma endregion

#pragma region Funtions
VOID Animation::CreateAnimationFont() {

	AnimationFont = CreateFont(20, 0, 0, 0,
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
VOID Animation::DrawAnimationFrame(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH, INT HEIGHT, CONST CHAR Symbol, USHORT Proportion, COLORREF SymbolColor) {

	if (WIDTH > 0 and HEIGHT > 0) {

		SIZE size = { 0 };
		SYSTEMTIME st = { 0 };
		INT XCELL = WIDTH / Proportion, YCELL = HEIGHT / Proportion;

		COLORREF PreviousColor = GetTextColor(hdc);
		SetTextColor(hdc, SymbolColor);

		GetTextExtentPoint32A(hdc, &Symbol, 1, &size); // Symbol Size In Pixels

		GetSystemTime(&st); // Gets System Time
		srand(st.wMilliseconds); // Random Sead

		for (UINT X = 0; X < Proportion; X++) {
			for (UINT Y = 0; Y < Proportion; Y++) {

				INT SYMBOL_X = rand() % XCELL; // Random Number From 0 To (XCELL - 1)
				INT SYMBOL_Y = rand() % YCELL; // Random Number From 0 To (YCELL - 1)

				TextOutA(hdc, COORD_X + (SYMBOL_X + XCELL * X) - size.cx / 2, COORD_Y + (SYMBOL_Y + YCELL * Y) - size.cy / 2, &Symbol, 1);

			}
		}

		SetTextColor(hdc, PreviousColor);

	} else {
		OutputDebugString(L"\'ERROR \'Animation::drawAnimationFrame\' - Width or Height Must be non Zero Value!\'\r\n");
	}

}
#pragma endregion

#pragma region Events
VOID Animation::onCreate(HWND hAnimation, LPARAM lParam) {

	LPCREATESTRUCT window = reinterpret_cast<LPCREATESTRUCT>(lParam);

	if (window->hwndParent != NULL and (window->style & WS_CHILD) != NULL) {

		CONST COLORREF SymbolColor = RGB(255, 255, 255);

		AnimationStyle *StylePtr = new AnimationStyle{ SymbolColor, 4, '+'}; // Default Value Initilization

		// Move Style Data To Heap Memory Structure / If "AnimationStyle" Structure is Passed To lpParam
		if (window->lpCreateParams != nullptr) {
			StylePtr->SymbolColor = static_cast<LPAnimationStyle>(window->lpCreateParams)->SymbolColor;
			StylePtr->Proportion = static_cast<LPAnimationStyle>(window->lpCreateParams)->Proportion;
			StylePtr->Symbol = static_cast<LPAnimationStyle>(window->lpCreateParams)->Symbol;
		}

		SetWindowLongPtr(hAnimation, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(StylePtr)); // Save Pointer To Window User Data

		if (window->cx != 0 and window->cy != 0) {
			SetTimer(hAnimation, Update::AnimationTimer, Update::Second / 10, nullptr);
		}

	} else {
		OutputDebugStringA("\'ERROR \'Animation\' - \"hwndParent\" Must Be Non Zero Value\r\n\'");
		DestroyWindow(hAnimation);
	}

}

VOID Animation::onWindowPosChanging(HWND hAnimation, LPARAM lParam) {

	LPWINDOWPOS window = reinterpret_cast<LPWINDOWPOS>(lParam);

	if (window->cx != 0 and window->cy != 0) {
		SetTimer(hAnimation, Update::AnimationTimer, Update::Second / 10, nullptr);
	} else {
		KillTimer(hAnimation, Update::AnimationTimer);
	}

}

VOID Animation::onTimer(HWND hAnimation, WPARAM wParam, LPARAM lParam) {
	if (wParam == Update::AnimationTimer)
		RedrawWindow(hAnimation, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
}

VOID Animation::onPaint(HWND hAnimation) {

	PAINTSTRUCT ps = { 0 };
	RECT Dimensions = { 0 };

	GetClientRect(hAnimation, &Dimensions);

	HDC AnimationDC = BeginPaint(hAnimation, &ps);

	HDC MemoryDC = CreateCompatibleDC(AnimationDC);
	HBITMAP Bitmap = CreateCompatibleBitmap(AnimationDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	FillRect(MemoryDC, &Dimensions, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
	SelectObject(MemoryDC, AnimationFont);

	LPAnimationStyle StylePtr = reinterpret_cast<LPAnimationStyle>(GetWindowLongPtr(hAnimation, GWLP_USERDATA));
	
	//////////////////////////////////////////////////////
	//// +------------------------------------------+ ////
	//// |                                          | ////
	//// | [in] struct AnimationStyle - SymbolColor | ////
	//// | [in] struct AnimationStyle - Proportion  | ////
	//// | [in] struct AnimationStyle - Symbol      | ////
	//// |                                          | ////
	//// +------------------------------------------+ ////
	//////////////////////////////////////////////////////

	CONST USHORT MAX_CHAR_STRING = 256;
	WCHAR WindowTitle[MAX_CHAR_STRING] = { 0 };
	INT TextLength = GetWindowText(hAnimation, WindowTitle, ARRAYSIZE(WindowTitle));

	DrawAnimationFrame(MemoryDC, Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom, StylePtr->Symbol, StylePtr->Proportion, StylePtr->SymbolColor);
	
	// Text Shadow
	SetTextColor(MemoryDC, DarkerColor(StylePtr->SymbolColor, 0x20)); // 0x20 / 32
	TextOut(MemoryDC, Dimensions.left + 4, Dimensions.top + 4, WindowTitle, TextLength);
	// Text
	SetTextColor(MemoryDC, StylePtr->SymbolColor);
	TextOut(MemoryDC, Dimensions.left + 2, Dimensions.top + 2, WindowTitle, TextLength);

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
		LPAnimationStyle StylePtr = reinterpret_cast<LPAnimationStyle>(GetWindowLongPtr(hAnimation, GWLP_USERDATA));
		delete[] StylePtr;
		return 0;
	}
	}

	return DefWindowProc(hAnimation, Msg, wParam, lParam);

}
#pragma endregion
