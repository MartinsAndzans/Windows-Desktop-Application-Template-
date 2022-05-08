#include "Calculator.h"

#pragma region InitCalculatorStaticMembers
HFONT Calculator::CalculatorFont = { 0 };

CONST SHORT Calculator::Padding = 10;

CONST SHORT Calculator::ButtonWidth = 50;
CONST SHORT Calculator::ButtonHeight = 50;

CONST SHORT Calculator::StaticWidth = Calculator::ButtonWidth * 4 + Calculator::Padding * 3;
CONST SHORT Calculator::StaticHeight = 35;

CONST SIZE Calculator::CalculatorDimensions = { Calculator::StaticWidth + Calculator::Padding * 2, Calculator::StaticHeight * 2 + Calculator::ButtonHeight * 5 + Calculator::Padding * 8 };
#pragma endregion

#pragma region InitCalculator
/// <summary>
/// Optional Function - Registers Class "Calculator"
/// <para>250 - 400</para>
/// </summary>
/// <returns>If Function Succeeded returns TRUE, but If not returns FALSE</returns>
BOOL Calculator::RegisterCalculatorClass() {
	
	CreateCalculatorFont();

	WNDCLASSEX CalculatorEx = { 0 };

	CalculatorEx.cbClsExtra = 0;
	CalculatorEx.cbWndExtra = 0;
	CalculatorEx.cbSize = sizeof(WNDCLASSEX);
	CalculatorEx.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	CalculatorEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	CalculatorEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	CalculatorEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	CalculatorEx.hInstance = GetModuleHandle(NULL);
	CalculatorEx.lpfnWndProc = CalculatorProcedure;
	CalculatorEx.lpszClassName = L"Calculator";
	CalculatorEx.lpszMenuName = NULL;
	CalculatorEx.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;

	if (!RegisterClassEx(&CalculatorEx)) {
		return FALSE;
	}

    return TRUE;

}
#pragma endregion

#pragma region Functions
VOID Calculator::CreateCalculatorFont() {

	CalculatorFont = CreateFont(20, 0, 0, 0,
		FW_BOLD,
		false,
		false,
		false,
		ANSI_FIXED_FONT,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		VARIABLE_PITCH,
		L"Segoe UI");

}
SIZE_T Calculator::FindChar(LPCSTR Text, CONST CHAR Char, SIZE_T TextLength) {

	for (SIZE_T I = 0; I < TextLength; I++) {
		if (Text[I] == Char) {
			return I;
		}
	}

	return static_cast<SIZE_T>(-1);

}
std::string Calculator::RoundDoubleString(std::string DoubleString) {

	if (DoubleString.find('.') != std::string::npos) {

		while (DoubleString.ends_with('0') or DoubleString.ends_with('.')) {

			if (DoubleString.ends_with('.')) {
				DoubleString.pop_back();
				return DoubleString;
			}

			DoubleString.pop_back();

		}

		return DoubleString;

	}

	return DoubleString;

}
VOID Calculator::CreateCalculatorControls(HWND hCalculator) {

	HWND Hwnd = { 0 };
	MathOutputs *OutputsPtr = new MathOutputs{};
	SetWindowLongPtr(hCalculator, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(OutputsPtr));

	std::vector <std::string> Captions = { "", "0",
		"x", "/", "CE", "<-",
		"1", "2", "3", "+",
		"4", "5", "6", "-",
		"7", "8", "9", "+/-",
		"%", "0", ".", "=" };

	std::vector<SHORT> CalculatorConntrolsIDs = { ID_CL_OPERATION, ID_CL_OUTPUT_RESULT,
		ID_CL_MULTIPLY, ID_CL_DEVIDE, ID_CL_CLEAR, ID_CL_BACK,
		ID_CL_NUM1, ID_CL_NUM2, ID_CL_NUM3, ID_CL_SUM,
		ID_CL_NUM4, ID_CL_NUM5, ID_CL_NUM6, ID_CL_MINUS,
		ID_CL_NUM7, ID_CL_NUM8, ID_CL_NUM9, ID_CL_PLUSMINUS,
		ID_CL_MODULE, ID_CL_NUM0, ID_CL_DOT, ID_CL_RESULT };

	// ID_CL_OPERATION | ID_CL_OUTPUT_RESULT
	CONST INT StaticY[] = { Padding, StaticHeight + Padding * 2 };

	// NUMPAD CONTROLS
	POINTS NumPad = { Padding, StaticHeight * 2 + Padding * 3 };

	for (SIZE_T I = 0; I < CalculatorConntrolsIDs.size(); I++) {

		if (I == 0 or I == 1) {

			// ID_CL_OPERATION | ID_CL_OUTPUT_RESULT
			Hwnd = CreateWindowExA(WS_EX_STATICEDGE,
				"STATIC",
				Captions[I].c_str(),
				WS_CHILD | WS_BORDER | WS_VISIBLE | SS_OWNERDRAW,
				Padding, StaticY[I], StaticWidth, StaticHeight,
				hCalculator,
				(HMENU)CalculatorConntrolsIDs[I],
				GetModuleHandle(NULL),
				NULL);

			if (I == 0) OutputsPtr->Opearation = Hwnd;
			if (I == 1) OutputsPtr->Result = Hwnd;

		} else {

			//NUMPAD CONTROLS
			Hwnd = CreateWindowExA(WS_EX_STATICEDGE,
				"BUTTON",
				Captions[I].c_str(),
				WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER | BS_VCENTER,
				NumPad.x, NumPad.y, ButtonWidth, ButtonHeight,
				hCalculator,
				(HMENU)CalculatorConntrolsIDs[I],
				GetModuleHandle(NULL),
				NULL);

			NumPad.x = NumPad.x + (ButtonWidth + Padding); // Move In X Coordinate
			if (NumPad.x == CalculatorDimensions.cx) {
				NumPad.x = Padding;                             //
				NumPad.y = NumPad.y + (ButtonHeight + Padding); // Move In Y Coordinate
			}                                                   //

		}

		if (Hwnd == NULL) {
			std::string ErrorMessage = "ERROR " + std::to_string(GetLastError()) + " - Child Window not Created!";
			MessageBoxA(hCalculator, ErrorMessage.c_str(), "-ERROR-", MB_OK | MB_ICONERROR);
			DestroyWindow(hCalculator);
		} else {
			SetFont(Hwnd, CalculatorFont);
		}

	}

}
#pragma endregion

