#pragma once

#ifndef _CALCULATOR_
#define _CALCULATOR_

#include <Windows.h>
#include <string>
#include <math.h>

#define CL_SHOW 20002

#define MAX_RESULT_LENGTH 8

#define DEVISION_ZERO_BY_ZERO L"Undefined"
#define DEVISION_BY_ZERO L"Infinity"
#define RESULT_IS_TOO_LARGE L"Result Is Too Large"

#define MAX_CLTITLE_CHAR 256
#define HInstance() GetModuleHandle(NULL)

#define CalculatorBackgroundColor RGB(255, 255, 255)

#pragma region CalculatorControls

#define ID_CL_CLOSE 10011
#define ID_CL_OPERATION 10012
#define ID_CL_OUTPUT_RESULT 10013
#define ID_CL_MULTIPLY 10014
#define ID_CL_DEVIDE 10015
#define ID_CL_CLEAR 10016
#define ID_CL_BACK 10017
#define ID_CL_SUM 10018
#define ID_CL_MINUS 10019
#define ID_CL_PLUSMINUS 10020
#define ID_CL_NUM0 10021
#define ID_CL_NUM1 10022
#define ID_CL_NUM2 10023
#define ID_CL_NUM3 10024
#define ID_CL_NUM4 10025
#define ID_CL_NUM5 10026
#define ID_CL_NUM6 10027
#define ID_CL_NUM7 10028
#define ID_CL_NUM8 10029
#define ID_CL_NUM9 10030
#define ID_CL_MODULE 10031
#define ID_CL_DOT 10032
#define ID_CL_RESULT 10033

#pragma endregion

#define SetFont(hwnd, font) SendMessage(hwnd, WM_SETFONT, (WPARAM)font, NULL)

class Calculator {

private:

	#pragma region CalculatorStaticMembers

	static HDC CalculatorDC;
	static PAINTSTRUCT ps;

	static HDC MemoryDC;
	static HBITMAP Bitmap;

	static HBRUSH CalculatorBackroundBrush;

	static HFONT CalculatorFont;

	static HMENU CalculatorControlsID[];

	static CONST INT PADDING;

	static CONST INT BUTTONWIDTH;
	static CONST INT BUTTONHEIGHT;

	static CONST INT STATICWIDTH;
	static CONST INT STATICHEIGHT;

	static CONST POINT CalculatorDimensions;

	static RECT Dimensions;

	#pragma endregion

	#pragma region Functions

	static HFONT createCalculatorFont();

	static INT FindChar(wchar_t(&Text)[], const wchar_t Char, INT TextLength);
	static BOOL RoundDouble(wchar_t(&Text)[], INT TextLength);

	static std::wstring _ditow(int64_t Value);
	static BOOL _ftow(DOUBLE Value, wchar_t (&Buffer)[256 * 2 + 1], INT Precision);

	static BOOL createCalculatorControls(HWND hCalculator);

	#pragma endregion

	#pragma region Events

	static VOID onCreate(HWND hCalculator, LPARAM lParam);
	static VOID onWindowPosChanging(HWND hCalculator, LPARAM lParam);
	static VOID onDrawItem(HWND hCalculator, WPARAM wParam, LPARAM lParam);
	static VOID onPaint(HWND hCalculator);
	static VOID onCommand(HWND hCalculator, WPARAM wParam, LPARAM lParam);

	#pragma endregion

	#pragma region CalculatorProcedure

	static LRESULT CALLBACK CalculatorProcedure(HWND hCalculator, UINT Msg, WPARAM wParam, LPARAM lParam);

	#pragma endregion

public:

	#pragma region InitCalculator

	static BOOL InitCalculator();

	#pragma endregion

};

#endif