#include "ColorPicker.h"

// INIT COLOR PICKER STATIC MEMBERS

#pragma region InitColorPickerStaticMembers

HDC ColorPicker::ColorPickerDC = { 0 };
PAINTSTRUCT ColorPicker::ps = { 0 };

HDC ColorPicker::MemoryDC = { 0 };
HBITMAP ColorPicker::Bitmap = { 0 };

HBRUSH ColorPicker::ColorPickerBackgroundBrush = CreateSolidBrush(ColorPickerBackgroundColor);

CONST POINT ColorPicker::DimensionsSmall = { 420, 40 };
CONST POINT ColorPicker::DimensionsLarge = { 420, 100 };

RECT ColorPicker::Dimensions = { 0 };

POINT ColorPicker::mousePosition = { 0 };

BOOL ColorPicker::LMButtonPressed = FALSE;

#pragma endregion

// INIT COLOR PICKER

#pragma region InitColorPicker

BOOL ColorPicker::InitColorPicker() {

	/// <summary>
	/// OPTIONAL FUNCTION
	/// CREATES CLASS "COLOR PICKER"
	/// WINDOW TITLE "SMALL" CREATES SMALL COLOR PICKER X, Y = { 420, 40 }
	/// WINDOW TITLE "LARGE" CREATES LARGE COLOR PICKER X, Y = { 420, 100 }
	/// CONTROL RETURN ID THROUGH WM_COMMAND MESSAGE LOWORD(WPARAM)
	/// CONTROL RETURN HWND THROUGH WM_COMMAND MESSAGE HIWORD(WPARAM)
	/// CONTROL RETURN COLOR THROUGH WM_COMMAND MESSAGE LPARAM
	/// </summary>
	/// <returns>If Function Succeeded Returns True, but If not False</returns>

	WNDCLASSEX ColorPickerEx = { 0 };

	ColorPickerEx.cbClsExtra = 0;
	ColorPickerEx.cbWndExtra = 0;
	ColorPickerEx.cbSize = sizeof(WNDCLASSEX);
	ColorPickerEx.hbrBackground = ColorPickerBackgroundBrush;
	ColorPickerEx.hCursor = LoadCursor(NULL, IDC_CROSS);
	ColorPickerEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	ColorPickerEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	ColorPickerEx.hInstance = HInstance();
	ColorPickerEx.lpfnWndProc = ColorPickerProcedure;
	ColorPickerEx.lpszClassName = L"COLOR PICKER";
	ColorPickerEx.lpszMenuName = NULL;
	ColorPickerEx.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;

	if (!RegisterClassEx(&ColorPickerEx)) {
		return FALSE;
	}

	return TRUE;

}

#pragma endregion

// PAINT FUNCTIONS

#pragma region Functions

VOID ColorPicker::drawCross(HDC hdc, INT X, INT Y, INT W, INT H, COLORREF CrossColor) {

	/// <summary>
	/// This Function Draws Cross
	/// </summary>
	/// <param name="hdc">Device Context</param>
	/// <param name="X">X Coordinate</param>
	/// <param name="Y">Y Coordinate</param>
	/// <param name="W">Width</param>
	/// <param name="H">Height</param>
	/// <param name="CrossColor">Cross Color</param>

	if (W % 2 != NULL && H % 2 != NULL) {

		CONST INT Proportion = 3;
		INT XS = 0, XE = W, YS = 0, YE = H, XCELL = W / Proportion, YCELL = H / Proportion;

		YS = H / 2;
		for (XS = 0; XS <= XE; XS++) {

			if (XS > W / 2 - XCELL / 2 && XS < W / 2 + XCELL / 2) {
				continue;
			}

			SetPixel(hdc, X + XS, Y + YS - 1, CrossColor);
			SetPixel(hdc, X + XS, Y + YS, CrossColor);
			SetPixel(hdc, X + XS, Y + YS + 1, CrossColor);

		}
		XS = W / 2;
		for (YS = 0; YS <= YE; YS++) {

			if (YS > H / 2 - XCELL / 2 && YS < H / 2 + YCELL / 2) {
				continue;
			}

			SetPixel(hdc, X + XS - 1, Y + YS, CrossColor);
			SetPixel(hdc, X + XS, Y + YS, CrossColor);
			SetPixel(hdc, X + XS + 1, Y + YS, CrossColor);

		}

	}
	else {

		OutputDebugString(L"ERROR [ColorPicker::drawCross] - Width or Height Must be Odd Number Value!\r\n");

	}

}

