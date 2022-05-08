#include "ColorPicker.h"

#pragma region InitColorPickerStaticMembers
CONST SIZE ColorPicker::DimensionsSmall = { 420, 40 };
CONST SIZE ColorPicker::DimensionsLarge = { 420, 100 };
#pragma endregion

// INIT COLOR PICKER

#pragma region InitColorPicker
/// <summary>
/// Optional Function - Creates Class "COLOR PICKER"
/// Window Title "Small" Creates Small "COLOR PICKER" 420 - 40
/// Window Title "Large" Creates Large "COLOR PICKER" 420 - 100
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
	ColorPickerEx.hInstance = GetModuleHandle(NULL);
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
VOID ColorPicker::drawCross(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH, INT HEIGHT, COLORREF CrossColor) {

	if (WIDTH % 2 != 0 and HEIGHT % 2 != 0) {

		CONST SHORT Proportion = 3;
		INT XCELL = WIDTH / Proportion, YCELL = HEIGHT / Proportion;

		// Horizontal Line
		for (INT X = COORD_X; X <= COORD_X + WIDTH; X++) {

			if (X > COORD_X + XCELL and X < COORD_X + XCELL * 2) {
				continue;
			}

			SetPixel(hdc, X, COORD_Y + HEIGHT / 2 - 1, CrossColor); // ==
			SetPixel(hdc, X, COORD_Y + HEIGHT / 2, CrossColor); // --
			SetPixel(hdc, X, COORD_Y + HEIGHT / 2 + 1, CrossColor); // ==

		}
		// Vertical Line
		for (INT Y = COORD_Y; Y <= COORD_Y + HEIGHT; Y++) {

			if (Y > COORD_Y + YCELL and Y < COORD_Y + YCELL * 2) {
				continue;
			}

			SetPixel(hdc, COORD_X + WIDTH / 2 - 1, Y, CrossColor); // ==
			SetPixel(hdc, COORD_X + WIDTH / 2, Y, CrossColor); // --
			SetPixel(hdc, COORD_X + WIDTH / 2 + 1, Y, CrossColor); // ==

		}

	}
	else {

		OutputDebugString(L"ERROR [Draw::drawCross] - Width or Height Must be Odd Number!\r\n");

	}

}
VOID ColorPicker::drawSmallGradient(HDC hdc, INT COORD_X, INT COORD_Y, COLORREF BorderColor) {

	CONST USHORT BorderWidth = 2;
	CONST USHORT WIDTH = 420, HEIGTH = 40;

	// Border
	for (USHORT W = 0; W < BorderWidth; W++) {
		// Up and Down
		for (INT X = COORD_X; X <= COORD_X + WIDTH; X++) {
			SetPixel(hdc, X, COORD_Y + W, BorderColor);
			SetPixel(hdc, X, COORD_Y + HEIGTH - W, BorderColor);
		}
		// Left and Right
		for (INT Y = COORD_Y; Y <= COORD_Y + HEIGTH; Y++) {
			SetPixel(hdc, COORD_X + W, Y, BorderColor);
			SetPixel(hdc, COORD_X + WIDTH - W, Y, BorderColor);
		}
	}

	BYTE R = 255, G = 255, B = 255;
	CONST BYTE COLORSTEP = 5;
	USHORT DONE = 0;

	//Gradient
	for (INT X = COORD_X + BorderWidth; X <= COORD_X + WIDTH - BorderWidth; X++) {
		for (INT Y = COORD_Y + BorderWidth; Y <= COORD_Y + HEIGTH - BorderWidth; Y++) {
			SetPixel(hdc, X, Y, RGB(R, G, B));
		}
		if (DONE == 0) (G == 0 and B == 0) ? DONE = 1 : (G -= COLORSTEP, B -= COLORSTEP); // White [255 255 255] -> Red [255 0 0]
		else if (DONE == 1) (G == 255) ? DONE = 2 : G += COLORSTEP; // Red [255 0 0] -> Yellow [255 255 0]
		else if (DONE == 2) (R == 0) ? DONE = 3 : R -= COLORSTEP; // Yellow [255 255 0] -> Grean [0 255 0]
		else if (DONE == 3) (B == 255) ? DONE = 4 : B += COLORSTEP; // Grean [0 255 0] -> Light Blue [0 255 255]
		else if (DONE == 4) (G == 0) ? DONE = 5 : G -= COLORSTEP; // Light Blue [0 255 255] -> Blue [0 0 255]
		else if (DONE == 5) (R == 255) ? DONE = 6 : R += COLORSTEP; // Blue [0 0 255] -> Pink [255 0 255]
		else if (DONE == 6) (B == 0) ? DONE = 7 : B -= COLORSTEP; // Pink [255 0 255] -> Red [255 0 0]
		else if (DONE == 7) (R == 0) ? DONE = 8 : R = R - COLORSTEP; // Red [255 0 0] -> Black [0 0 0]
	}

}
VOID ColorPicker::drawLargeGradient(HDC hdc, INT COORD_X, INT COORD_Y, COLORREF BorderColor) {

	CONST USHORT BorderWidth = 2;
	CONST USHORT WIDTH = 420, HEIGTH = 100;

	// Border
	for (USHORT W = 0; W < BorderWidth; W++) {
		// Up and Down
		for (INT X = COORD_X; X <= COORD_X + WIDTH; X++) {
			SetPixel(hdc, X, COORD_Y + W, BorderColor);
			SetPixel(hdc, X, COORD_Y + HEIGTH - W, BorderColor);
		}
		// Left and Right
		for (INT Y = COORD_Y; Y <= COORD_Y + HEIGTH; Y++) {
			SetPixel(hdc, COORD_X + W, Y, BorderColor);
			SetPixel(hdc, COORD_X + WIDTH - W, Y, BorderColor);
		}
	}

	BYTE R = 255, G = 255, B = 255;
	BYTE RY = 255, GY = 255, BY = 255;
	CONST BYTE COLORSTEP = 5;
	USHORT DONE = 0;

	//Gradient
	for (INT X = COORD_X + BorderWidth; X <= COORD_X + WIDTH - BorderWidth; X++) {
		for (INT Y = COORD_Y + BorderWidth; Y <= COORD_Y + HEIGTH - BorderWidth; Y++) {
			SetPixel(hdc, X, Y, RGB(RY, GY, BY));
			if (Y < COORD_Y + HEIGTH / 2) {
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
		if (DONE == 0) (G == 0 and B == 0) ? DONE = 1 : (G -= COLORSTEP, B -= COLORSTEP); // White [255 255 255] -> Red [255 0 0]
		else if (DONE == 1) (G == 255) ? DONE = 2 : G += COLORSTEP; // Red [255 0 0] -> Yellow [255 255 0]
		else if (DONE == 2) (R == 0) ? DONE = 3 : R -= COLORSTEP; // Yellow [255 255 0] -> Grean [0 255 0]
		else if (DONE == 3) (B == 255) ? DONE = 4 : B += COLORSTEP; // Grean [0 255 0] -> Light Blue [0 255 255]
		else if (DONE == 4) (G == 0) ? DONE = 5 : G -= COLORSTEP; // Light Blue [0 255 255] -> Blue [0 0 255]
		else if (DONE == 5) (R == 255) ? DONE = 6 : R += COLORSTEP; // Blue [0 0 255] -> Pink [255 0 255]
		else if (DONE == 6) (B == 0) ? DONE = 7 : B -= COLORSTEP; // Pink [255 0 255] -> Red [255 0 0]
		else if (DONE == 7) (R == 0) ? DONE = 8 : R = R - COLORSTEP; // Red [255 0 0] -> Black [0 0 0]
	}

}
#pragma endregion

#pragma region Events
VOID ColorPicker::onCreate(HWND hColorPicker, LPARAM lParam) {

	LPCREATESTRUCT window = reinterpret_cast<LPCREATESTRUCT>(lParam);

	if (window->hwndParent != NULL and (window->style & WS_CHILD) != NULL) {

		if (lstrcmpW(window->lpszName, L"Small") == 0 and window->cx != 0 and window->cy != 0) {
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsSmall.cx, DimensionsSmall.cy, SWP_SHOWWINDOW);
		} else if (lstrcmpW(window->lpszName, L"Large") == 0 and window->cx != 0 and window->cy != 0) {
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsLarge.cx, DimensionsLarge.cy, SWP_SHOWWINDOW);
		} else if (window->cx != 0 and window->cy != 0) {
			SetWindowPos(hColorPicker, NULL, window->x, window->y, DimensionsSmall.cx, DimensionsSmall.cy, SWP_SHOWWINDOW);
		}

	} else {

		OutputDebugStringA("\'ERROR \'ColorPicker\' - \"hwndParent\" Must Be Non Zero Value\r\n\'");
		DestroyWindow(hColorPicker);

	}

}

