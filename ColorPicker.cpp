#include "ColorPicker.h"

#pragma region InitColorPickerStaticMembers

HDC ColorPicker::ColorPickerDC = { 0 };
PAINTSTRUCT ColorPicker::ps = { 0 };

HDC ColorPicker::MemoryDC = { 0 };
HBITMAP ColorPicker::Bitmap = { 0 };

CONST POINT ColorPicker::DimensionsSmall = { 420, 40 };
CONST POINT ColorPicker::DimensionsLarge = { 420, 100 };

RECT ColorPicker::Dimensions = { 0 };

HPEN ColorPicker::PBorder = { 0 };
HBRUSH ColorPicker::BColor = ( 0 );

RECT ColorPicker::SelectedColor = { 0 };

POINT ColorPicker::mousePosition = { 0 };

#pragma endregion

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
	ColorPickerEx.hbrBackground = CreateSolidBrush(ColorPickerBackgroundColor);
	ColorPickerEx.hCursor = LoadCursor(NULL, IDC_CROSS);
	ColorPickerEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	ColorPickerEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	ColorPickerEx.hInstance = HInstance();
	ColorPickerEx.lpfnWndProc = ColorPickerProcedure;
	ColorPickerEx.lpszClassName = L"COLOR PICKER";
	ColorPickerEx.lpszMenuName = NULL;
	ColorPickerEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	if (!RegisterClassEx(&ColorPickerEx)) {
		return FALSE;
	}

	return TRUE;

}

#pragma endregion

// PAINT FUNCTIONS

#pragma region Functions

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

void ColorPicker::onCreate(HWND hColorPicker, LPARAM lParam) {

	LPCREATESTRUCT window = LPCREATESTRUCT(lParam);

	if (window->hwndParent == NULL || (window->style & WS_CHILD) == NULL ||
		(window->style & WS_POPUP) != NULL || (window->style & WS_DLGFRAME) != NULL ||
		(window->style & WS_OVERLAPPED) != NULL || (window->style & WS_SYSMENU) != NULL ||
		(window->style & WS_THICKFRAME) != NULL) {
		DestroyWindow(hColorPicker);
	}

	Dimensions.right = window->cx;
	Dimensions.bottom = window->cy;

	std::wstring WWindowTitle = window->lpszName;

	if (WWindowTitle == L"SMALL") {
		if (window->cx != 0 || window->cy != 0) {
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsSmall.x, DimensionsSmall.y, SWP_SHOWWINDOW);
		}
	}
	else if (WWindowTitle == L"LARGE") {
		if (window->cx != 0 || window->cy != 0) {
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsLarge.x, DimensionsLarge.y, SWP_SHOWWINDOW);
		}
	}
	else {
		if (window->cx != 0 || window->cy != 0) {
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsSmall.x, DimensionsSmall.y, SWP_SHOWWINDOW);
		}
	}

}

void ColorPicker::onWindowPosChanged(HWND hColorPicker, LPARAM lParam) {

	LPWINDOWPOS window = (LPWINDOWPOS)lParam;

	Dimensions.right = window->cx;
	Dimensions.bottom = window->cy;

	WCHAR WindowTitle[MAX_CPTITLE_CHAR] = { 0 };
	GetWindowText(hColorPicker, WindowTitle, ARRAYSIZE(WindowTitle));
	std::wstring WWindowTitle = WindowTitle;

	if (WWindowTitle == L"SMALL") {
		if (window->cx != 0 || window->cy != 0) {
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsSmall.x, DimensionsSmall.y, SWP_SHOWWINDOW);
		}
	}
	else if (WWindowTitle == L"LARGE") {
		if (window->cx != 0 || window->cy != 0) {
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsLarge.x, DimensionsLarge.y, SWP_SHOWWINDOW);
		}
	}
	else {
		if (window->cx != 0 || window->cy != 0) {
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsSmall.x, DimensionsSmall.y, SWP_SHOWWINDOW);
		}
	}

}