INT ColorPicker::drawGradientSmall(HDC hdc, INT X, INT Y, INT W, INT H, INT BorderWidth, COLORREF BorderColor) {

	/// <summary>
	/// This Function Draws Small Gradient
	/// </summary>
	/// <param name="hdc">Device Context</param>
	/// <param name="X">X Coordinate</param>
	/// <param name="Y">Y Coordinate</param>
	/// <param name="W">Width</param>
	/// <param name="H">Height</param>
	/// <param name="BorderWidth">Border Width</param>
	/// <param name="BorderColor">Border Color</param>

	INT R = 255, G = 255, B = 255;
	INT XS = 0, XE = W, YS = 0, YE = H;
	CONST INT COLORSTEP = 5;
	INT DONE = 0;

	// Border

	for (int i = 0; i < BorderWidth; i++) {
		// Left
		for (YS = 0; YS <= YE; YS++) {
			SetPixel(hdc, X + XS + i, Y + YS, BorderColor);
		}
		// Up and Down
		for (XS = 0; XS <= XE; XS++) {
			YS = 0;
			SetPixel(hdc, X + XS, Y + YS + i, BorderColor);
			YS = H;
			SetPixel(hdc, X + XS, Y + YS - i, BorderColor);
		}
		// Right
		for (YS = 0; YS <= YE; YS++) {
			SetPixel(hdc, X + XE - i, Y + YS, BorderColor);
		}
		XS = 0;
	}

	XS = BorderWidth;

	//Gradient

	for (int i = XE; i >= BorderWidth * 2; i--) {
		for (YS = 0 + BorderWidth; YS <= YE - BorderWidth; YS++) {
			SetPixel(hdc, X + XS, Y + YS, RGB(R, G, B));
		}
		XS++;
		(DONE == 0) ? (G <= 0 && B <= 0) ? (G = 0, B = 0, DONE = 1) : (G = G - COLORSTEP, B = B - COLORSTEP) :
		(DONE == 1) ? (G >= 255) ? (G = 255, DONE = 2) : (G = G + COLORSTEP) :
		(DONE == 2) ? (R <= 0) ? (R = 0, DONE = 3) : (R = R - COLORSTEP) :
		(DONE == 3) ? (B >= 255) ? (B = 255, DONE = 4) : (B = B + COLORSTEP) :
		(DONE == 4) ? (G <= 0) ? (G = 0, DONE = 5) : (G = G - COLORSTEP) :
		(DONE == 5) ? (R >= 255) ? (R = 255, DONE = 6) : (R = R + COLORSTEP) :
		(DONE == 6) ? (B <= 0) ? (B = 0, DONE = 7) : (B = B - COLORSTEP) :
		(DONE == 7) ? (R <= 0) ? (R = 0, DONE = 8) : (R = R - COLORSTEP) :
		(DONE == 8);
	}

	return BorderWidth;

}

