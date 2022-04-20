#include "ColorPicker.h"

#pragma region InitColorPickerStaticMembers
HDC ColorPicker::ColorPickerDC = { 0 };
PAINTSTRUCT ColorPicker::ps = { 0 };

HDC ColorPicker::MemoryDC = { 0 };
HBITMAP ColorPicker::Bitmap = { 0 };

CONST SIZE ColorPicker::DimensionsSmall = { 420, 40 };
CONST SIZE ColorPicker::DimensionsLarge = { 420, 100 };

RECT ColorPicker::Dimensions = { 0 };

POINT ColorPicker::mousePosition = { 0 };
BOOL ColorPicker::LMButtonPressed = FALSE;
#pragma endregion

// INIT COLOR PICKER

#pragma region InitColorPicker
/// <summary>
/// Optional Function - Creates Class "COLOR PICKER"
/// Window Title "SMALL" Creates Small "COLOR PICKER" X | Y = 420 | 40
/// Window Title "LARGE" Creates Large "COLOR PICKER" X | Y = 420 | 100
/// </summary>
/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
BOOL ColorPicker::InitColorPicker() {

	WNDCLASSEX ColorPickerEx = { 0 };

	ColorPickerEx.cbClsExtra = 0;
	ColorPickerEx.cbWndExtra = 0;
	ColorPickerEx.cbSize = sizeof(WNDCLASSEX);
	ColorPickerEx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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

#pragma region Functions
VOID ColorPicker::drawCross(HDC hdc, INT X, INT Y, INT W, INT H, COLORREF CrossColor) {

	if (W % 2 != NULL && H % 2 != NULL) {

		CONST SHORT Proportion = 3;
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

SHORT ColorPicker::drawGradientSmall(HDC hdc, INT COORD_X, INT COORD_Y, SHORT BorderWidth, COLORREF BorderColor) {

	CONST SHORT Width = 420, Height = 40;

	// Border
	for (SHORT Counter = 0; Counter < BorderWidth; Counter++) {
		// Left and Right
		for (INT Y = COORD_Y; Y <= COORD_Y + Height; Y++) {
			SetPixel(hdc, COORD_X + Counter, Y, BorderColor);
			SetPixel(hdc, COORD_X + Width - Counter, Y, BorderColor);
		}
		// Up and Down
		for (INT X = COORD_X; X <= COORD_X + Width; X++) {
			SetPixel(hdc, X, COORD_Y + Counter, BorderColor);
			SetPixel(hdc, X, COORD_Y + Height - Counter, BorderColor);
		}
	}

	BYTE R = 255, G = 255, B = 255;
	CONST BYTE COLORSTEP = 5;
	BYTE DONE = 0;

	//Gradient
	for (INT X = COORD_X + BorderWidth; X <= COORD_X + Width - BorderWidth; X++) {
		for (INT Y = COORD_Y + BorderWidth; Y <= COORD_Y + Height - BorderWidth; Y++) {
			SetPixel(hdc, X, Y, RGB(R, G, B));
		}
		if (DONE == 0) (G == 0 && B == 0) ? DONE = 1 : (G -= COLORSTEP, B -= COLORSTEP); // White [255 255 255] -> Red [255 0 0]
		else if (DONE == 1) (G == 255) ? DONE = 2 : G += COLORSTEP; // Red [255 0 0] -> Yellow [255 255 0]
		else if (DONE == 2) (R == 0) ? DONE = 3 : R -= COLORSTEP; // Yellow [255 255 0] -> Grean [0 255 0]
		else if (DONE == 3) (B == 255) ? DONE = 4 : B += COLORSTEP; // Grean [0 255 0] -> Light Blue [0 255 255]
		else if (DONE == 4) (G == 0) ? DONE = 5 : G -= COLORSTEP; // Light Blue [0 255 255] -> Blue [0 0 255]
		else if (DONE == 5) (R == 255) ? DONE = 6 : R += COLORSTEP; // Blue [0 0 255] -> Pink [255 0 255]
		else if (DONE == 6) (B == 0) ? DONE = 7 : B -= COLORSTEP; // Pink [255 0 255] -> Red [255 0 0]
		else if (DONE == 7) (R == 0) ? DONE = 8 : R = R - COLORSTEP; // Red [255 0 0] -> Black [0 0 0]
	}

	return BorderWidth;

}

SHORT ColorPicker::drawGradientLarge(HDC hdc, INT COORD_X, INT COORD_Y, SHORT BorderWidth, COLORREF BorderColor) {

	CONST SHORT Width = 420, Height = 100;

	// Border
	for (SHORT Counter = 0; Counter < BorderWidth; Counter++) {
		// Left and Right
		for (INT Y = COORD_Y; Y <= COORD_Y + Height; Y++) {
			SetPixel(hdc, COORD_X + Counter, Y, BorderColor);
			SetPixel(hdc, COORD_X + Width - Counter, Y, BorderColor);
		}
		// Up and Down
		for (INT X = COORD_X; X <= COORD_X + Width; X++) {
			SetPixel(hdc, X, COORD_Y + Counter, BorderColor);
			SetPixel(hdc, X, COORD_Y + Height - Counter, BorderColor);
		}
	}

	BYTE R = 255, G = 255, B = 255;
	BYTE RY = 255, GY = 255, BY = 255;
	CONST BYTE COLORSTEP = 5;
	BYTE DONE = 0;

	//Gradient
	for (INT X = COORD_X + BorderWidth; X <= COORD_X + Width - BorderWidth; X++) {
		for (INT Y = COORD_Y + BorderWidth; Y <= COORD_Y + Height - BorderWidth; Y++) {
			SetPixel(hdc, X, Y, RGB(RY, GY, BY));
			if (Y < Height / 2) {
				if (RY != R) RY -= COLORSTEP; //
				if (GY != G) GY -= COLORSTEP; // Up - Middle
				if (BY != B) BY -= COLORSTEP; //
			}
			else {
				if (RY != 0) RY -= COLORSTEP; //
				if (GY != 0) GY -= COLORSTEP; // Middle - Down
				if (BY != 0) BY -= COLORSTEP; //
			}
		}
		RY = 255, GY = 255, BY = 255;
		if (DONE == 0) (G == 0 && B == 0) ? DONE = 1 : (G -= COLORSTEP, B -= COLORSTEP); // White [255 255 255] -> Red [255 0 0]
		else if (DONE == 1) (G == 255) ? DONE = 2 : G += COLORSTEP; // Red [255 0 0] -> Yellow [255 255 0]
		else if (DONE == 2) (R == 0) ? DONE = 3 : R -= COLORSTEP; // Yellow [255 255 0] -> Grean [0 255 0]
		else if (DONE == 3) (B == 255) ? DONE = 4 : B += COLORSTEP; // Grean [0 255 0] -> Light Blue [0 255 255]
		else if (DONE == 4) (G == 0) ? DONE = 5 : G -= COLORSTEP; // Light Blue [0 255 255] -> Blue [0 0 255]
		else if (DONE == 5) (R == 255) ? DONE = 6 : R += COLORSTEP; // Blue [0 0 255] -> Pink [255 0 255]
		else if (DONE == 6) (B == 0) ? DONE = 7 : B -= COLORSTEP; // Pink [255 0 255] -> Red [255 0 0]
		else if (DONE == 7) (R == 0) ? DONE = 8 : R = R - COLORSTEP; // Red [255 0 0] -> Black [0 0 0]
	}

	return BorderWidth;

}
#pragma endregion

#pragma region OverloadedOperator
static BOOL operator==(POINT &mousePosition, RECT &Rectangle) {

	if (mousePosition.x >= Rectangle.left && mousePosition.x <= Rectangle.right &&
		mousePosition.y >= Rectangle.top && mousePosition.y <= Rectangle.bottom) {
		return TRUE;
	}
	else {
		return FALSE;
	}

}
#pragma endregion

#pragma region Events
VOID ColorPicker::onCreate(HWND hColorPicker, LPARAM lParam) {

	LPCREATESTRUCT window = LPCREATESTRUCT(lParam);

	if (window->hwndParent != NULL && (window->style & WS_CHILD) != NULL &&
		(window->style & WS_POPUP) == NULL && (window->style & WS_DLGFRAME) == NULL &&
		(window->style & WS_OVERLAPPED) == NULL && (window->style & WS_SYSMENU) == NULL &&
		(window->style & WS_THICKFRAME) == NULL) {

		if (lstrcmpW(window->lpszName, L"SMALL") == 0 && window->cx != 0 && window->cy != 0)
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsSmall.cx, DimensionsSmall.cy, SWP_SHOWWINDOW);
		else if (lstrcmpW(window->lpszName, L"LARGE") == 0 && window->cx != 0 && window->cy != 0)
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsLarge.cx, DimensionsLarge.cy, SWP_SHOWWINDOW);
		else if (window->cx != 0 && window->cy != 0)
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsSmall.cx, DimensionsSmall.cy, SWP_SHOWWINDOW);

	}
	else {
		OutputDebugString(L"ERROR [Color Picker] - \"hwndParent\" Must Be Non Zero Value\r\n");
		DestroyWindow(hColorPicker);

	}

}

