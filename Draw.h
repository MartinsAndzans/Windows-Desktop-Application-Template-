#pragma once

#ifndef _DRAW_
#define _DRAW_

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#include <Windows.h>
#include <ciso646>

#define WHITE_COLOR RGB(255, 255, 255)
#define BLACK_COLOR RGB(0, 0, 0)
#define ORANGE_COLOR RGB(240, 190, 0)
#define RED_COLOR RGB(255, 0, 0)
#define BLUE_COLOR RGB(0, 0, 255)
#define GREEN_COLOR RGB(0, 255, 0)
#define DARK_GREEN_COLOR RGB(0, 145, 0)

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

		for (INT Counter = 0; Counter < Width; Counter++) {

			// TOP
			MoveToEx(hdc, Rectangle.left, Rectangle.top + Counter, NULL);
			LineTo(hdc, Rectangle.right, Rectangle.top + Counter);
			// BOTTOM
			MoveToEx(hdc, Rectangle.left, Rectangle.bottom - Counter, NULL);
			LineTo(hdc, Rectangle.right, Rectangle.bottom - Counter);
			// LEFT
			MoveToEx(hdc, Rectangle.left + Counter, Rectangle.top, NULL);
			LineTo(hdc, Rectangle.left + Counter, Rectangle.bottom);
			// RIGHT
			MoveToEx(hdc, Rectangle.right - Counter, Rectangle.top, NULL);
			LineTo(hdc, Rectangle.right - Counter, Rectangle.bottom);

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
	static VOID drawBitmapFromFile(HDC DestinationDC, RECT &DestinationRectangle, LPSTR FilePath, UINT BitmapType = IMAGE_BITMAP, DWORD DrawMethod = SRCCOPY) {

		HDC BitmapDC = CreateCompatibleDC(DestinationDC);
		HBITMAP Bitmap = (HBITMAP)LoadImageA(NULL, FilePath, BitmapType, 0, 0, LR_LOADFROMFILE | LR_VGACOLOR);

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
		COLORREF RectangleColor = WHITE_COLOR, COLORREF BorderColor = BLACK_COLOR) {

		// Border
		for (SHORT Counter = 0; Counter < BorderWidth; Counter++) {
			// Left and Right
			for (INT Y = COORD_Y; Y <= COORD_Y + HEIGTH; Y++) {
				SetPixel(hdc, COORD_X + Counter, Y, BorderColor);
				SetPixel(hdc, COORD_X + WIDTH - Counter, Y, BorderColor);
			}
			// Up and Down
			for (INT X = COORD_X; X <= COORD_X + WIDTH; X++) {
				SetPixel(hdc, X, COORD_Y + Counter, BorderColor);
				SetPixel(hdc, X, COORD_Y + HEIGTH - Counter, BorderColor);
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
	static VOID FillRectOpacity50(HDC hdc, RECT &Rectangle, COLORREF Color) {

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
	/// This Function Draws Romb
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="WIDTH">- Width</param>
	/// <param name="HEIGTH">- Height</param>
	/// <param name="Color">- Romb Color</param>
	static VOID drawRomb(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH = 60, INT HEIGTH = 60, COLORREF Color = BLACK_COLOR) {

		INT OFFSET = 0;

		for (INT Y = COORD_Y; Y <= COORD_Y + HEIGTH; Y++) {
			for (INT X = COORD_X + WIDTH / 2 - OFFSET; X <= COORD_X + WIDTH / 2 + OFFSET; X++) {
				SetPixel(hdc, X, Y, Color);
			}
			if (Y < COORD_Y + HEIGTH / 2) {
				OFFSET++;
			} else {
				OFFSET--;
			}
		}

	}

	/// <summary>
	/// This Function Draws Arrow
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="WIDTH">- Width</param>
	/// <param name="HEIGHT">- Height</param>
	/// <param name="Color">- Color</param>
	static VOID drawArrow(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH = 40, INT HEIGHT = 40, COLORREF Color = BLACK_COLOR) {

		CONST SHORT Proportion = 3;
		INT OFFSET = 0, XCELL = WIDTH / Proportion, YCELL = HEIGHT / Proportion;

		for (INT Y = COORD_Y; Y <= COORD_Y + HEIGHT; Y++) {

			if (Y > COORD_Y + YCELL * 2) {
				for (INT X = (COORD_X + XCELL / 2) + OFFSET; X <= (COORD_X + XCELL * 2 + XCELL / 2) - OFFSET; X++) // +----+
					SetPixel(hdc, X, Y, Color);                                                                    //  \  /  x 1
				OFFSET++;                                                                                          //   \/
			} else {
				for (INT X = COORD_X + XCELL; X <= COORD_X + XCELL * 2; X++) // |====|
					SetPixel(hdc, X, Y, Color);                              // |====| x 2
			}                                                                // |====|

		}

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
	static SHORT drawSmallGradient(HDC hdc, INT COORD_X, INT COORD_Y, SHORT BorderWidth = 2, COLORREF BorderColor = BLACK_COLOR) {

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
	static SHORT drawLargeGradient(HDC hdc, INT COORD_X, INT COORD_Y, SHORT BorderWidth = 2, COLORREF BorderColor = BLACK_COLOR) {

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
				if (Y < COORD_Y + Height / 2) {
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
	/// This Function Draws Frame
	/// </summary>
	/// <param name="hdc">- Device Context</param>
	/// <param name="Rectangle">- Drawing Rectangle</param>
	/// <param name="SymbolColor">- Symbol Color</param>
	/// <param name="Symbol">- Symbol</param>
	/// <param name="Proportion">- Proportion</param>
	static VOID drawFrame(HDC hdc, RECT& Rectangle, COLORREF SymbolColor = WHITE_COLOR, CONST CHAR Symbol = '+', UINT Proportion = 4) {

		if (Rectangle.right - Rectangle.left != 0 and Rectangle.bottom - Rectangle.top != 0) {

			SIZE size = { 0 };
			SYSTEMTIME st = { 0 };
			INT XS = 0, YS = 0, XCELL = (Rectangle.right - Rectangle.left) / Proportion, YCELL = (Rectangle.bottom - Rectangle.top) / Proportion;

			COLORREF PreviousColor = GetTextColor(hdc);
			SetTextColor(hdc, SymbolColor);

			GetTextExtentPointA(hdc, &Symbol, 1, &size); // Symbol Size In Pixels

			GetSystemTime(&st); // Gets System Time
			srand(st.wMilliseconds); // Random Sead

			for (UINT I = 0; I < Proportion * Proportion; I++) {

				INT SYMBOLX = rand() % XCELL + XS; // XS - (XS + XCELL)
				INT SYMBOLY = rand() % YCELL + YS; // YS - (YS + YCELL)

				TextOutA(hdc, Rectangle.left + SYMBOLX - size.cx / 2, Rectangle.top + SYMBOLY - size.cy / 2, &Symbol, 1);

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
	/// <param name="COORD_X">- X Coordinate</param>
	/// <param name="COORD_Y">- Y Coordinate</param>
	/// <param name="WIDTH">- Width</param>
	/// <param name="HEIGTH">- Height</param>
	/// <param name="CrossColor">- Cross Color</param>
	static VOID drawCross(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH = 23, INT HEIGHT = 23, COLORREF CrossColor = BLACK_COLOR) {

		if (WIDTH % 2 != NULL and HEIGHT % 2 != NULL) {

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

#endif
