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
			for (YS = 0 + BorderWidth; YS <= YE - BorderWidth; YS++) {
				SetPixel(hdc, X + XS, Y + YS, RectangleColor);
			}
		}

	}

	static VOID drawCube(HDC hdc, INT X = 100, INT Y = 100, INT W = 40, INT H = 40, INT Z = 20) {

		while (Z >= 0) {
			drawRectangle(hdc, X, Y, W, H);
			X--, Y++, Z--;
		}

	}

	static VOID drawRomb(HDC hdc, int x = 0, int y = 0, int w = 60, int h = 60, COLORREF color = BLACK_COLOR, bool alpha = true) {

		/// <summary>
		/// This Function Draws Romb
		/// 
		///    ^
		///   / \
		///  /	 \
		/// *     >
		///  \   /
		///   \ /
		///	   v
		/// 
		/// </summary>
		/// <param name="hdc">Device Context</param>
		/// <param name="x">X Coordinate</param>
		/// <param name="y">Y Coordinate</param>
		/// <param name="w">Width</param>
		/// <param name="h">Height</param>
		/// <param name="color">Romb Color</param>
		/// <param name="alpha">Alpha</param>

		int offset = 0, xb = x, yb = y, xl = x + w;

		for (int r = 0; r <= h; r++) {
			for (x = xb + offset; x <= xl; x++) {
				SetPixel(hdc, x, y, color);
				if (alpha == false) {
					SetPixel(hdc, x + 1, y, color);
				}
				y--;
			}
			xl++;
			offset = offset + 1;
			y = yb + offset;
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

	static VOID drawGrid(HDC hdc, INT X = 0, INT Y = 0, INT W = 80, INT H = 80, SIZE_T CellSize = 1, COLORREF GridFirstColor = WHITE_COLOR, COLORREF GridSecondColor = BLACK_COLOR) {

		INT XS = 0, XE = W, YS = 0, YE = H;
		COLORREF GridColor = GridFirstColor;

		for (YS = 0; YS <= YE; YS++) {
			for (XS = 0; XS <= XE; XS = XS++) {
				//for (int cell = 0; cell <= CellSize; cell++) {
					SetPixel(hdc, X + XS, Y + YS, GridColor);
				//}
				(GridColor == GridFirstColor) ? GridColor = GridSecondColor : GridColor = GridFirstColor;
			}
		}

	}

	static INT CustomButton(HDC hdc, POINT mousePosition, INT ID, HFONT Font,
		INT X = 0, INT Y = 0, INT W = 160, INT H = 60,
		INT BorderWidth = 2, std::wstring Caption = L"BUTTON", COLORREF BorderColor = BLACK_COLOR,
		COLORREF TextColor = BLACK_COLOR, COLORREF ButtonColor = WHITE_COLOR) {

		#ifndef MOUSE_HOVER
		#define MOUSE_HOVER 0
		#endif
		#ifndef MOUSE_CLICKED
		#define MOUSE_CLICKED 1
		#endif

		/// <summary>
		/// This Function Initialize Custom Button
		//////
		/// GLOBAL VARIABLES
		///
		/// POINT mousePosition;
		/// INT Event
		/// 
		/// WM_PAINT
		///
		/// Event = CustomButton(HDC hdc, POINT mousePosition);
		/// 
		/// WM_MOUSEMOVE
		/// 
		/// GetCursorPos(&mousePosition);
		/// ScreenToClient(hMainWindow, &mousePosition);
		/// 
		/// if (mousePosition.x >= X - 10 && mousePosition.y >= Y - 10 &&
		///	mousePosition.x <= X + W + 10 && mousePosition.y <= Y + H + 10) {
		///	RedrawWindow(hMainWindow, &MainWindowDimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);
		/// }
		//////
		/// </summary>
		/// <param name="hdc">Device Context</param>
		/// <param name="mousePosition">Mouse Position on Client Area</param>
		/// <param name="ID">Button Unique Identifire</param>
		/// <param name="X">X Coordinate</param>
		/// <param name="Y">Y Coordinate</param>
		/// <param name="W">Width</param>
		/// <param name="H">Height</param>
		/// <param name="BorderWidth">Border Width</param>
		/// <param name="Caption">Button Caption</param>
		/// <param name="BorderColor">Border Color</param>
		/// <param name="TextColor">Text Color</param>
		/// <param name="ButtonColor">Buttton Color</param>
		/// <returns>
		/// If Left Mouse Button Clicked Returns Event Code + Button Identifire, but If Mouse Hovers Over Button Returns Event Code + Button Identifire
		/// Event Codes:
		/// MOUSE_CLICKED Button Clicked
		/// MOUSE_HOVER Mouse Hovers Over Button
		/// </returns>

		SIZE size;
		INT ButtonSizeChange = 5;

		SetTextColor(hdc, TextColor);
		SelectObject(hdc, Font);

		if (GetAsyncKeyState(VK_LBUTTON) && mousePosition.x >= X + BorderWidth && mousePosition.y >= Y + BorderWidth &&
			mousePosition.x <= X + W - BorderWidth && mousePosition.y <= Y + H - BorderWidth) {
			drawRectangle(hdc, X, Y, W, H, BorderWidth, ButtonColor, BorderColor);
			GetTextExtentPoint(hdc, Caption.c_str(), (int)Caption.length(), &size);
			TextOut(hdc, X + W / 2 - size.cx / 2, Y + H / 2 - size.cy / 2, Caption.c_str(), (int)Caption.length());
			return MOUSE_CLICKED + ID;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && mousePosition.x >= X + BorderWidth && mousePosition.y >= Y + BorderWidth &&
			mousePosition.x <= X + W - BorderWidth && mousePosition.y <= Y + H - BorderWidth) {
			drawRectangle(hdc, X - ButtonSizeChange, Y - ButtonSizeChange, W + ButtonSizeChange * 2, H + ButtonSizeChange * 2, BorderWidth, ButtonColor, BorderColor);
			GetTextExtentPoint(hdc, Caption.c_str(), (int)Caption.length(), &size);
			TextOut(hdc, X + W / 2 - size.cx / 2, Y + H / 2 - size.cy / 2, Caption.c_str(), (int)Caption.length());
			return MOUSE_HOVER + ID;
		}
		else {
			drawRectangle(hdc, X, Y, W, H, BorderWidth, ButtonColor, BorderColor);
			GetTextExtentPoint(hdc, Caption.c_str(), (int)Caption.length(), &size);
			TextOut(hdc, X + W / 2 - size.cx / 2, Y + H / 2 - size.cy / 2, Caption.c_str(), (int)Caption.length());
			return 0;
		}
		return 0;

	}

};

#endif
