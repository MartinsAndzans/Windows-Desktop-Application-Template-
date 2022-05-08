#include "DropFiles.h"

#pragma region DropFilesStaticMembers
HFONT DropFiles::DropFilesFont = { 0 };
BOOL DropFiles::FileDroped = FALSE;
#pragma endregion

#pragma region RegisterDropFilesClass
/// <summary>
/// Optional Function - Registers Class "DropFiles"
/// </summary>
/// <returns>If Function Succeeded returns TRUE, but If not returns FALSE</returns>
BOOL DropFiles::RegisterDropFilesClass() {

	CreateDropFilesFont();

	WNDCLASSEX DropFilesEx = { 0 };

	DropFilesEx.cbClsExtra = 0;
	DropFilesEx.cbWndExtra = 0;
	DropFilesEx.cbSize = sizeof(WNDCLASSEX);
	DropFilesEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	DropFilesEx.hCursor = LoadCursor(NULL, IDC_CROSS);
	DropFilesEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	DropFilesEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	DropFilesEx.hInstance = GetModuleHandle(NULL);
	DropFilesEx.lpfnWndProc = DropFilesProcedure;
	DropFilesEx.lpszClassName = L"DropFiles";
	DropFilesEx.lpszMenuName = NULL;
	DropFilesEx.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;
	
	if (!RegisterClassEx(&DropFilesEx)) {
		return FALSE;
	}

	return TRUE;

}
#pragma endregion

#pragma region Functions
VOID DropFiles::CreateDropFilesFont() {

	DropFilesFont = CreateFont(20, 0, 0, 0,
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
VOID DropFiles::FillRectOpacity50(HDC hdc, CONST RECT &Rectangle, COLORREF Color) {

	BOOL DRAWPIXEL; // TRUE = |X| - FALSE = | |
	for (INT X = Rectangle.left; X <= Rectangle.right; X++) {
		(X % 2 == NULL) ? DRAWPIXEL = TRUE : DRAWPIXEL = FALSE; // 0, 2, 4, 5 = TRUE - 1, 3, 5, 7 = FALSE 
		for (INT Y = Rectangle.top; Y <= Rectangle.bottom; Y++) {
			if (DRAWPIXEL) {
				DRAWPIXEL = FALSE;
				SetPixel(hdc, X, Y, Color);
			} else {
				DRAWPIXEL = TRUE;
			}
		}
	}

}
VOID DropFiles::FillArrow(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH, INT HEIGHT, COLORREF Color) {

	CONST SHORT Proportion = 3;
	INT XCELL = WIDTH / Proportion, YCELL = HEIGHT / Proportion;

	SetDCPenColor(hdc, Color);
	SetDCBrushColor(hdc, Color);

	HPEN PreviousPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(DC_PEN));
	HBRUSH PreviousBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(DC_BRUSH));

	SetPolyFillMode(hdc, WINDING);

	POINT First = { COORD_X + XCELL, COORD_Y }; // --+----
	POINT Second = { COORD_X + XCELL * 2, COORD_Y }; // ----+--
	POINT Third = { COORD_X + XCELL, COORD_Y + YCELL * 2 }; // --+----
	POINT Fourth = { COORD_X + XCELL * 2, COORD_Y + YCELL * 2 }; // ----+--

	POINT Fifth = { COORD_X, COORD_Y + YCELL * 2 }; // +------
	POINT Sixth = { COORD_X + WIDTH, COORD_Y + YCELL * 2 }; //------+
	POINT Seventh = { COORD_X + WIDTH / 2, COORD_Y + HEIGHT }; // ---+---

	POINT ArrowVertices[] = { Third, First, Second, Fourth, Sixth, Seventh, Fifth };

	Polygon(hdc, ArrowVertices, ARRAYSIZE(ArrowVertices));

	SelectObject(hdc, PreviousPen);
	SelectObject(hdc, PreviousBrush);

}
#pragma endregion

#pragma region Events
VOID DropFiles::onCreate(HWND hDropFiles, LPARAM lParam) {

	LPCREATESTRUCT window = reinterpret_cast<LPCREATESTRUCT>(lParam);

	if (window->hwndParent != NULL and (window->style & WS_CHILD) != NULL) {

		DragAcceptFiles(hDropFiles, TRUE);

		CONST COLORREF BackgroundColor = RGB(255, 255, 255), ForegroundColor = RGB(0, 0, 0);

		DropFilesStyle *StylePtr = new DropFilesStyle{ BackgroundColor, ForegroundColor }; // Default Value Initialization

		// Move Style Data To Heap Memory Structure | If "DropFilesStyle" Structure is Passed To lpParam
		if (window->lpCreateParams != nullptr) {
			StylePtr ->BackgroundColor = static_cast<LPDropFilesStyle>(window->lpCreateParams)->BackgroundColor;
			StylePtr->ForegroundColor = static_cast<LPDropFilesStyle>(window->lpCreateParams)->ForegroundColor;
		}
		////

		SetWindowLongPtr(hDropFiles, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(StylePtr)); // Save Pointer To Window User Data

	} else {
		OutputDebugStringA("\'ERROR \'DropFiles\' - \"hwndParent\" Must Be Non Zero Value\r\n\'");
		DestroyWindow(hDropFiles);
	}

}

