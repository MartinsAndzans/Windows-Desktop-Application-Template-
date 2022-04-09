#pragma once

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#ifndef _DRAW_
#define _DRAW_

#include <Windows.h>
#include <string>

#ifndef WHITE_COLOR
#define WHITE_COLOR RGB(255, 255, 255)
#endif
#ifndef BLACK_COLOR
#define BLACK_COLOR RGB(0, 0, 0)
#endif
#ifndef ORANGE_COLOR
#define ORANGE_COLOR RGB(214, 152, 45)
#endif
#ifndef RED_COLOR
#define RED_COLOR RGB(238, 20, 20)
#endif
#ifndef BLUE_COLOR
#define BLUE_COLOR RGB(40, 34, 214)
#endif
#ifndef GREEN_COLOR
#define GREEN_COLOR RGB(45, 125, 15)
#endif

class Draw {

public:


	static VOID drawRectangle(HDC hdc, RECT rect) {

		/// <summary>
		/// This Function Draws Rectangle
		/// </summary>
		/// <param name="hdc">Device Context</param>
		/// <param name="rect">Rectangle</param>

		MoveToEx(hdc, rect.left, rect.top, NULL);
		LineTo(hdc, rect.right, rect.top);

		MoveToEx(hdc, rect.left, rect.top, NULL);
		LineTo(hdc, rect.left, rect.bottom);

		MoveToEx(hdc, rect.left, rect.bottom, NULL);
		LineTo(hdc, rect.right, rect.bottom);

		MoveToEx(hdc, rect.right, rect.top, NULL);
		LineTo(hdc, rect.right, rect.bottom);

	}