#pragma region Events

VOID Calculator::onCreate(HWND hCalculator, LPARAM lParam) {

	LPCREATESTRUCT window = reinterpret_cast<LPCREATESTRUCT>(lParam);

	if (window->hwndParent != NULL and (window->style & WS_CHILD) != NULL) {

		CreateCalculatorControls(hCalculator);

		if (window->cx != 0 and window->cy != 0) {
			SetWindowPos(hCalculator, NULL, window->x, window->y, CalculatorDimensions.cx, CalculatorDimensions.cy, SWP_SHOWWINDOW);
		}

	} else {
		DestroyWindow(hCalculator);
	}

}

VOID Calculator::onWindowPosChanging(HWND hCalculator, LPARAM lParam) {

	LPWINDOWPOS window = reinterpret_cast<LPWINDOWPOS>(lParam);

	if (window->cx != 0 and window->cy != 0) {
		window->cx = CalculatorDimensions.cx, window->cy = CalculatorDimensions.cy;
	}

}

VOID Calculator::onDrawItem(HWND hCalculator, WPARAM wParam, LPARAM lParam) {

	LPDRAWITEMSTRUCT item = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);

	CONST COLORREF LIGHT_GRAY_COLOR = RGB(240, 240, 240);

	if (item->CtlID == ID_CL_OPERATION or item->CtlID == ID_CL_OUTPUT_RESULT) {

		SIZE size = { 0 };
		CONST USHORT MAX_CHAR_STRING = 256;
		WCHAR StaticText[MAX_CHAR_STRING] = { 0 };
		SetDCBrushColor(item->hDC, LIGHT_GRAY_COLOR);
		FillRect(item->hDC, &item->rcItem, static_cast<HBRUSH>(GetStockObject(DC_BRUSH)));
		SetBkMode(item->hDC, TRANSPARENT);
		INT TextLength = GetWindowText(item->hwndItem, StaticText, ARRAYSIZE(StaticText));
		GetTextExtentPoint(item->hDC, StaticText, TextLength, &size);
		TextOut(item->hDC, item->rcItem.right - size.cx, item->rcItem.bottom / 2 - size.cy / 2, StaticText, TextLength);

	}

}