void ColorPicker::onMouseMove(HWND hColorPicker, WPARAM wParam, LPARAM lParam) {

	GetCursorPos(&mousePosition);
	ScreenToClient(hColorPicker, &mousePosition);

	RedrawWindow(hColorPicker, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);

}

void ColorPicker::onPaint(HWND hColorPicker) {

	ColorPickerDC = BeginPaint(hColorPicker, &ps);

	MemoryDC = CreateCompatibleDC(ColorPickerDC);
	Bitmap = CreateCompatibleBitmap(ColorPickerDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	FillRect(MemoryDC, &Dimensions, CreateSolidBrush(ColorPickerBackgroundColor));

	INT BorderWidth = 0;

	WCHAR WindowTitle[MAX_CPTITLE_CHAR] = { 0 };
	GetWindowText(hColorPicker, WindowTitle, ARRAYSIZE(WindowTitle));
	std::wstring WWindowTitle = WindowTitle;

	if (WWindowTitle == L"SMALL") {
		BorderWidth = drawGradientSmall(MemoryDC, Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom);
	}
	else if (WWindowTitle == L"LARGE") {
		BorderWidth = drawGradientLarge(MemoryDC, Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom);
	}
	else {
		BorderWidth = drawGradientSmall(MemoryDC, Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom);
	}

	SelectObject(MemoryDC, BColor); SelectObject(MemoryDC, PBorder);

	Ellipse(MemoryDC, SelectedColor.left, SelectedColor.top, SelectedColor.right, SelectedColor.bottom);

	GetAsyncKeyState(VK_LBUTTON);

	if (GetAsyncKeyState(VK_LBUTTON) && mousePosition.x >= 0 + BorderWidth && mousePosition.y >= 0 + BorderWidth &&
		mousePosition.x <= Dimensions.right - BorderWidth && mousePosition.y <= Dimensions.bottom - BorderWidth) {

		COLORREF Color = GetPixel(MemoryDC, mousePosition.x, mousePosition.y);
		PBorder = CreatePen(PS_SOLID, BorderWidth, RGB(0, 0, 0)); BColor = CreateSolidBrush(Color);
		SelectObject(MemoryDC, BColor); SelectObject(MemoryDC, PBorder);
		SelectedColor = { mousePosition.x - 10, mousePosition.y - 10, mousePosition.x + 10, mousePosition.y + 10 };
		Ellipse(MemoryDC, SelectedColor.left, SelectedColor.top, SelectedColor.right, SelectedColor.bottom);
		DWORD ID = GetWindowLong(hColorPicker, GWL_ID);
		
		//// 
		//// +------------------------------------+
		//// |                                    |
		//// | WPARAM - LOWORD(ID) | HIWORD(HWND) |
		//// | LPARAM - Color                     |
		//// |                                    |
		//// +------------------------------------+
		//// 

		PostMessage(GetParent(hColorPicker), WM_COMMAND, MAKEWPARAM(ID, hColorPicker), Color);

	}

	GetAsyncKeyState(VK_LBUTTON);

	BitBlt(ColorPickerDC, 0, 0, Dimensions.right, Dimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	EndPaint(hColorPicker, &ps);

}

#pragma endregion

// WNDPROC

#pragma region ColorPickerProcedure

LRESULT ColorPicker::ColorPickerProcedure(HWND hColorPicker, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_CREATE:
	{
		onCreate(hColorPicker, lParam);
		return 0;
	}
	case WM_WINDOWPOSCHANGED:
	{
		onWindowPosChanged(hColorPicker, lParam);
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
	case WM_LBUTTONDOWN:
	{
		RedrawWindow(hColorPicker, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		RedrawWindow(hColorPicker, &Dimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
		return 0;
	}
	}
	return DefWindowProc(hColorPicker, msg, wParam, lParam);

}

#pragma endregion