INT ColorPicker::drawGradientLarge(HDC hdc, INT X, INT Y, INT W, INT H, INT BorderWidth, COLORREF BorderColor) {

	/// <summary>
	/// This Function Draws Large Gradient
	/// </summary>
	/// <param name="hdc">Device Context</param>
	/// <param name="X">X Coordinate</param>
	/// <param name="Y">Y Coordinate</param>
	/// <param name="W">Width</param>
	/// <param name="H">Height</param>
	/// <param name="BorderWidth">Border Width</param>
	/// <param name="BorderColor">Border Color</param>

	INT R = 255, G = 255, B = 255;
	INT RS = 255, GS = 255, BS = 255;
	INT XS = 0, XE = W, YS = 0, YE = H;
	CONST INT COLORSTEP = 5;
	INT DONE = 0;

	// Border

	for (int i = 0; i < BorderWidth; i++) {
		// Left
		for (YS = 0; YS <= YE; YS++) {
			SetPixel(hdc, X + XS + i, Y + YS, BorderColor);
		}
		// Up and Down
		for (XS = 0; XS <= XE; XS++) {
			YS = 0;
			SetPixel(hdc, X + XS, Y + YS + i, BorderColor);
			YS = H;
			SetPixel(hdc, X + XS, Y + YS - i, BorderColor);
		}
		// Right
		for (YS = 0; YS <= YE; YS++) {
			SetPixel(hdc, X + XE - i, Y + YS, BorderColor);
		}
		XS = 0;
	}

	XS = BorderWidth;

	//Gradient

	for (int i = XE; i >= BorderWidth * 2; i--) {
		for (YS = 0 + BorderWidth; YS <= YE - BorderWidth; YS++) {
			SetPixel(hdc, X + XS, Y + YS, RGB(RS, GS, BS));
			(RS > R) ? RS = RS - COLORSTEP : RS = RS;
			(GS > G) ? GS = GS - COLORSTEP : GS = GS;
			(BS > B) ? BS = BS - COLORSTEP : BS = BS;
			(RS <= R && GS <= G && BS <= B) ? (RS != 0) ? RS = RS - COLORSTEP : RS = RS : RS = RS;
			(RS <= R && GS <= G && BS <= B) ? (GS != 0) ? GS = GS - COLORSTEP : GS = GS : GS = GS;
			(RS <= R && GS <= G && BS <= B) ? (BS != 0) ? BS = BS - COLORSTEP : BS = BS : BS = BS;
		}
		RS = 255, GS = 255, BS = 255, XS++;
		(DONE == 0) ? (G <= 0 && B <= 0) ? (G = 0, B = 0, DONE = 1) : (G = G - COLORSTEP, B = B - COLORSTEP) :
		(DONE == 1) ? (G >= 255) ? (G = 255, DONE = 2) : (G = G + COLORSTEP) :
		(DONE == 2) ? (R <= 0) ? (R = 0, DONE = 3) : (R = R - COLORSTEP) :
		(DONE == 3) ? (B >= 255) ? (B = 255, DONE = 4) : (B = B + COLORSTEP) :
		(DONE == 4) ? (G <= 0) ? (G = 0, DONE = 5) : (G = G - COLORSTEP) :
		(DONE == 5) ? (R >= 255) ? (R = 255, DONE = 6) : (R = R + COLORSTEP) :
		(DONE == 6) ? (B <= 0) ? (B = 0, DONE = 7) : (B = B - COLORSTEP) :
		(DONE == 7) ? (R <= 0) ? (R = 0, DONE = 8) : (R = R - COLORSTEP) :
		(DONE == 8);
	}

	return BorderWidth;

}

#pragma endregion

// EVENTS

#pragma region Events

VOID ColorPicker::onCreate(HWND hColorPicker, LPARAM lParam) {

	LPCREATESTRUCT window = LPCREATESTRUCT(lParam);

	if (window->hwndParent != NULL && (window->style & WS_CHILD) != NULL &&
		(window->style & WS_POPUP) == NULL && (window->style & WS_DLGFRAME) == NULL &&
		(window->style & WS_OVERLAPPED) == NULL && (window->style & WS_SYSMENU) == NULL &&
		(window->style & WS_THICKFRAME) == NULL) {

		if (lstrcmpW(window->lpszName, L"SMALL") == 0) {
			if (window->cx != 0 && window->cy != 0) {
				SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsSmall.x, DimensionsSmall.y, SWP_SHOWWINDOW);
			}
		}
		else if (lstrcmpW(window->lpszName, L"LARGE") == 0) {
			if (window->cx != 0 && window->cy != 0) {
				SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsLarge.x, DimensionsLarge.y, SWP_SHOWWINDOW);
			}
		}
		else {
			if (window->cx != 0 && window->cy != 0) {
				SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsSmall.x, DimensionsSmall.y, SWP_SHOWWINDOW);
			}
		}

	}
	else {

		DestroyWindow(hColorPicker);

	}

}

VOID ColorPicker::onWindowPosChanging(HWND hColorPicker, LPARAM lParam) {

	LPWINDOWPOS window = (LPWINDOWPOS)lParam;

	WCHAR WindowTitle[MAX_CPTITLE_CHAR] = { 0 };
	GetWindowText(hColorPicker, WindowTitle, ARRAYSIZE(WindowTitle));
	
	if (lstrcmpW(WindowTitle, L"SMALL") == 0) {
		if (window->cx != 0 && window->cy != 0) {
			window->cx = DimensionsSmall.x, window->cy = DimensionsSmall.y;
		}
	}
	else if (lstrcmpW(WindowTitle, L"LARGE") == 0) {
		if (window->cx != 0 && window->cy != 0) {
			window->cx = DimensionsLarge.x, window->cy = DimensionsLarge.y;
		}
	}
	else {
		if (window->cx != 0 && window->cy != 0) {
			window->cx = DimensionsSmall.x, window->cy = DimensionsSmall.y;
		}
	}

}

VOID ColorPicker::onMouseMove(HWND hColorPicker, WPARAM wParam, LPARAM lParam) {

	if (LMButtonPressed) {

		GetCursorPos(&mousePosition);
		ScreenToClient(hColorPicker, &mousePosition);

		GetClientRect(hColorPicker, &Dimensions);
		RedrawWindow(hColorPicker, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);

	}

}