VOID Calculator::onPaint(HWND hCalculator) {

	PAINTSTRUCT ps = { 0 };
	RECT Dimensions = { 0 };

	GetClientRect(hCalculator, &Dimensions);

	HDC CalculatorDC = BeginPaint(hCalculator, &ps);

	HDC MemoryDC = CreateCompatibleDC(CalculatorDC);
	HBITMAP Bitmap = CreateCompatibleBitmap(CalculatorDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	FillRect(MemoryDC, &Dimensions, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));

	SelectObject(MemoryDC, CalculatorFont);



	DeleteDC(MemoryDC);
	DeleteObject(Bitmap);

	EndPaint(hCalculator, &ps);

}

VOID Calculator::onCommand(HWND hCalculator, WPARAM wParam, LPARAM lParam) {

	switch (LOWORD(wParam)) {
	case ID_CL_CLEAR:
	{

		LpMathOutputs OutputsPtr = reinterpret_cast<LpMathOutputs>(GetWindowLongPtr(hCalculator, GWLP_USERDATA));

		CONST USHORT MAX_CHAR_STRING = 256;
		CHAR Result[MAX_CHAR_STRING] = { 0 };
		GetWindowTextA(OutputsPtr->Result, Result, ARRAYSIZE(Result));

		if (strcmp(Result, "0") == 0) {
			SetWindowTextA(OutputsPtr->Opearation, "");
		} else {
			SetWindowTextA(OutputsPtr->Result, "0");
		}

		break;

	}
	case ID_CL_BACK:
	{

		LpMathOutputs OutputsPtr = reinterpret_cast<LpMathOutputs>(GetWindowLongPtr(hCalculator, GWLP_USERDATA));

		CONST USHORT MAX_CHAR_STRING = 256;
		CHAR Result[MAX_CHAR_STRING] = { 0 };
		INT ResultLength = GetWindowTextA(OutputsPtr->Result, Result, ARRAYSIZE(Result));

		if (ResultLength == 1 or (ResultLength == 2 and Result[0] == L'-') or
			strcmp(Result, DEVISION_ZERO_BY_ZERO) == 0 or strcmp(Result, DEVISION_BY_ZERO) == 0) {
			SetWindowTextA(OutputsPtr->Result, "0");
		} else {
			Result[ResultLength - 1] = '\0';
			SetWindowTextA(OutputsPtr->Result, Result);
		}

		break;

	}
	case ID_CL_NUM0: case ID_CL_NUM1: case ID_CL_NUM2: case ID_CL_NUM3:
	case ID_CL_NUM4: case ID_CL_NUM5: case ID_CL_NUM6: case ID_CL_NUM7:
	case ID_CL_NUM8: case ID_CL_NUM9:
	{

		LpMathOutputs OutputsPtr = reinterpret_cast<LpMathOutputs>(GetWindowLongPtr(hCalculator, GWLP_USERDATA));

		CONST USHORT MIN_CHAR_STRING = 2;
		CHAR Num[MIN_CHAR_STRING] = { 0 };
		GetWindowTextA((HWND)lParam, Num, ARRAYSIZE(Num));
		CONST USHORT MAX_CHAR_STRING = 256;
		CHAR Result[MAX_CHAR_STRING] = { 0 };
		GetWindowTextA(OutputsPtr->Result, Result, ARRAYSIZE(Result));

		if (strcmp(Result, DEVISION_ZERO_BY_ZERO) == 0 or strcmp(Result, DEVISION_BY_ZERO) == 0) {
			SetWindowTextA(OutputsPtr->Result, Num);
			break;
		}
		
		if (strcmp(Result, "0") == 0) {
			SetWindowTextA(OutputsPtr->Result, Num);
		} else if (strcmp(Result, "-0") == 0) {
			strcpy_s(Result, "-"), strcat_s(Result, Num);
			SetWindowTextA(OutputsPtr->Result, Result);
		} else {
			strcat_s(Result, Num);
			SetWindowTextA(OutputsPtr->Result, Result);
		}

		break;

	}
	case ID_CL_DOT:
	{

		LpMathOutputs OutputsPtr = reinterpret_cast<LpMathOutputs>(GetWindowLongPtr(hCalculator, GWLP_USERDATA));

		CONST USHORT MAX_CHAR_STRING = 256;
		CHAR Result[MAX_CHAR_STRING] = { 0 };
		INT ResultLength = GetWindowTextA(OutputsPtr->Result, Result, ARRAYSIZE(Result));

		if (strcmp(Result, DEVISION_ZERO_BY_ZERO) == 0 or strcmp(Result, DEVISION_BY_ZERO) == 0) {
			SetWindowTextA(OutputsPtr->Result, "0.");
			break;
		}

		if (FindChar(Result, '.', ResultLength) == static_cast<SIZE_T>(-1)) {
			strcat_s(Result, ".");
			SetWindowTextA(OutputsPtr->Result, Result);
		} else {
			MessageBeep(MB_ICONINFORMATION);
		}

		break;

	}
	case ID_CL_PLUSMINUS:
	{

		LpMathOutputs OutputsPtr = reinterpret_cast<LpMathOutputs>(GetWindowLongPtr(hCalculator, GWLP_USERDATA));

		CONST USHORT MAX_CHAR_STRING = 256;
		CHAR ResultBuffer[MAX_CHAR_STRING] = { 0 };
		GetWindowTextA(OutputsPtr->Result, ResultBuffer, ARRAYSIZE(ResultBuffer));
		std::string Result = ResultBuffer;

		if (strcmp(ResultBuffer, DEVISION_ZERO_BY_ZERO) == 0 or strcmp(ResultBuffer, DEVISION_BY_ZERO) == 0) {
			SetWindowTextA(OutputsPtr->Result, "-0");
			break;
		}

		if (Result.find('-') == std::string::npos) {
			Result.insert(0, "-");
			SetWindowTextA(OutputsPtr->Result, Result.c_str());
		} else {
			Result.replace(0, 1, "");
			SetWindowTextA(OutputsPtr->Result, Result.c_str());
		}

		break;

	}
	case ID_CL_MULTIPLY: case ID_CL_DEVIDE: case ID_CL_SUM: case ID_CL_MINUS: case ID_CL_MODULE:
	{

		LpMathOutputs OutputsPtr = reinterpret_cast<LpMathOutputs>(GetWindowLongPtr(hCalculator, GWLP_USERDATA));

		CONST USHORT MIN_CHAR_STRING = 2;
		CHAR OperatorBuffer[MIN_CHAR_STRING] = { 0 };
		GetWindowTextA((HWND)lParam, OperatorBuffer, ARRAYSIZE(OperatorBuffer));
		std::string Operator = OperatorBuffer;
		CONST USHORT MAX_CHAR_STRING = 256;
		CHAR ResultBuffer[MAX_CHAR_STRING] = { 0 };
		GetWindowTextA(OutputsPtr->Result, ResultBuffer, ARRAYSIZE(ResultBuffer));
		std::string Result = ResultBuffer;

		if (Result.find(DEVISION_ZERO_BY_ZERO) == std::string::npos and Result.find(DEVISION_BY_ZERO) == std::string::npos) {

			Result += " " + Operator;

			SetWindowTextA(OutputsPtr->Opearation, Result.c_str());
			SetWindowTextA(OutputsPtr->Result, "0");

		} else {
			MessageBeep(MB_ICONINFORMATION);
		}

		break;

	}
	case ID_CL_RESULT:
	{

		LpMathOutputs OutputsPtr = reinterpret_cast<LpMathOutputs>(GetWindowLongPtr(hCalculator, GWLP_USERDATA));

		CONST USHORT MAX_CHAR_STRING = 256;
		CHAR OperationBuffer[MAX_CHAR_STRING] = { 0 };
		GetWindowTextA(OutputsPtr->Opearation, OperationBuffer, ARRAYSIZE(OperationBuffer));
		std::string Operation = OperationBuffer;
		CHAR ResultBuffer[MAX_CHAR_STRING] = { 0 };
		GetWindowTextA(OutputsPtr->Result, ResultBuffer, ARRAYSIZE(ResultBuffer));
		std::string Result = ResultBuffer;

		if (!Operation.empty() and Result.find(DEVISION_ZERO_BY_ZERO) == std::string::npos and Result.find(DEVISION_BY_ZERO) == std::string::npos) {

			#pragma region Multiply
			if (Operation.find('x') != std::string::npos) {

				// Mathematical Operation
				DOUBLE Num1 = std::stod(Operation);
				DOUBLE Num2 = std::stod(Result);
				DOUBLE Multiply = Num1 * Num2;
				//
				
				// Operation Output
				Operation = RoundDoubleString(std::to_string(Num1)) + " x " + RoundDoubleString(std::to_string(Num2)) + " =";
				//

				// Result Output
				Result = RoundDoubleString(std::to_string(Multiply));
				//

				SetWindowTextA(OutputsPtr->Opearation, Operation.c_str());
				SetWindowTextA(OutputsPtr->Result, Result.c_str());

				break;

			}
			#pragma endregion

			#pragma region Devide
			if (Operation.find('/') != std::string::npos) {

				// Mathematical Operation
				DOUBLE Num1 = std::stod(Operation);
				DOUBLE Num2 = std::stod(Result);
				DOUBLE Devide = 0;
				//

				// Operation Output
				Operation = RoundDoubleString(std::to_string(Num1)) + " / " + RoundDoubleString(std::to_string(Num2)) + " =";
				//
				
				// Check For Invalid Operations
				if (Num1 == 0 and Num2 == 0) {
					Result = DEVISION_ZERO_BY_ZERO;
					goto DevideOutput;
				}
				else if (Num2 == 0) {
					Result = DEVISION_BY_ZERO;
					goto DevideOutput;
				}
				//

				// Result Output
				Devide = Num1 / Num2;
				Result = RoundDoubleString(std::to_string(Devide));
				//

				DevideOutput:
				SetWindowTextA(OutputsPtr->Opearation, Operation.c_str());
				SetWindowTextA(OutputsPtr->Result, Result.c_str());

				break;

			}
			#pragma endregion

			#pragma region Sum
			if (Operation.find('+') != std::string::npos) {

				// Mathematical Operation
				DOUBLE Num1 = std::stod(Operation);
				DOUBLE Num2 = std::stod(Result);
				DOUBLE Sum = Num1 + Num2;
				//

				// Operation Output
				Operation = RoundDoubleString(std::to_string(Num1)) + " + " + RoundDoubleString(std::to_string(Num2)) + " =";
				//

				// Result Output
				Result = RoundDoubleString(std::to_string(Sum));
				//

				SetWindowTextA(OutputsPtr->Opearation, Operation.c_str());
				SetWindowTextA(OutputsPtr->Result, Result.c_str());

				break;

			}
			#pragma endregion

			#pragma region Minus
			if (Operation.find('-') != std::string::npos) {

				// Mathematical Operation
				DOUBLE Num1 = std::stod(Operation);
				DOUBLE Num2 = std::stod(Result);
				DOUBLE Minus = Num1 - Num2;
				//

				// Operation Output
				Operation = RoundDoubleString(std::to_string(Num1)) + " - " + RoundDoubleString(std::to_string(Num2)) + " =";
				//

				// Result Output
				Result = RoundDoubleString(std::to_string(Minus));
				//

				SetWindowTextA(OutputsPtr->Opearation, Operation.c_str());
				SetWindowTextA(OutputsPtr->Result, Result.c_str());

				break;

			}
			#pragma endregion

			#pragma region Module
			if (Operation.find('%') != std::string::npos) {

				// Mathematical Operation
				INT Num1 = std::stoi(Operation);
				INT Num2 = std::stoi(Result);
				INT Module = 0;
				//

				// Operation Output
				Operation = RoundDoubleString(std::to_string(Num1)) + " % " + RoundDoubleString(std::to_string(Num2)) + " =";
				//

				// Check For Invalid Operations
				if (Num1 == 0 and Num2 == 0) {
					Result = DEVISION_ZERO_BY_ZERO;
					goto ModuleOutput;
				}
				else if (Num2 == 0) {
					Result = DEVISION_BY_ZERO;
					goto ModuleOutput;
				}
				//

				// Result Output
				Module = Num1 % Num2;
				Result = RoundDoubleString(std::to_string(Module));
				//

				ModuleOutput:
				SetWindowTextA(OutputsPtr->Opearation, Operation.c_str());
				SetWindowTextA(OutputsPtr->Result, Result.c_str());

				break;

			}
			#pragma endregion

		} else {
			MessageBeep(MB_ICONINFORMATION);
		}

		break;

	}
	}
	
}
#pragma endregion

#pragma region CalculatorProcedure
LRESULT CALLBACK Calculator::CalculatorProcedure(HWND hCalculator, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{
		onCreate(hCalculator, lParam);
		return 0;
	}
	case WM_WINDOWPOSCHANGING:
	{
		onWindowPosChanging(hCalculator, lParam);
		return 0;
	}
	case WM_DRAWITEM:
	{
		onDrawItem(hCalculator, wParam, lParam);
		return 0;
	}
	case WM_COMMAND:
	{
		onCommand(hCalculator, wParam, lParam);
		return 0;
	}
	case WM_DESTROY:
	{
		LpMathOutputs OutputsPtr = reinterpret_cast<LpMathOutputs>(GetWindowLongPtr(hCalculator, GWLP_USERDATA));
		delete[] OutputsPtr;
		return 0;
	}
	}

	return DefWindowProc(hCalculator, Msg, wParam, lParam);

};
#pragma endregion
