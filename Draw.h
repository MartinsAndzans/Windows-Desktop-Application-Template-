#ifndef _DRAW_
#define _DRAW_

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#include <ciso646>
#include <assert.h>
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
	/// <param name="hdc">Device Context</param>
	/// <param name="Rectangle">Rectangle</param>
	static VOID drawRectangle(HDC hdc, CONST RECT &Rectangle) {

		POINT RectangleVertices[] = {
			{ Rectangle.left, Rectangle.top }, // First Point
			{ Rectangle.right, Rectangle.top }, // Second Point
			{ Rectangle.right, Rectangle.bottom }, // Third Point
			{ Rectangle.left, Rectangle.bottom }, // Fourth Point
			{ Rectangle.left, Rectangle.top } // Fifth Point
		};

		Polyline(hdc, RectangleVertices, ARRAYSIZE(RectangleVertices));

	}

	/// <summary>
	/// This Function Draws Rectangle
	/// </summary>
	/// <param name="hdc">Device Context</param>
	/// <param name="COORD_X">X Coordinate</param>
	/// <param name="COORD_Y">Y Coordinate</param>
	/// <param name="WIDTH">Width</param>
	/// <param name="HEIGTH">Height</param>
	/// <param name="BorderWidth">Border Width</param>
	/// <param name="Color">Rectangle Color</param>
	/// <param name="BorderColor">Border Color</param>
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
	/// This Function Fill Rectangle [50/50 Pixels] - [50% Opacity]
	/// </summary>
	/// <param name="hdc">Device Context</param>
	/// <param name="Rectangle">Rectangle</param>
	/// <param name="Color">Color</param>
	static VOID FillRectOpacity50(HDC hdc, CONST RECT &Rectangle, COLORREF Color = Colors::BlackColor) {

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
	/// This Function Draws Bitmap From File
	/// </summary>
	/// <param name="hdc">Device Context</param>
	/// <param name="Rectangle">Where Draw Bitmap</param>
	/// <param name="FilePath">File Path -/Supported Image Formats : "*.bmp; *.ico; *.cur"/-</param>
	/// <param name="BitmapType">Bitmap Type</param>
	/// <param name="DrawMethod">Draw Method</param>
	/// <returns>If Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL drawBitmapFromFile(HDC hdc, RECT &Rectangle, LPCWSTR FilePath, UINT BitmapType = IMAGE_BITMAP, DWORD DrawMethod = SRCCOPY) {

		HDC BitmapDC = CreateCompatibleDC(hdc);
		HBITMAP Bitmap = (HBITMAP)LoadImage(NULL, FilePath, BitmapType, 0, 0, LR_LOADFROMFILE | LR_VGACOLOR);
		if (Bitmap == NULL) {
			OutputDebugString(L"\'[Draw::drawBitmapFromFile] - \"Image File not Found!\"\'\r\n");
			DeleteDC(BitmapDC);
			return FALSE;
		}

		SelectObject(BitmapDC, Bitmap);
		BitBlt(hdc, Rectangle.left, Rectangle.top, Rectangle.right, Rectangle.bottom, BitmapDC, 0, 0, DrawMethod);

		DeleteDC(BitmapDC);
		DeleteObject(Bitmap);

		return TRUE;

	}

	/// <summary>
	/// This Function Draws Triangle
	/// </summary>
	/// <param name="hdc">Device Context</param>
	/// <param name="V1">First Vertice</param>
	/// <param name="V2">Second Vertice</param>
	/// <param name="V3">Third Vertice</param>
	/// <param name="FillMode">Fill Mode ALTERNATE or WINDING</param>
	/// <param name="Color">Color</param>
	static VOID FillTriangle(HDC hdc, POINT V1, POINT V2, POINT V3, INT FillMode = ALTERNATE, COLORREF Color = Colors::BlueColor) {

		SetDCPenColor(hdc, Color);
		SetDCBrushColor(hdc, Color);

		HPEN PreviousPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(DC_PEN));
		HBRUSH PreviousBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(DC_BRUSH));

		SetPolyFillMode(hdc, FillMode);

		POINT TriangleVertices[] = {
			{ V1.x, V1.y },
			{ V2.x, V2.y },
			{ V3.x, V3.y }
		};

		Polygon(hdc, TriangleVertices, ARRAYSIZE(TriangleVertices));

		SelectObject(hdc, PreviousPen);
		SelectObject(hdc, PreviousBrush);

	}

	/// <summary>
	/// This Function Draws Arrow
	/// </summary>
	static VOID FillArrow(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH = 60, INT HEIGHT = 100, INT FillMode = ALTERNATE, COLORREF Color = Colors::BlackColor) {

		CONST SHORT Proportion = 3;
		INT XCELL = WIDTH / Proportion, YCELL = HEIGHT / Proportion;

		SetDCPenColor(hdc, Color);
		SetDCBrushColor(hdc, Color);

		HPEN PreviousPen = (HPEN)SelectObject(hdc, (HPEN)GetStockObject(DC_PEN));
		HBRUSH PreviousBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(DC_BRUSH));

		SetPolyFillMode(hdc, FillMode);

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

	/// <summary>
	/// This Function Draws Small Gradient
	/// <para>Size - Width = 420 / Height = 40</para>
	/// </summary>
	static VOID drawSmallGradient(HDC hdc, INT COORD_X, INT COORD_Y, COLORREF BorderColor = Colors::BlackColor) {

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

	/// <summary>
	/// This Function Draws Large Gradient
	/// <para>Size - Width = 420 / Height = 100</para>
	/// </summary>
	static VOID drawLargeGradient(HDC hdc, INT COORD_X, INT COORD_Y, COLORREF BorderColor = Colors::BlackColor) {

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

	/// <summary>
	/// This Function Draws Animation Frame
	/// </summary>
	static VOID drawAnimationFrame(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH = 100, INT HEIGHT = 100, CONST CHAR Symbol = '+', UINT Proportion = 4, COLORREF SymbolColor = Colors::WhiteColor) {

		if (WIDTH > 0 and HEIGHT > 0) {

			SIZE size = { 0 };
			SYSTEMTIME st = { 0 };
			INT XCELL = WIDTH / Proportion, YCELL = HEIGHT / Proportion;

			COLORREF PreviousColor = GetTextColor(hdc);
			SetTextColor(hdc, SymbolColor);

			GetTextExtentPointA(hdc, &Symbol, 1, &size); // Symbol Size In Pixels

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
			OutputDebugString(L"ERROR [Draw::drawAnimationFrame] - Width or Height Must be non Zero Value!\r\n");
		}

	}

	/// <summary>
	/// This Function Draws Cross
	/// </summary>
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

struct Graphics {

private:

	ID2D1Factory *Factory = nullptr; // * DirectX2D Factory *
	ID2D1DCRenderTarget *RenderTarget = nullptr; // * DirectX2D GDI Compatible Render Target *

public:

	BOOL Init() {
		
		// * Creating DirectX2D Factory *
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory)))
			return FALSE;

		// * Creating DirectX2D GDI Compatible Render Target *
		D2D1_RENDER_TARGET_PROPERTIES RTP = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
			0.0F, 0.0F, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT);
		if (FAILED(Factory->CreateDCRenderTarget(&RTP, &RenderTarget)))
			return FALSE;

		return TRUE;

	}

	VOID BeginDraw(HDC RenderDC, CONST RECT &RenderRect) {
		// * Binding Device Context To DirectX2D GDI Compatible Render Target *
		RenderTarget->BindDC(RenderDC, &RenderRect);
		// * Begins Drawing *
		RenderTarget->BeginDraw();
	}

	VOID EndDraw() {
		// * Ends Drawing *
		RenderTarget->EndDraw();
	}

	BOOL DrawCircle(CONST D2D1_POINT_2F &CenterPoint, FLOAT Radius,
		D2D1_COLOR_F Color = D2D1::ColorF(D2D1::ColorF::Enum::DarkBlue), FLOAT BrushWidth = 2.0F) {

		ID2D1SolidColorBrush *SolidColorBrush = nullptr;
		if (SUCCEEDED(RenderTarget->CreateSolidColorBrush(Color, &SolidColorBrush))) {
			RenderTarget->DrawEllipse(D2D1::Ellipse(CenterPoint, Radius, Radius), SolidColorBrush, BrushWidth);
			SolidColorBrush->Release();
			return TRUE;
		}
		return FALSE;

	}

	BOOL DrawTriangle(CONST D2D1_POINT_2F &V1, CONST D2D1_POINT_2F &V2, CONST D2D1_POINT_2F &V3,
		D2D1_COLOR_F Color = D2D1::ColorF(D2D1::ColorF::Enum::DarkBlue), FLOAT BrushWidth = 2.0F) {

		ID2D1PathGeometry *PathGeometry = nullptr;
		if (FAILED(Factory->CreatePathGeometry(&PathGeometry)))
			return FALSE;

		ID2D1GeometrySink *GeometrySink = nullptr;
		if (FAILED(PathGeometry->Open(&GeometrySink))) {
			PathGeometry->Release();
			return FALSE;
		}

		GeometrySink->BeginFigure(V1, D2D1_FIGURE_BEGIN_FILLED);
		GeometrySink->AddLine(V2);
		GeometrySink->AddLine(V3);
		GeometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);
		GeometrySink->Close();
		GeometrySink->Release();

		ID2D1SolidColorBrush *SolidColorBrush = nullptr;
		if (SUCCEEDED(RenderTarget->CreateSolidColorBrush(Color, &SolidColorBrush))) {
			RenderTarget->DrawGeometry(PathGeometry, SolidColorBrush, BrushWidth);
			SolidColorBrush->Release();
			PathGeometry->Release();
			return TRUE;
		}

		return FALSE;

	}

	BOOL FillCircle(CONST D2D1_POINT_2F &CenterPoint, FLOAT Radius,
		D2D1_COLOR_F Color = D2D1::ColorF(D2D1::ColorF::Enum::DarkBlue)) {

		ID2D1SolidColorBrush *SolidColorBrush = nullptr;
		if (SUCCEEDED(RenderTarget->CreateSolidColorBrush(Color, &SolidColorBrush))) {
			RenderTarget->FillEllipse(D2D1::Ellipse(CenterPoint, Radius, Radius), SolidColorBrush);
			SolidColorBrush->Release();
			return TRUE;
		}
		return FALSE;

	}

	~Graphics() {

		if (Factory != nullptr) Factory->Release();
		if (RenderTarget != nullptr) RenderTarget->Release();

	}

};

#endif // _DRAW_