VOID ColorPicker::onPaint(HWND hColorPicker) {

	GetClientRect(hColorPicker, &Dimensions);

	ColorPickerDC = BeginPaint(hColorPicker, &ps);

	MemoryDC = CreateCompatibleDC(ColorPickerDC);
	Bitmap = CreateCompatibleBitmap(ColorPickerDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	HBRUSH BackgroundBrush = CreateSolidBrush(ColorPickerBackgroundColor);
	FillRect(MemoryDC, &Dimensions, BackgroundBrush);
	DeleteObject(BackgroundBrush);

	INT BorderWidth = 0;

	WCHAR WindowTitle[MAX_CPTITLE_CHAR] = { 0 };
	GetWindowText(hColorPicker, WindowTitle, ARRAYSIZE(WindowTitle));

	if (lstrcmpW(WindowTitle, L"SMALL") == 0) {
		BorderWidth = drawGradientSmall(MemoryDC, Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom);
	}
	else if (lstrcmpW(WindowTitle, L"LARGE") == 0) {
		BorderWidth = drawGradientLarge(MemoryDC, Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom);
	}
	else {
		BorderWidth = drawGradientSmall(MemoryDC, Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom);
	}

	if (LMButtonPressed && mousePosition.x >= 0 + BorderWidth && mousePosition.y >= 0 + BorderWidth &&
		mousePosition.x <= Dimensions.right - BorderWidth && mousePosition.y <= Dimensions.bottom - BorderWidth) {

		COLORREF Color = GetPixel(MemoryDC, mousePosition.x, mousePosition.y);
		drawCross(MemoryDC, mousePosition.x - 23 / 2, mousePosition.y - 23 / 2);
		SetWindowLong(hColorPicker, GWL_USERDATA, MAKELONG(mousePosition.x, mousePosition.y));
		
		//////////////////////////////////////////////////////
		//// +------------------------------------------+ ////
		//// |                                          | ////
		//// | [out] WPARAM - LOWORD(ID) | HIWORD(HWND) | ////
		//// | [out] LPARAM - Color                     | ////
		//// |                                          | ////
		//// +------------------------------------------+ ////
		//////////////////////////////////////////////////////

		DWORD ID = GetWindowLong(hColorPicker, GWL_ID);
		PostMessage(GetParent(hColorPicker), WM_COMMAND, MAKEWPARAM(ID, hColorPicker), Color);

	}
	else {

		DWORD Point = GetWindowLong(hColorPicker, GWL_USERDATA);

		if (Point != NULL) {

			drawCross(MemoryDC, LOWORD(Point) - 23 / 2, HIWORD(Point) - 23 / 2);

		}


	}

	BitBlt(ColorPickerDC, 0, 0, Dimensions.right, Dimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	DeleteDC(MemoryDC);
	DeleteObject(Bitmap);

	EndPaint(hColorPicker, &ps);

}

#pragma endregion

// WNDPROC

#pragma region ColorPickerProcedure

LRESULT CALLBACK ColorPicker::ColorPickerProcedure(HWND hColorPicker, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{
		onCreate(hColorPicker, lParam);
		return 0;
	}
	case WM_WINDOWPOSCHANGING:
	{
		onWindowPosChanging(hColorPicker, lParam);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		onMouseMove(hColorPicker, wParam, lParam);
		return 0;
	}
	case WM_PAINT:
	{
		onPaint(hColorPicker);
		return 0;
	}
	case WM_NCMOUSEMOVE:
	{
		LMButtonPressed = FALSE;
		GetCursorPos(&mousePosition);
		ScreenToClient(hColorPicker, &mousePosition);
		GetClientRect(hColorPicker, &Dimensions);
		RedrawWindow(hColorPicker, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		LMButtonPressed = TRUE;
		GetCursorPos(&mousePosition);
		ScreenToClient(hColorPicker, &mousePosition);
		GetClientRect(hColorPicker, &Dimensions);
		RedrawWindow(hColorPicker, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		LMButtonPressed = FALSE;
		GetCursorPos(&mousePosition);
		ScreenToClient(hColorPicker, &mousePosition);
		GetClientRect(hColorPicker, &Dimensions);
		RedrawWindow(hColorPicker, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
		return 0;
	}
	}

	return DefWindowProc(hColorPicker, Msg, wParam, lParam);


}
#pragma endregion