VOID ColorPicker::onWindowPosChanging(HWND hColorPicker, LPARAM lParam) {

	LPWINDOWPOS window = reinterpret_cast<LPWINDOWPOS>(lParam);

	WCHAR WindowTitle[MAX_COLOR_PICKER_CHAR_STRING] = { 0 };
	GetWindowText(hColorPicker, WindowTitle, ARRAYSIZE(WindowTitle));

	if (lstrcmpW(WindowTitle, L"Small") == 0 and window->cx != 0 and window->cy != 0) {
		window->cx = DimensionsSmall.cx, window->cy = DimensionsSmall.cy;
	} else if (lstrcmpW(WindowTitle, L"Large") == 0 and window->cx != 0 and window->cy != 0) {
		window->cx = DimensionsLarge.cx, window->cy = DimensionsLarge.cy;
	} else if (window->cx != 0 and window->cy != 0) {
		window->cx = DimensionsSmall.cx, window->cy = DimensionsSmall.cy;
	}

}

VOID ColorPicker::onMouseMove(HWND hColorPicker, WPARAM wParam, LPARAM lParam) {
	if (GetAsyncKeyState(VK_LBUTTON)) RedrawWindow(hColorPicker, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
}

VOID ColorPicker::onPaint(HWND hColorPicker) {

	PAINTSTRUCT ps = { 0 };
	RECT Dimensions = { 0 };
	POINT mousePosition = { 0 };

	GetCursorPos(&mousePosition);
	ScreenToClient(hColorPicker, &mousePosition);

	GetClientRect(hColorPicker, &Dimensions);

	HDC ColorPickerDC = BeginPaint(hColorPicker, &ps);

	HDC MemoryDC = CreateCompatibleDC(ColorPickerDC);
	HBITMAP Bitmap = CreateCompatibleBitmap(ColorPickerDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	FillRect(MemoryDC, &Dimensions, (HBRUSH)GetStockObject(BLACK_BRUSH));
	CONST COLORREF BlackColor = RGB(0, 0, 0);

	WCHAR WindowTitle[MAX_COLOR_PICKER_CHAR_STRING] = { 0 };
	GetWindowText(hColorPicker, WindowTitle, ARRAYSIZE(WindowTitle));

	// Draw Gradient
	if (lstrcmpW(WindowTitle, L"Small") == 0) {
		drawSmallGradient(MemoryDC, Dimensions.left, Dimensions.top, BlackColor);
	} else if (lstrcmpW(WindowTitle, L"Large") == 0) {
		drawLargeGradient(MemoryDC, Dimensions.left, Dimensions.top, BlackColor);
	} else {
		drawSmallGradient(MemoryDC, Dimensions.left, Dimensions.top, BlackColor);
	}
	////

	if (GetAsyncKeyState(VK_LBUTTON) and mousePosition.x >= Dimensions.left and mousePosition.x <= Dimensions.right and
		mousePosition.y >= Dimensions.top and mousePosition.y <= Dimensions.bottom) {

		COLORREF Color = GetPixel(MemoryDC, mousePosition.x, mousePosition.y);
		drawCross(MemoryDC, mousePosition.x - 23 / 2, mousePosition.y - 23 / 2, 23, 23, BlackColor);
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

	} else {
		LONG PreviousPoint = GetWindowLong(hColorPicker, GWL_USERDATA);
		if (PreviousPoint != NULL) {
			drawCross(MemoryDC, LOWORD(PreviousPoint) - 23 / 2, HIWORD(PreviousPoint) - 23 / 2, 23, 23, BlackColor);
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
		case WM_LBUTTONDOWN:
		{
			RedrawWindow(hColorPicker, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
			return 0;
		}
	}

	return DefWindowProc(hColorPicker, Msg, wParam, lParam);


}
#pragma endregion