VOID ColorPicker::onWindowPosChanging(HWND hColorPicker, LPARAM lParam) {

	LPWINDOWPOS window = (LPWINDOWPOS)lParam;

	WCHAR WindowTitle[MAX_COLOR_PICKER_CHAR_STRING] = { 0 };
	GetWindowText(hColorPicker, WindowTitle, ARRAYSIZE(WindowTitle));

	if (lstrcmpW(WindowTitle, L"SMALL") == 0 && window->cx != 0 && window->cy != 0)
		window->cx = DimensionsSmall.cx, window->cy = DimensionsSmall.cy;
	else if (lstrcmpW(WindowTitle, L"LARGE") == 0 && window->cx != 0 && window->cy != 0)
		window->cx = DimensionsLarge.cx, window->cy = DimensionsLarge.cy;
	else if (window->cx != 0 && window->cy != 0)
		window->cx = DimensionsSmall.cx, window->cy = DimensionsSmall.cy;

}

VOID ColorPicker::onMouseMove(HWND hColorPicker, WPARAM wParam, LPARAM lParam) {

	if (LMButtonPressed) {
		RedrawWindow(hColorPicker, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
	}

}

VOID ColorPicker::onPaint(HWND hColorPicker) {

	GetCursorPos(&mousePosition);
	ScreenToClient(hColorPicker, &mousePosition);
	GetClientRect(hColorPicker, &Dimensions);

	ColorPickerDC = BeginPaint(hColorPicker, &ps);

	MemoryDC = CreateCompatibleDC(ColorPickerDC);
	Bitmap = CreateCompatibleBitmap(ColorPickerDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	SetDCBrushColor(MemoryDC, ColorPickerBackgroundColor);
	FillRect(MemoryDC, &Dimensions, (HBRUSH)GetStockObject(DC_BRUSH));

	WCHAR WindowTitle[MAX_COLOR_PICKER_CHAR_STRING] = { 0 };
	GetWindowText(hColorPicker, WindowTitle, ARRAYSIZE(WindowTitle));

	// Draw Gradient
	SHORT BorderWidth = 0;
	if (lstrcmpW(WindowTitle, L"SMALL") == 0)
		BorderWidth = drawGradientSmall(MemoryDC, Dimensions.left, Dimensions.top);
	else if (lstrcmpW(WindowTitle, L"LARGE") == 0)
		BorderWidth = drawGradientLarge(MemoryDC, Dimensions.left, Dimensions.top);
	else
		BorderWidth = drawGradientSmall(MemoryDC, Dimensions.left, Dimensions.top);
	////
	
	RECT GradientRectangle = { Dimensions.left + BorderWidth, Dimensions.top + BorderWidth,
		Dimensions.right - BorderWidth, Dimensions.bottom - BorderWidth };

	if (LMButtonPressed && mousePosition == GradientRectangle) {
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

		PostMessage(GetParent(hColorPicker), WM_COMMAND, MAKEWPARAM(GetWindowLong(hColorPicker, GWL_ID), hColorPicker), Color);
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
		RedrawWindow(hColorPicker, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		LMButtonPressed = TRUE;
		RedrawWindow(hColorPicker, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		LMButtonPressed = FALSE;
		RedrawWindow(hColorPicker, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
		return 0;
	}
	}

	return DefWindowProc(hColorPicker, Msg, wParam, lParam);


}
#pragma endregion
