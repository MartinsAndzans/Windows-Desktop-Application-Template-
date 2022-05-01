#ifndef _DRAW_
#define _DRAW_

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#include <ciso646>
#include <Windows.h>
#include <d2d1.h>

class Draw {

public:

	/// <summary>
	/// * Predefined Colors *
	/// </summary>
	enum Colors {
		WhiteColor = RGB(255, 255, 255),
		BlackColor = RGB(0, 0, 0),
		OrangeColor = RGB(240, 190, 0),
		RedColor = RGB(255, 0, 0),
		BlueColor = RGB(0, 0, 255),
		GreenColor = RGB(0, 255, 0),
		DarkGreenColor = RGB(0, 145, 0)
	};

	/// <summary>
	/// This Function Draws Rectangle
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="Rectangle">- Rectangle</param>
	static VOID drawRectangle(HDC hdc, RECT &Rectangle) {

		// TOP
		MoveToEx(hdc, Rectangle.left, Rectangle.top, NULL);
		LineTo(hdc, Rectangle.right, Rectangle.top);
		// BOTTOM
		MoveToEx(hdc, Rectangle.left, Rectangle.bottom, NULL);
		LineTo(hdc, Rectangle.right, Rectangle.bottom);
		// LEFT
		MoveToEx(hdc, Rectangle.left, Rectangle.top, NULL);
		LineTo(hdc, Rectangle.left, Rectangle.bottom);
		// RIGHT
		MoveToEx(hdc, Rectangle.right, Rectangle.top, NULL);
		LineTo(hdc, Rectangle.right, Rectangle.bottom);

	}

	/// <summary>
	/// This Function Draws Dashed Rectangle
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="Rectangle">- Rectangle</param>
	/// <param name="Width">- Width</param>
	/// <param name="Color">- Color</param>
	static VOID drawDashedRectangle(HDC hdc, RECT &Rectangle, UINT Width, COLORREF Color = Colors::BlackColor) {

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

	/// <summary>
	/// This Function Draws Bitmap From File
	/// </summary>
	/// <param name="DestinationDC">- Device Context</param>
	/// <param name="DestinationRectangle">- Where Draw Bitmap</param>
	/// <param name="FilePath">- File Path - Supported Image Formats : "*.bmp; *.ico; *.cur"</param>
	/// <param name="BitmapType">- IMAGE_BITMAP || IMAGE_ICON || IMAGE_CURSOR</param>
	/// <param name="DrawMethod">- Draw Method - [EXAMPLE - SRCCOPY]</param>
	/// <returns>If Succeeded returns TRUE, but If not returns FALSE</returns>
	static VOID drawBitmapFromFile(HDC DestinationDC, RECT &DestinationRectangle, LPCWSTR FilePath, UINT BitmapType = IMAGE_BITMAP, DWORD DrawMethod = SRCCOPY) {

		HDC BitmapDC = CreateCompatibleDC(DestinationDC);
		HBITMAP Bitmap = (HBITMAP)LoadImage(NULL, FilePath, BitmapType, 0, 0, LR_LOADFROMFILE | LR_VGACOLOR);

		SelectObject(BitmapDC, Bitmap);
		BitBlt(DestinationDC, DestinationRectangle.left, DestinationRectangle.top, DestinationRectangle.right, DestinationRectangle.bottom, BitmapDC, 0, 0, DrawMethod);

		DeleteDC(BitmapDC);
		DeleteObject(Bitmap);

	}

	/// <summary>
	/// This Function Draws Rectangle
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="WIDTH">- Width</param>
	/// <param name="HEIGTH">- Height</param>
	/// <param name="BorderWidth">- Border Width</param>
	/// <param name="Color">- Rectangle Color</param>
	/// <param name="BorderColor">- Border Color</param>
	static VOID drawRectangle(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH = 120, INT HEIGTH = 40, INT BorderWidth = 2,
		COLORREF RectangleColor = Colors::BlueColor, COLORREF BorderColor = Colors::BlackColor) {

		// Border
		for (SHORT W = 0; W < BorderWidth; W++) {
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

		// Rectangle
		for (INT X = COORD_X + BorderWidth; X <= COORD_X + WIDTH - BorderWidth; X++) {
			for (INT Y = COORD_Y + BorderWidth; Y <= COORD_Y + HEIGTH - BorderWidth; Y++) {
				SetPixel(hdc, X, Y, RectangleColor);
			}
		}

	}

	/// <summary>
	/// This Function Fills Rectangle [50/50 Pixels] - [50% Opacity]
	/// <para>|X| |X| |X|</para>
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="Rectangle">- Rectangle</param>
	/// <param name="Color">- Color</param>
	static VOID FillRectOpacity50(HDC hdc, RECT &Rectangle, COLORREF Color = Colors::BlackColor) {

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

	/// <summary>
	/// This Function Draws Triangle
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="X1">- X1 Coordinate</param>
	/// <param name="Y1">- Y1 Coordinate</param>
	/// <param name="X2">- Y2 Coordinate</param>
	/// <param name="Y2">- Y2 Coordinate</param>
	/// <param name="X3">- X3 Coordinate</param>
	/// <param name="Y3">- Y3 Coordinate</param>
	/// <param name="Color">- Color</param>
	static VOID drawTriangle(HDC hdc, INT X1, INT Y1, INT X2, INT Y2, INT X3, INT Y3, COLORREF Color = Colors::BlueColor) {

		SetDCPenColor(hdc, Color);
		SetDCBrushColor(hdc, Color);

		HPEN PrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(DC_PEN));
		HBRUSH PrevBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(DC_BRUSH));

		SetPolyFillMode(hdc, ALTERNATE);

		POINT First = { X1, Y1 };
		POINT Second = { X2, Y2 };
		POINT Third = { X3, Y3 };

		POINT Vertices[] = { First, Second, Third };

		Polygon(hdc, Vertices, ARRAYSIZE(Vertices));

		SelectObject(hdc, PrevPen);
		SelectObject(hdc, PrevBrush);

	}

	/// <summary>
	/// This Function Draws Arrow
	/// <para>..+--+..</para>
	/// <para>..|..|..</para>
	/// <para>..|..|..</para>
	/// <para>+-+--+-+</para>
	/// <para>.\..../.</para>
	/// <para>..\../..</para>
	/// <para>...\/...</para>
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="WIDTH">- Width</param>
	/// <param name="HEIGHT">- Height</param>
	/// <param name="Color">- Color</param>
	static VOID drawArrow(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH = 100, INT HEIGHT = 100, COLORREF Color = Colors::BlackColor) {

		CONST SHORT Proportion = 3;
		INT XCELL = WIDTH / Proportion, YCELL = HEIGHT / Proportion;

		SetDCPenColor(hdc, Color);
		SetDCBrushColor(hdc, Color);

		HPEN PrevPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(DC_PEN));
		HBRUSH PrevBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(DC_BRUSH));

