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

	/// <summary>
	/// This Function Draws Bitmap Image From File
	/// </summary>
	/// <param name="DestinationDC">- Device Context</param>
	/// <param name="DestinationRectangle">- Where Draw Bitmap</param>
	/// <param name="FilePath">- File Path - [EXAMPLE - "Image.bmp/.ico/.cur"]</param>
	/// <param name="BitmapType">- IMAGE_BITMAP || IMAGE_ICON || IMAGE_CURSOR</param>
	/// <param name="DrawMethod">- Draw Method - [EXAMPLE - SRCCOPY]</param>
	/// <returns>If Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL drawBitmapFromFile(HDC DestinationDC, RECT &DestinationRectangle, std::string FilePath, UINT BitmapType = IMAGE_BITMAP, DWORD DrawMethod = SRCCOPY) {

		HDC BitmapDC = CreateCompatibleDC(DestinationDC);
		if (BitmapDC == NULL) return FALSE;
		HBITMAP Bitmap = (HBITMAP)LoadImageA(NULL, FilePath.c_str(), BitmapType, 0, 0, LR_LOADFROMFILE | LR_VGACOLOR);
		if (Bitmap == NULL) return FALSE;

		SelectObject(BitmapDC, Bitmap);
		BitBlt(DestinationDC, DestinationRectangle.left, DestinationRectangle.top, DestinationRectangle.right, DestinationRectangle.bottom, BitmapDC, 0, 0, DrawMethod);

		DeleteDC(BitmapDC);
		DeleteObject(Bitmap);

		return TRUE;

	}

	/// <summary>
	/// This Function Draws Rectangle
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="W">- Width</param>
	/// <param name="H">- Height</param>
	/// <param name="BorderWidth">- Border Width</param>
	/// <param name="Color">- Rectangle Color</param>
	/// <param name="BorderColor">- Border Color</param>
	static VOID drawRectangle(HDC hdc, INT COORD_X = 0, INT COORD_Y = 0, INT W = 120, INT H = 40, INT BorderWidth = 2,
		COLORREF RectangleColor = WHITE_COLOR, COLORREF BorderColor = BLACK_COLOR) {

		INT XS = 0, XE = W, YS = 0, YE = H;

		// Border
		for (SHORT Counter = 0; Counter < BorderWidth; Counter++) {
			// Left and Right
			for (INT Y = COORD_Y; Y <= COORD_Y + W; Y++) {
				SetPixel(hdc, COORD_X + Counter, Y, BorderColor);
				SetPixel(hdc, COORD_X + H - Counter, Y, BorderColor);
			}
			// Up and Down
			for (INT X = COORD_X; X <= COORD_X + W; X++) {
				SetPixel(hdc, X, COORD_Y + Counter, BorderColor);
				SetPixel(hdc, X, COORD_Y + H - Counter, BorderColor);
			}
		}

		// Rectangle
		for (INT X = BorderWidth; X <= COORD_X + W - BorderWidth; X++) {
			for (INT Y = BorderWidth; Y <= COORD_Y + H - BorderWidth; Y++) {
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
	static VOID FillRectOpacity50(HDC hdc, RECT &Rectangle, COLORREF Color) {

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

	/// <summary>
	/// This Function Draws Romb
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="W">- Width</param>
	/// <param name="H">- Height</param>
	/// <param name="Color">- Romb Color</param>
	static VOID drawRomb(HDC hdc, INT COORD_X = 0, INT COORD_Y = 0, INT W = 60, INT H = 60, COLORREF Color = BLACK_COLOR) {

		INT OFFSET = 0;

		for (INT Y = COORD_Y; Y <= COORD_Y + H; Y++) {
			if (Y < H / 2) OFFSET++;
			else OFFSET--;
			for (INT X = COORD_X + W / 2 - OFFSET; X <= COORD_X + W / 2 + OFFSET; X++) {
				SetPixel(hdc, X, Y, Color);
			}
		}

	}

	/// <summary>
	/// This Function Draws Arrow
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="X">- X Coordinate</param>
	/// <param name="Y">- Y Coordinate</param>
	/// <param name="W">- Width</param>
	/// <param name="H">- Height</param>
	static VOID drawArrow(HDC hdc, INT X, INT Y, INT W, INT H, COLORREF Color) {

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

	/// <summary>
	/// This Function Draws Small Gradient
	/// <para>Size - Width = 420 Height = 40</para>
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="BorderWidth">- Border Width</param>
	/// <param name="BorderColor">- Border Color</param>
	/// <returns>Returns Border Width</returns>
	static SHORT drawGradientSmall(HDC hdc, INT COORD_X, INT COORD_Y, SHORT BorderWidth = 2, COLORREF BorderColor = BLACK_COLOR) {

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

	/// <summary>
	/// This Function Draws Large Gradient
	/// <para>Size - Width = 420 Height = 100</para>
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="BorderWidth">- Border Width</param>
	/// <param name="BorderColor">- Border Color</param>
	/// <returns>Returns Border Width</returns>
	static SHORT drawGradientLarge(HDC hdc, INT COORD_X, INT COORD_Y, SHORT BorderWidth, COLORREF BorderColor) {

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

	/// <summary>
	/// This Function Draws Stars
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="Rectangle">- Drawing Rectangle</param>
	/// <param name="StarColor">- Star Color</param>
	/// <param name="StarSymbol">- Star Symbol</param>
	/// <param name="Proportion">- Proportion</param>
	static VOID drawStars(HDC hdc, RECT &Rectangle, COLORREF StarColor, CONST CHAR StarSymbol[], UINT Proportion) {

		if (Proportion == 0) Proportion++;

		if (Rectangle.right != 0 && Rectangle.bottom != 0) {

			SIZE size = { 0 };
			SYSTEMTIME st = { 0 };
			INT XS = 0, YS = 0, XCELL = (Rectangle.right - Rectangle.left) / Proportion, YCELL = (Rectangle.bottom - Rectangle.top) / Proportion;

			COLORREF PreviousColor = GetTextColor(hdc);
			SetTextColor(hdc, StarColor);

			GetTextExtentPointA(hdc, StarSymbol, (int)strlen(StarSymbol), &size);

			GetSystemTime(&st);
			srand(st.wMilliseconds);

			for (UINT i = 0; i < Proportion * Proportion; i++) {

				INT STARX = rand() % XCELL + XS; // XS - (XS + XCELL)
				INT STARY = rand() % YCELL + YS; // YS - (YS + YCELL)

				TextOutA(hdc, Rectangle.left + STARX - size.cx / 2, Rectangle.top + STARY - size.cy / 2, StarSymbol, (int)strlen(StarSymbol));

				///////////////////////////
				//// -->               ////
				//// +---+---+---+---+ ////
				//// | 0 | 1 | 2 | 3 | ////
				//// +---+---+---+---+ ////
				///////////////////////////

				XS = XS + XCELL;

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

				if (XS == XCELL * Proportion) {
					XS = 0;
					YS = YS + YCELL;
				}

			}

			SetTextColor(hdc, PreviousColor);

		}
		else {

			OutputDebugString(L"ERROR [AnimationStars::drawStars] - Width or Height Must be non Zero Value!\r\n");

		}

	}

	/// <summary>
	/// This Function Draws Cross
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="X">- X Coordinate</param>
	/// <param name="Y">- Y Coordinate</param>
	/// <param name="W">- Width</param>
	/// <param name="H">- Height</param>
	/// <param name="CrossColor">- Cross Color</param>
	static VOID drawCross(HDC hdc, INT X = 0, INT Y = 0, INT W = 23, INT H = 23, COLORREF CrossColor = BLACK_COLOR) {

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
