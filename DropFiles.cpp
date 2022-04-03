#include "DropFiles.h"

// INIT DROP FILE STATIC MEMBERS

#pragma region CalculatorStaticMembers

HDC DropFiles::DropFilesDC = { 0 };
PAINTSTRUCT DropFiles::ps = { 0 };

HDC DropFiles::MemoryDC = { 0 };
HBITMAP DropFiles::Bitmap = { 0 };

COLORREF DropFiles::TextColor = RGB(0, 0, 0);

COLORREF DropFiles::DropFilesBackgroundColor = RGB(255, 255, 255);
HBRUSH DropFiles::DropFilesBackroundBrush = { 0 };

HFONT DropFiles::DropFilesFont = { 0 };

BOOL DropFiles::FileDroped = FALSE;

RECT DropFiles::Dimensions = { 0 };

#pragma endregion

// INIT DROP FILES

#pragma region InitDropFiles

BOOL DropFiles::InitDropFiles() {

	DropFilesFont = createDropFilesFont();

	WNDCLASSEX DropFilesEx = { 0 };

	DropFilesEx.cbClsExtra = 0;
	DropFilesEx.cbWndExtra = 0;
	DropFilesEx.cbSize = sizeof(WNDCLASSEX);
	DropFilesEx.hbrBackground = DropFilesBackroundBrush;
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

// FUNCTIONS

#pragma region Functions

HFONT DropFiles::createDropFilesFont() {

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
		L"Times New Roman");

	return Font;

}

VOID DropFiles::drawLineWithSpaces(HDC hdc, INT X, INT Y, INT LineLength, INT LineWidth, UINT Proportion, COLORREF Color, BOOL VerticalLine) {

	if (Proportion == 0) {
		OutputDebugString(L"[DropFiles::drawLineWithSpaces] - Proportion Must Be non Zero Value");
	}

	BOOL DRAW = TRUE;
	INT LineCell = LineLength / Proportion;

	if (VerticalLine == FALSE) {

		for (int horz = 0; horz <= LineLength; horz++) {

			// |---| X |---|
			if (DRAW == FALSE) {
				LineCell--;
				if (LineCell == 0) {
					DRAW = TRUE;
					LineCell = LineLength / Proportion;
				}
				continue;
			}

			// |-X-|   |-X-|
			for (int w = 0; w < LineWidth; w++) {
				SetPixel(hdc, X + horz, Y + w, Color);
			}
			LineCell--;
			if (LineCell == 0) {
				DRAW = FALSE;
				LineCell = LineLength / Proportion;
			}

		}

		return VOID();
	}

	if (VerticalLine == TRUE) {

		for (int vert = 0; vert <= LineLength; vert++) {

			// |---| X |---|
			if (DRAW == FALSE) {
				LineCell--;
				if (LineCell == 0) {
					DRAW = TRUE;
					LineCell = LineLength / Proportion;
				}
				continue;
			}

			// |-X-|   |-X-|
			for (int w = 0; w < LineWidth; w++) {
				SetPixel(hdc, X + w, Y + vert, Color);
			}
			LineCell--;
			if (LineCell == 0) {
				DRAW = FALSE;
				LineCell = LineLength / Proportion;
			}

		}

		return VOID();
	}

}