VOID DropFiles::onPaint(HWND hDropFiles) {

	PAINTSTRUCT ps = { 0 };
	RECT Dimensions = { 0 };

	GetClientRect(hDropFiles, &Dimensions);

	HDC DropFilesDC = BeginPaint(hDropFiles, &ps);

	HDC MemoryDC = CreateCompatibleDC(DropFilesDC);
	HBITMAP Bitmap = CreateCompatibleBitmap(DropFilesDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	
	LPDropFilesStyle StylePtr = reinterpret_cast<LPDropFilesStyle>(GetWindowLongPtr(hDropFiles, GWLP_USERDATA));

	////////////////////////////////////////////////////////////
	//// +------------------------------------------------+ ////
	//// |                                                | ////
	//// | [in] struct DropFilesStyle - BackgroundColor   | ////
	//// | [in] struct DropFilesStyle - ForegroundColor   | ////
	//// |                                                | ////
	//// +------------------------------------------------+ ////
	////////////////////////////////////////////////////////////

	SetDCBrushColor(MemoryDC, StylePtr->BackgroundColor);
	FillRect(MemoryDC, &Dimensions, static_cast<HBRUSH>(GetStockObject(DC_BRUSH)));
	SelectObject(MemoryDC, DropFilesFont);

	// Text To User
	SIZE size = { 0 };
	CONST USHORT MAX_CHAR_STRING = 256;
	WCHAR WindowTitle[MAX_CHAR_STRING] = { 0 };
	INT TextLength = GetWindowText(hDropFiles, WindowTitle, ARRAYSIZE(WindowTitle));
	SetTextColor(MemoryDC, StylePtr->ForegroundColor);
	GetTextExtentPoint(MemoryDC, WindowTitle, TextLength, &size);
	TextOut(MemoryDC, Dimensions.right / 2 - size.cx / 2, Dimensions.bottom / 2 - size.cy / 2, WindowTitle, TextLength);
	////

	if (FileDroped) {
		CONST USHORT ArrowPadding = 20;
		INT ArrowWidth = Dimensions.bottom / 2, ArrowHeight = Dimensions.bottom - ArrowPadding * 2;
		FillRectOpacity50(MemoryDC, Dimensions, StylePtr->ForegroundColor);
		FillArrow(MemoryDC, Dimensions.right / 2 - ArrowWidth / 2, Dimensions.bottom / 2 - ArrowHeight / 2, ArrowWidth, ArrowHeight, StylePtr->ForegroundColor);
	}

	BitBlt(DropFilesDC, 0, 0, Dimensions.right, Dimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	DeleteDC(MemoryDC);
	DeleteObject(Bitmap);

	EndPaint(hDropFiles, &ps);

	if (FileDroped) {
		Sleep(200); // 200 Milliseconds Delay
		FileDroped = FALSE;
		RedrawWindow(hDropFiles, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
	}

}

VOID DropFiles::onDropFiles(HWND hDropFiles, WPARAM wParam) {

	///////////////////////////////////////////////////////
	//// +-------------------------------------------+ ////
	//// |                                           | ////
	//// | [out] WPARAM - LOWORD(ID) | HIWORD(HWND)  | ////
	//// | [out] LPARAM - HDROP                      | ////
	//// |                                           | ////
	//// +-------------------------------------------+ ////
	///////////////////////////////////////////////////////

	FileDroped = TRUE;
	PostMessage(GetParent(hDropFiles), WM_COMMAND, MAKEWPARAM(GetWindowLong(hDropFiles, GWL_ID), hDropFiles), wParam);
	RedrawWindow(hDropFiles, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);

}
#pragma endregion

#pragma region DropFilesProcedure
LRESULT CALLBACK DropFiles::DropFilesProcedure(HWND hDropFiles, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{
		onCreate(hDropFiles, lParam);
		return 0;
	}
	case WM_PAINT:
	{
		onPaint(hDropFiles);
		return 0;
	}
	case WM_DROPFILES:
	{
		onDropFiles(hDropFiles, wParam);
		return 0;
	}
	case WM_DESTROY:
	{
		LPDropFilesStyle StylePtr = reinterpret_cast<LPDropFilesStyle>(GetWindowLongPtr(hDropFiles, GWLP_USERDATA));
		delete[] StylePtr;
		return 0;
	}
	}

	return DefWindowProc(hDropFiles, Msg, wParam, lParam);

}
#pragma endregion