	static VOID drawDashedRectangle(HDC hdc, RECT &Rectangle, SIZE_T Width, COLORREF Color) {

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

	static BOOL drawBitmapFromFile(HDC DestinationDC, RECT &DestinationRectangle, std::string FilePath, UINT BitmapType = IMAGE_BITMAP, DWORD DrawMethod = SRCCOPY) {

		HDC BitmapDC = CreateCompatibleDC(DestinationDC);
		if (BitmapDC == NULL) { return FALSE; }
		HBITMAP Bitmap = (HBITMAP)LoadImageA(NULL, FilePath.c_str(), BitmapType, 0, 0, LR_LOADFROMFILE | LR_VGACOLOR);
		if (Bitmap == NULL) { return FALSE; }

		SelectObject(BitmapDC, Bitmap);
		BitBlt(DestinationDC, DestinationRectangle.left, DestinationRectangle.top, DestinationRectangle.right, DestinationRectangle.bottom, BitmapDC, 0, 0, DrawMethod);

		DeleteDC(BitmapDC);
		DeleteObject(Bitmap);

		return TRUE;

	}

	static VOID drawRectangle(HDC hdc, INT X = 0, INT Y = 0, INT W = 120, INT H = 40, INT BorderWidth = 2, COLORREF RectangleColor = WHITE_COLOR, COLORREF BorderColor = BLACK_COLOR) {

		/// <summary>
		/// This Function Draws Rectangle
		/// </summary>
		/// <param name="hdc">Device Context</param>
		/// <param name="X">X Coordinate</param>
		/// <param name="Y">Y Coordinate</param>
		/// <param name="W">Width</param>
		/// <param name="H">Height</param>
		/// <param name="BorderWidth">Border Width</param>
		/// <param name="Color">Rectangle Color</param>
		/// <param name="BorderColor">Border Color</param>

		INT XS = 0, XE = W, YS = 0, YE = H;

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

		// Rectangle

		for (XS = BorderWidth; XS <= XE - BorderWidth; XS++) {
			for (YS = BorderWidth; YS <= YE - BorderWidth; YS++) {
				SetPixel(hdc, X + XS, Y + YS, RectangleColor);
			}
		}

	}

	static VOID FillRectOpacity50(HDC hdc, RECT &Rectangle, COLORREF Color) {

		BOOL DRAWPIXEL; // TRUE = |X| - FALSE = | |
		for (int x = Rectangle.left; x <= Rectangle.right; x++) {
			(x % 2 == NULL) ? DRAWPIXEL = TRUE : DRAWPIXEL = FALSE; // 0, 2, 4, 5 = TRUE - 1, 3, 5, 7 = FALSE 
			for (int y = Rectangle.top; y <= Rectangle.bottom; y++) {
				if (DRAWPIXEL == TRUE) {
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

	static VOID drawCube(HDC hdc, INT X = 0, INT Y = 0, INT W = 40, INT H = 40, INT Z = 20) {

		while (Z >= 0) {
			drawRectangle(hdc, X, Y, W, H);
			X--, Y++, Z--;
		}

	}

	static VOID drawRomb(HDC hdc, int x = 0, int y = 0, int w = 60, int h = 60, COLORREF color = BLACK_COLOR) {

		/// <summary>
		/// This Function Draws Romb
		/// </summary>
		/// <param name="hdc">Device Context</param>
		/// <param name="x">X Coordinate</param>
		/// <param name="y">Y Coordinate</param>
		/// <param name="w">Width</param>
		/// <param name="h">Height</param>
		/// <param name="color">Romb Color</param>

		int offset = 0, xs = 0, xe = w, ys = 0, ye = h;

		for (int ys = 0; ys <= ye; ys++) {
			if (ys >= h / 2) {
				for (int xs = w / 2 - offset; xs <= w / 2 + offset; xs++) {
					SetPixel(hdc, x + xs, y + ys, color);
				}
				offset--;
				continue;
			}
			for (int xs = w / 2 - offset; xs <= w / 2 + offset; xs++) {
				SetPixel(hdc, x + xs, y + ys, color);
			}
			offset++;
		}

	}

	static VOID drawArrow(HDC hdc, INT X, INT Y, INT W, INT H, COLORREF Color) {

		/// <summary>
		/// This Function Draws Arrow
		/// </summary>
		/// <param name="hdc">Device Context</param>
		/// <param name="x">X Coordinate</param>
		/// <param name="y">Y Coordinate</param>
		/// <param name="w">Width</param>
		/// <param name="h">Height</param>

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

			for (XS = XCELL; XS <= XCELL * 2; XS++) {
				SetPixel(hdc, X + XS, Y + YS, Color);
			}

		}

	}

	static INT drawGradientSmall(HDC hdc, INT X = 0, INT Y = 0, INT W = 420, INT H = 40, INT BorderWidth = 2, COLORREF BorderColor = BLACK_COLOR) {

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
			for (YS = BorderWidth; YS <= YE - BorderWidth; YS++) {
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

	static INT drawGradientLarge(HDC hdc, INT X = 0, INT Y = 0, INT W = 420, INT H = 100, INT BorderWidth = 2, COLORREF BorderColor = BLACK_COLOR) {

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
			for (YS = BorderWidth; YS <= YE - BorderWidth; YS++) {
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

	static VOID drawMatrix(HDC hdc, RECT MatrixDimensions) {



	}

	static VOID drawStars(HDC hdc, INT X, INT Y, INT W, INT H, COLORREF StarColor, const wchar_t StarSymbol[], INT Proportion) {

		/// <summary>
		/// This Function Draws Stars
		/// </summary>
		/// <param name="hdc">Device Context</param>
		/// <param name="X">X Coordinate</param>
		/// <param name="Y">Y Coordinate</param>
		/// <param name="W">Width</param>
		/// <param name="H">Height</param>
		/// <param name="StarColor">Star Color</param>
		/// <param name="StarSymbol">Star Symbol</param>
		/// <param name="Proportion">Proportion</param>

		if (W && H != 0) {

			SIZE size = { 0 };
			SYSTEMTIME st = { 0 };
			INT CELL = 0;
			INT XS = 0, XE = W, YS = 0, YE = H, XCELL = W / Proportion, YCELL = H / Proportion;

			COLORREF DefaultColor = GetTextColor(hdc);
			SetTextColor(hdc, StarColor);

			GetTextExtentPoint(hdc, StarSymbol, lstrlenW(StarSymbol), &size);

			GetSystemTime(&st);
			srand(st.wMilliseconds);

			for (int i = 0; i < Proportion * Proportion; i++) {

				INT STARX = rand() % XCELL + XS; // XS - (XS + XCELL)
				INT STARY = rand() % YCELL + YS; // YS - (YS + YCELL)

				TextOut(hdc, X + STARX - size.cx / 2, Y + STARY - size.cy / 2, StarSymbol, lstrlenW(StarSymbol));

				///////////////////////////
				//// -->               ////
				//// +---+---+---+---+ ////
				//// | 0 | 1 | 2 | 3 | ////
				//// +---+---+---+---+ ////
				///////////////////////////

				XS = XS + XCELL;
				CELL++;

				/////////////////
				//// +---+ | ////
				//// | 0 | | ////
				//// +---+ V ////
				//// | 1 |   ////
				//// +---+   ////
				//// | 2 |   ////
				//// +---+   ////
				//// | 3 |   ////
				//// +---+   ////
				/////////////////

				if (CELL == Proportion) {
					CELL = 0;
					XS = 0;
					YS = YS + YCELL;
				}

			}

			SetTextColor(hdc, DefaultColor);

		}
		else {

			OutputDebugString(L"ERROR [Draw::drawStars] - Width or Height Must be non Zero Value!\r\n");

		}

	}

	static VOID drawCross(HDC hdc, INT X = 0, INT Y = 0, INT W = 23, INT H = 23, COLORREF CrossColor = BLACK_COLOR) {

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

				if (YS > H / 2 - YCELL / 2 && YS < H / 2 + YCELL / 2) {
					continue;
				}

				SetPixel(hdc, X + XS - 1, Y + YS, CrossColor);
				SetPixel(hdc, X + XS, Y + YS, CrossColor);
				SetPixel(hdc, X + XS + 1, Y + YS, CrossColor);

			}

		}
		else {

			OutputDebugString(L"ERROR [Draw::drawCross] - Width or Height Must be Odd Number Value!\r\n");

		}

	}

};

#endif
