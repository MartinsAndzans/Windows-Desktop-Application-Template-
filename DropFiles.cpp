#include "DropFiles.h"

#pragma region CalculatorStaticMembers
HDC DropFiles::DropFilesDC = { 0 };
PAINTSTRUCT DropFiles::ps = { 0 };

HDC DropFiles::MemoryDC = { 0 };
HBITMAP DropFiles::Bitmap = { 0 };

HFONT DropFiles::DropFilesFont = { 0 };
BOOL DropFiles::FileDroped = FALSE;
RECT DropFiles::Dimensions = { 0 };
#pragma endregion

#pragma region InitDropFiles
/// <summary>
/// Optional Function - Creates Class "DROP FILES"
/// </summary>
/// <returns>If Function Succeeded returns TRUE, but If not returns FALSE</returns>
BOOL DropFiles::InitDropFiles() {

	CreateDropFilesFont();

	WNDCLASSEX DropFilesEx = { 0 };

	DropFilesEx.cbClsExtra = 0;
	DropFilesEx.cbWndExtra = 0;
	DropFilesEx.cbSize = sizeof(WNDCLASSEX);
	DropFilesEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	DropFilesEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	DropFilesEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	DropFilesEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	DropFilesEx.hInstance = HInstance();
	DropFilesEx.lpfnWndProc = DropFilesProcedure;
	DropFilesEx.lpszClassName = L"DROP FILES";
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

VOID DropFiles::drawDashedRectangle(HDC hdc, RECT &Rectangle, UINT Width, COLORREF Color) {

	HPEN Pen = CreatePen(PS_DASH, 1, Color);
	HPEN PreviousPen = (HPEN)SelectObject(hdc, Pen);

	for (UINT W = 0; W < Width; W++) {

		// TOP
		MoveToEx(hdc, Rectangle.left, Rectangle.top + W, NULL);
		LineTo(hdc, Rectangle.right, Rectangle.top + W);
		// BOTTOM
		MoveToEx(hdc, Rectangle.left, Rectangle.bottom - W, NULL);
		LineTo(hdc, Rectangle.right, Rectangle.bottom - W);
		// LEFT
		MoveToEx(hdc, Rectangle.left + W, Rectangle.top, NULL);
		LineTo(hdc, Rectangle.left + W, Rectangle.bottom);
		// RIGHT
		MoveToEx(hdc, Rectangle.right - W, Rectangle.top, NULL);
		LineTo(hdc, Rectangle.right - W, Rectangle.bottom);

	}

	SelectObject(hdc, PreviousPen);
	DeleteObject(Pen);

}

VOID DropFiles::FillRectOpacity50(HDC hdc, RECT &Rectangle, COLORREF Color) {

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

VOID DropFiles::drawArrow(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH, INT HEIGHT, COLORREF Color) {

	CONST SHORT Proportion = 3;
	INT OFFSET = 0, XCELL = WIDTH / Proportion, YCELL = HEIGHT / Proportion;

	for (INT Y = COORD_Y; Y <= COORD_Y + HEIGHT; Y++) {

		if (Y < COORD_Y + YCELL * 2) {                                   // --|##|--
			for (INT X = COORD_X + XCELL; X <= COORD_X + XCELL * 2; X++) // --|##|-- x2
				SetPixel(hdc, X, Y, Color);                              // --|##|--
		} else {
			for (INT X = COORD_X + OFFSET; X <= COORD_X + WIDTH - OFFSET; X++) // ##|##|##
				SetPixel(hdc, X, Y, Color);                                    // -#|##|#- x1
			OFFSET++;                                                          // --|##|--
		}

	}

}
#pragma endregion

#pragma region Events
VOID DropFiles::onCreate(HWND hDropFiles, LPARAM lParam) {

	LPCREATESTRUCT window = (LPCREATESTRUCT)lParam;

	if (window->hwndParent != NULL and (window->style & WS_CHILD) != NULL and
		(window->style & WS_THICKFRAME) == NULL and (window->style & WS_DLGFRAME) == NULL and
		(window->style & WS_OVERLAPPED) == NULL and (window->style & WS_SYSMENU) == NULL) {

		DragAcceptFiles(hDropFiles, TRUE);

		DropFilesStyle *Style = new DropFilesStyle{ WHITE_COLOR, BLACK_COLOR }; // Default Initialization

		// Move Style Data To Heap Memory Structure | If "DropFilesStyle" Structure is Passed To lpParam
		if (window->lpCreateParams != NULL) {
			if (((LPDropFilesStyle)window->lpCreateParams)->BackgroundColor != NULL) Style->BackgroundColor = ((LPDropFilesStyle)window->lpCreateParams)->BackgroundColor;
			if (((LPDropFilesStyle)window->lpCreateParams)->ForegroundColor != NULL) Style->ForegroundColor = ((LPDropFilesStyle)window->lpCreateParams)->ForegroundColor;
		}
		////

		SetWindowLongPtr(hDropFiles, GWLP_USERDATA, (LONG_PTR)Style);

	} else {
		DestroyWindow(hDropFiles);
	}

}

VOID DropFiles::onPaint(HWND hDropFiles) {

	GetClientRect(hDropFiles, &Dimensions);

	DropFilesDC = BeginPaint(hDropFiles, &ps);

	MemoryDC = CreateCompatibleDC(DropFilesDC);
	Bitmap = CreateCompatibleBitmap(DropFilesDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);

	LPDropFilesStyle Style = (LPDropFilesStyle)GetWindowLongPtr(hDropFiles, GWLP_USERDATA);

	SetDCBrushColor(MemoryDC, Style->BackgroundColor);
	SetTextColor(MemoryDC, Style->ForegroundColor);

	////////////////////////////////////////////////////////////
	//// +------------------------------------------------+ ////
	//// |                                                | ////
	//// | [in] struct DropFilesStyle - BackgroundColor   | ////
	//// | [in] struct DropFilesStyle - ForegroundColor   | ////
	//// |                                                | ////
	//// +------------------------------------------------+ ////
	////////////////////////////////////////////////////////////

	FillRect(MemoryDC, &Dimensions, (HBRUSH)GetStockObject(DC_BRUSH));
	SelectObject(MemoryDC, DropFilesFont);

	// Border
	CONST SHORT BorderPadding = 4, BorderWidth = 2;
	RECT Border = { Dimensions.left + BorderPadding, Dimensions.top + BorderPadding, Dimensions.right - BorderPadding, Dimensions.bottom - BorderPadding };
	drawDashedRectangle(MemoryDC, Border, BorderWidth, Style->ForegroundColor);
	////

	// Text To User
	SIZE size = { 0 };
	WCHAR WindowTitle[MAX_DROP_FILES_CHAR_STRING] = { 0 };
	GetWindowText(hDropFiles, WindowTitle, ARRAYSIZE(WindowTitle));
	GetTextExtentPoint(MemoryDC, WindowTitle, lstrlenW(WindowTitle), &size);
	TextOut(MemoryDC, Dimensions.right / 2 - size.cx / 2, Dimensions.bottom / 2 - size.cy / 2, WindowTitle, lstrlenW(WindowTitle));
	////

	if (FileDroped) {
		CONST SHORT ArrowPadding = 20;
		INT ArrowWidth = Dimensions.right / 3, ArrowHeight = Dimensions.bottom - ArrowPadding * 2;
		FillRectOpacity50(MemoryDC, Dimensions, Style->ForegroundColor);
		drawArrow(MemoryDC, Dimensions.right / 2 - ArrowWidth / 2, Dimensions.bottom / 2 - ArrowHeight / 2, ArrowWidth, ArrowHeight, Style->ForegroundColor);
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
		LPDropFilesStyle Style = (LPDropFilesStyle)GetWindowLongPtr(hDropFiles, GWLP_USERDATA);
		delete[] Style;
		return 0;
	}
	}

	return DefWindowProc(hDropFiles, Msg, wParam, lParam);

}
#pragma endregion