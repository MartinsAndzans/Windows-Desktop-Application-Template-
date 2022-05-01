#ifndef _CALCULATOR_
#define _CALCULATOR_

#include <ciso646>
#include <Windows.h>
#include <string>
#include <vector>

#define CL_SHOW 2002 // 250 - 400

#define DEVISION_ZERO_BY_ZERO "Undefined"
#define DEVISION_BY_ZERO "Infinity"

#define MIN_CALCULATOR_CHAR_STRING 2
#define MAX_CALCULATOR_CHAR_STRING 256
#define HInstance() GetModuleHandle(NULL)

#define SetFont(hwnd, font) SendMessage(hwnd, WM_SETFONT, (WPARAM)font, NULL)

class Calculator {

private:

	enum CalculatorConntrolsID {
		ID_CL_OPERATION, ID_CL_OUTPUT_RESULT,
		ID_CL_MULTIPLY, ID_CL_DEVIDE, ID_CL_CLEAR, ID_CL_BACK,
		ID_CL_NUM1, ID_CL_NUM2, ID_CL_NUM3, ID_CL_SUM,
		ID_CL_NUM4, ID_CL_NUM5, ID_CL_NUM6, ID_CL_MINUS,
		ID_CL_NUM7, ID_CL_NUM8, ID_CL_NUM9, ID_CL_PLUSMINUS,
		ID_CL_MODULE, ID_CL_NUM0, ID_CL_DOT, ID_CL_RESULT
	};

	typedef struct MathOutputs {
		HWND Opearation;
		HWND Result;
	}*LpMathOutputs;

	#pragma region CalculatorStaticMembers
	static HFONT CalculatorFont;

	static CONST SHORT Padding;

	static CONST SHORT ButtonWidth;
	static CONST SHORT ButtonHeight;

	static CONST SHORT StaticWidth;
	static CONST SHORT StaticHeight;

	static CONST SIZE CalculatorDimensions;

	static RECT Dimensions;
	#pragma endregion

	#pragma region Functions
	static VOID CreateCalculatorFont();
	static SIZE_T FindChar(LPSTR Text, CONST CHAR Char, SIZE_T TextLength);
	static std::string RoundDoubleString(std::string DoubleString);
	static VOID CreateCalculatorControls(HWND hCalculator);
	#pragma endregion

	#pragma region Events
	static VOID onCreate(HWND hCalculator, LPARAM lParam);
	static VOID onWindowPosChanging(HWND hCalculator, LPARAM lParam);
	static VOID onDrawItem(HWND hCalculator, WPARAM wParam, LPARAM lParam);
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

#endif // _CALCULATOR_