VOID DropFiles::drawArrow(HDC hdc, INT X, INT Y, INT W, INT H, COLORREF Color) {

	CONST SHORT Proportion = 3;
	INT OFFSET = 0;
	INT XS = 0, XE = W, YS = 0, YE = H, XCELL = W / Proportion, YCELL = H / Proportion;

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

//EVENTS

#pragma region Events

VOID DropFiles::onCreate(HWND hDropFiles, LPARAM lParam) {

	LPCREATESTRUCT window = (LPCREATESTRUCT)lParam;

	if (window->hwndParent != NULL && (window->style & WS_CHILD) != NULL &&
		(window->style & WS_POPUP) == NULL && (window->style & WS_DLGFRAME) == NULL &&
		(window->style & WS_OVERLAPPED) == NULL && (window->style & WS_SYSMENU) == NULL &&
		(window->style & WS_THICKFRAME) == NULL) {

		DragAcceptFiles(hDropFiles, TRUE);

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
	HBRUSH BackgroundBrush = CreateSolidBrush(DropFilesBackgroundColor);
	FillRect(MemoryDC, &Dimensions, BackgroundBrush);
	DeleteObject(BackgroundBrush);

	SelectObject(MemoryDC, DropFilesFont);
	SetTextColor(MemoryDC, TextColor);

	CONST SHORT BorderProportion = 20, BorderPadding = 4, BorderWidth = 2;

	// UP
	drawLineWithSpaces(MemoryDC, Dimensions.left + BorderPadding, Dimensions.top + BorderPadding,
		Dimensions.right - BorderPadding * 2, BorderWidth, BorderProportion, TextColor, FALSE);
	// DOWN
	drawLineWithSpaces(MemoryDC, Dimensions.left + BorderPadding, Dimensions.bottom - BorderPadding - BorderWidth,
		Dimensions.right - BorderPadding * 2, BorderWidth, BorderProportion, TextColor, FALSE);
	// LEFT
	drawLineWithSpaces(MemoryDC, Dimensions.left + BorderPadding, Dimensions.top + BorderPadding,
		Dimensions.bottom - BorderPadding * 2, BorderWidth, BorderProportion, TextColor, TRUE);
	// RIGHT
	drawLineWithSpaces(MemoryDC, Dimensions.right - BorderPadding - BorderWidth, Dimensions.top + BorderPadding,
		Dimensions.bottom - BorderPadding * 2, BorderWidth, BorderProportion, TextColor, TRUE);

	SIZE size = { 0 };
	WCHAR WindowTitle[MAX_DFTITLE_CHAR] = { 0 };
	GetWindowText(hDropFiles, WindowTitle, ARRAYSIZE(WindowTitle));
	GetTextExtentPoint(MemoryDC, WindowTitle, lstrlenW(WindowTitle), &size);

	if (lstrcmpW(WindowTitle, L"") == 0) {
		wcscpy_s(WindowTitle, L"Drop Files Here");
		GetTextExtentPoint(MemoryDC, WindowTitle, lstrlenW(WindowTitle), &size);
		TextOut(MemoryDC, Dimensions.right / 2 - size.cx / 2, Dimensions.bottom / 2 - size.cy / 2, WindowTitle, lstrlenW(WindowTitle));
	}
	else {
		TextOut(MemoryDC, Dimensions.right / 2 - size.cx / 2, Dimensions.bottom / 2 - size.cy / 2, WindowTitle, lstrlenW(WindowTitle));
	}

	if (FileDroped) {
		CONST SHORT PADDING = 20;
		for (int y = 0; y < Dimensions.right; y++) {
			for (int x = 0; x < Dimensions.right; x++) {
				if (y % 2 != NULL) {
					if (x % 2 != NULL) {
						SetPixel(MemoryDC, x, y, TextColor + 0x009C9C9C);
					}
					else if (x % 2 == NULL) {
						continue;
					}
				}
				else if (y % 2 == NULL) {
					if (x % 2 == NULL) {
						SetPixel(MemoryDC, x, y, TextColor + 0x009C9C9C);
					}
					else if (x % 2 != NULL) {
						continue;
					}
				}
			}
		}
		INT ArrowWidth = Dimensions.right / 3, ArrowHeight = Dimensions.bottom - PADDING * 2;
		drawArrow(MemoryDC, Dimensions.right / 2 - ArrowWidth / 2, Dimensions.bottom / 2 - ArrowHeight / 2, ArrowWidth, ArrowHeight, TextColor);
	}

	BitBlt(DropFilesDC, 0, 0, Dimensions.right, Dimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	if (FileDroped) {
		Sleep(200);
		FileDroped = FALSE;
		RedrawWindow(hDropFiles, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
	}

	DeleteDC(MemoryDC);
	DeleteObject(Bitmap);

	EndPaint(hDropFiles, &ps);

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
	DWORD ID = GetWindowLong(hDropFiles, GWL_ID);
	PostMessage(GetParent(hDropFiles), WM_COMMAND, MAKEWPARAM(ID, hDropFiles), wParam);
	RedrawWindow(hDropFiles, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);

}

#pragma endregion

#pragma region CalculatorProcedure

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
	}

	return DefWindowProc(hDropFiles, Msg, wParam, lParam);

}

#pragma endregion