		SetPolyFillMode(hdc, ALTERNATE);

		POINT First = { COORD_X + XCELL, COORD_Y }; // --+----
		POINT Second = { COORD_X + XCELL * 2, COORD_Y }; // ----+--
		POINT Third = { COORD_X + XCELL, COORD_Y + YCELL * 2 }; // --+----
		POINT Fourth = { COORD_X + XCELL * 2, COORD_Y + YCELL * 2 }; // ----+--

		POINT Fifth = { COORD_X, COORD_Y + YCELL * 2 }; // +------
		POINT Sixth = { COORD_X + WIDTH, COORD_Y + YCELL * 2 }; //------+
		POINT Seventh = { COORD_X + WIDTH / 2, COORD_Y + HEIGHT }; // ---+---

		POINT Vertices[] = { Third, First, Second, Fourth, Sixth, Seventh, Fifth };

		Polygon(hdc, Vertices, ARRAYSIZE(Vertices));

		SelectObject(hdc, PrevPen);
		SelectObject(hdc, PrevBrush);

	}

	/// <summary>
	/// This Function Draws Small Gradient
	/// <para>Size - Width = 420 / Height = 40</para>
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="BorderWidth">- Border Width</param>
	/// <param name="BorderColor">- Border Color</param>
	/// <returns>Returns Border Width</returns>
	static SHORT drawSmallGradient(HDC hdc, INT COORD_X, INT COORD_Y, SHORT BorderWidth = 2, COLORREF BorderColor = Colors::BlackColor) {

		CONST SHORT WIDTH = 420, HEIGTH = 40;

		// Border
		for (SHORT W = 0; W < BorderWidth; W++) {
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
		BYTE DONE = 0;

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

		return BorderWidth;

	}

	/// <summary>
	/// This Function Draws Large Gradient
	/// <para>Size - Width = 420 / Height = 100</para>
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="BorderWidth">- Border Width</param>
	/// <param name="BorderColor">- Border Color</param>
	/// <returns>Returns Border Width</returns>
	static SHORT drawLargeGradient(HDC hdc, INT COORD_X, INT COORD_Y, SHORT BorderWidth = 2, COLORREF BorderColor = Colors::BlackColor) {

		CONST SHORT WIDTH = 420, HEIGTH = 100;

		// Border
		for (SHORT W = 0; W < BorderWidth; W++) {
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
		BYTE DONE = 0;

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

		return BorderWidth;

	}

	/// <summary>
	/// This Function Draws Animation Frame
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="WIDTH">- Width</param>
	/// <param name="HEIGTH">- Height</param>
	/// <param name="Symbol">- Symbol</param>
	/// <param name="Proportion">- Proportion</param>
	/// <param name="SymbolColor">- Symbol Color</param>
	static VOID drawAnimationFrame(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH = 100, INT HEIGHT = 100, CONST CHAR Symbol = '+', UINT Proportion = 4, COLORREF SymbolColor = Colors::WhiteColor) {

		if (WIDTH != 0 and HEIGHT != 0) {

			SIZE size = { 0 };
			SYSTEMTIME st = { 0 };
			INT XCELL = WIDTH / Proportion, YCELL = HEIGHT / Proportion;

			COLORREF PreviousColor = GetTextColor(hdc);
			SetTextColor(hdc, SymbolColor);

			GetTextExtentPointA(hdc, &Symbol, 1, &size); // Symbol Size In Pixels

			GetSystemTime(&st); // Gets System Time
			srand(st.wMilliseconds); // Random Sead

			for (UINT Y = 0; Y < Proportion; Y++) {
				for (UINT X = 0; X < Proportion; X++) {

					INT SYMBOL_X = rand() % XCELL; // Random Number From 0 To (XCELL - 1)
					INT SYMBOL_Y = rand() % YCELL; // Random Number From 0 To (YCELL - 1)

					TextOutA(hdc, COORD_X + (SYMBOL_X + XCELL * X) - size.cx / 2, COORD_Y + (SYMBOL_Y + YCELL * Y) - size.cy / 2, &Symbol, 1);

				}
			}

			SetTextColor(hdc, PreviousColor);

		} else {
			OutputDebugString(L"ERROR [Draw::drawAnimationFrame] - Width or Height Must be non Zero Value!\r\n");
		}

	}

	/// <summary>
	/// This Function Draws Cross
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="WIDTH">- Width</param>
	/// <param name="HEIGTH">- Height</param>
	/// <param name="CrossColor">- Cross Color</param>
	static VOID drawCross(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH = 23, INT HEIGHT = 23, COLORREF CrossColor = Colors::BlackColor) {

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

};

class DirectX2D {

private:

	ID2D1Factory *D2D1Factory = nullptr;
	ID2D1DCRenderTarget *D2D1DCRenderTarget = nullptr;

public:

	typedef ID2D1Factory* LPID2D1Factory;
	typedef ID2D1DCRenderTarget* LPID2D1DCRenderTarget;

	HRESULT D2D1ErrorCode = S_OK; // DirectX2D Error Code Output

	DirectX2D() {
		D2D1ErrorCode = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &D2D1Factory);
	}

	HRESULT D2D1CreateDCRenderTarget(HDC RenderDC, RECT RenderRect, D2D1_RENDER_TARGET_TYPE type = D2D1_RENDER_TARGET_TYPE_HARDWARE) {
		D2D1_RENDER_TARGET_PROPERTIES D2D1RenderTargetProperties = D2D1::RenderTargetProperties(type,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
		D2D1ErrorCode = D2D1Factory->CreateDCRenderTarget(&D2D1RenderTargetProperties, &D2D1DCRenderTarget);
		D2D1DCRenderTarget->BindDC(RenderDC, &RenderRect);
	}

	LPID2D1DCRenderTarget GetD2D1DCRenderTarget() CONST {
		return D2D1DCRenderTarget;
	}

	~DirectX2D() {
		D2D1Factory->Release();
	}


};

#endif // _DRAW_
