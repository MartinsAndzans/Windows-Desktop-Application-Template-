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
/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
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

VOID DropFiles::drawDashedRectangle(HDC hdc, RECT &Rectangle, SIZE_T Width, COLORREF Color) {

	HPEN Pen = CreatePen(PS_DASH, 1, Color);
	HPEN PreviousPen = (HPEN)SelectObject(hdc, Pen);

	for (int width = 0; width < Width; width++) {

		// TOP
		MoveToEx(hdc, Rectangle.left, Rectangle.top + width, NULL);
		LineTo(hdc, Rectangle.right, Rectangle.top + width);
		// BOTTOM
		MoveToEx(hdc, Rectangle.left, Rectangle.bottom - width, NULL);
		LineTo(hdc, Rectangle.right, Rectangle.bottom - width);
		// LEFT
		MoveToEx(hdc, Rectangle.left + width, Rectangle.top, NULL);
		LineTo(hdc, Rectangle.left + width, Rectangle.bottom);
		// RIGHT
		MoveToEx(hdc, Rectangle.right - width, Rectangle.top, NULL);
		LineTo(hdc, Rectangle.right - width, Rectangle.bottom);

	}

	SelectObject(hdc, PreviousPen);
	DeleteObject(Pen);

}

VOID DropFiles::FillRectOpacity50(HDC hdc, RECT &Rectangle, COLORREF Color) {

	BOOL DRAWPIXEL; // TRUE = |X| - FALSE = | |
	for (int x = Rectangle.left; x <= Rectangle.right; x++) {
		(x % 2 == NULL) ? DRAWPIXEL = TRUE : DRAWPIXEL = FALSE; // 0, 2, 4, 5 = TRUE - 1, 3, 5, 7 = FALSE 
		for (int y = Rectangle.top; y <= Rectangle.bottom; y++) {
			if (DRAWPIXEL) {
				DRAWPIXEL = FALSE;
				SetPixel(hdc, x, y, Color);
			}
			else {
				DRAWPIXEL = TRUE;
				continue;
			}
		}
	}

}

VOID DropFiles::drawArrow(HDC hdc, INT X, INT Y, INT W, INT H, COLORREF Color) {

	CONST SHORT Proportion = 3;
	INT OFFSET = 0, XS = 0, XE = W, YS = 0, YE = H, XCELL = W / Proportion, YCELL = H / Proportion;

	// XS    |    |    XE
	// ******+----+****** YS
	// ******|    |******
	// ******|    |******
	// ******|    |******
	// ******|    |******
	// ******+----+******--
	// ******|    |******
	// ******|    |******
	// ******|    |******
	// ******|    |******
	// ***+----------+***--
	// ****\        /****
	// *****\      /*****
	// ******\    /******
	// *******\  /*******
	// ********\/******** YE

	for (YS = 0; YS <= YE; YS++) {

		if (YS >= YCELL * 2) {
			for (XS = XCELL / 2 + OFFSET; XS <= XCELL * 2 + XCELL / 2 - OFFSET; XS++) {
				SetPixel(hdc, X + XS, Y + YS, Color);
			}
			OFFSET++;
			continue;
		}

		for (XS = XCELL; XS <= XCELL * 2; XS++){
			SetPixel(hdc, X + XS, Y + YS, Color);
		}

	}

}
#pragma endregion

#pragma region Events
VOID DropFiles::onCreate(HWND hDropFiles, LPARAM lParam) {

	LPCREATESTRUCT window = (LPCREATESTRUCT)lParam;

	if (window->hwndParent != NULL && (window->style & WS_CHILD) != NULL &&
		(window->style & WS_POPUP) == NULL && (window->style & WS_DLGFRAME) == NULL &&
		(window->style & WS_OVERLAPPED) == NULL && (window->style & WS_SYSMENU) == NULL &&
		(window->style & WS_THICKFRAME) == NULL) {

		DragAcceptFiles(hDropFiles, TRUE);

		LpDropFilesStyle Parameters = (LpDropFilesStyle)window->lpCreateParams;

		DropFilesStyle *Style = new DropFilesStyle;
		ZeroMemory(Style, sizeof(DropFilesStyle));

		// Move Style Data To Heap
		if (window->lpCreateParams != NULL) {
			Style->BackgroundColor = Parameters->BackgroundColor;
			Style->ForegroundColor = Parameters->ForegroundColor;
		}

		SetWindowLongPtr(hDropFiles, GWLP_USERDATA, (LONG_PTR)Style);

	}
	else {

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

	LpDropFilesStyle Style = (LpDropFilesStyle)GetWindowLongPtr(hDropFiles, GWLP_USERDATA);

	(Style->BackgroundColor != NULL) ? SetDCBrushColor(MemoryDC, Style->BackgroundColor) : SetDCBrushColor(MemoryDC, DropFilesBackroundColor); // DEFAULT
	(Style->ForegroundColor != NULL) ? SetTextColor(MemoryDC, Style->ForegroundColor) : SetTextColor(MemoryDC, DropFilesForegroundColor); // DEFAULT

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

	// Margin
	CONST SHORT BorderPadding = 4, BorderWidth = 2;
	RECT Border = { Dimensions.left + BorderPadding, Dimensions.top + BorderPadding, Dimensions.right - BorderPadding, Dimensions.bottom - BorderPadding };
	drawDashedRectangle(MemoryDC, Border, BorderWidth, GetTextColor(MemoryDC));
	////

	// Text To User
	SIZE size = { 0 };
	WCHAR WindowTitle[MAX_DFTITLE_CHAR] = { 0 };
	GetWindowText(hDropFiles, WindowTitle, ARRAYSIZE(WindowTitle));
	GetTextExtentPoint(MemoryDC, WindowTitle, lstrlenW(WindowTitle), &size);
	TextOut(MemoryDC, Dimensions.right / 2 - size.cx / 2, Dimensions.bottom / 2 - size.cy / 2, WindowTitle, lstrlenW(WindowTitle));
	////

	if (FileDroped) {
		CONST SHORT ArrowPadding = 20;
		INT ArrowWidth = Dimensions.right / 3, ArrowHeight = Dimensions.bottom - ArrowPadding * 2;
		FillRectOpacity50(MemoryDC, Dimensions, GetTextColor(MemoryDC));
		drawArrow(MemoryDC, Dimensions.right / 2 - ArrowWidth / 2, Dimensions.bottom / 2 - ArrowHeight / 2, ArrowWidth, ArrowHeight, GetTextColor(MemoryDC));
	}

	BitBlt(DropFilesDC, 0, 0, Dimensions.right, Dimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	DeleteDC(MemoryDC);
	DeleteObject(Bitmap);

	EndPaint(hDropFiles, &ps);

	if (FileDroped) {
		Sleep(200); // 200 MilliSeconds Delay
		FileDroped = FALSE;
		RedrawWindow(hDropFiles, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
	}

}

VOID DropFiles::onDropFiles(HWND hDropFiles, WPARAM wParam) {

	GetClientRect(hDropFiles, &Dimensions);

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
	RedrawWindow(hDropFiles, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);

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
		LpDropFilesStyle Style = (LpDropFilesStyle)GetWindowLongPtr(hDropFiles, GWLP_USERDATA);
		delete[] Style;
		SetWindowLongPtr(hDropFiles, GWLP_USERDATA, NULL);
		return 0;
	}
	}

	return DefWindowProc(hDropFiles, Msg, wParam, lParam);

}
#pragma endregion
