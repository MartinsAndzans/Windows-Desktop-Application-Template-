#include "Calculator.h"

#pragma region InitCalculatorStaticMembers
HDC Calculator::CalculatorDC = { 0 };
PAINTSTRUCT Calculator::ps = { 0 };

HDC Calculator::MemoryDC = { 0 };
HBITMAP Calculator::Bitmap = { 0 };

HFONT Calculator::CalculatorFont = { 0 };

CONST SHORT Calculator::Padding = 10;

CONST SHORT Calculator::ButtonWidth = 50;
CONST SHORT Calculator::ButtonHeight = 50;

CONST SHORT Calculator::StaticWidth = Calculator::ButtonWidth * 4 + Calculator::Padding * 3;
CONST SHORT Calculator::StaticHeight = 35;

CONST SIZE Calculator::CalculatorDimensions = { Calculator::StaticWidth + Calculator::Padding * 2, Calculator::ButtonHeight * 6 + Calculator::StaticHeight * 2 + Calculator::Padding * 9 };

RECT Calculator::Dimensions = { 0 };
#pragma endregion

#pragma region InitCalculator
/// <summary>
/// Optional Function - Creates Class "CALCULATOR"
/// <para>Width | Height = 250 | 460</para>
/// </summary>
/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
BOOL Calculator::InitCalculator() {
	
	CreateCalculatorFont();

	WNDCLASSEX CalculatorEx = { 0 };

	CalculatorEx.cbClsExtra = 0;
	CalculatorEx.cbWndExtra = 0;
	CalculatorEx.cbSize = sizeof(WNDCLASSEX);
	CalculatorEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	CalculatorEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	CalculatorEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	CalculatorEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	CalculatorEx.hInstance = HInstance();
	CalculatorEx.lpfnWndProc = CalculatorProcedure;
	CalculatorEx.lpszClassName = L"CALCULATOR";
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

SIZE_T Calculator::FindChar(LPSTR Text, const char Char, SIZE_T TextLength) {

	for (SIZE_T I = 0; I < TextLength; I++) {
		if (Text[I] == Char) {
			return I;
		}
	}

	return -1;

}

std::string Calculator::RoundDoubleString(std::string Text) {

	#define LastSymbol Text.length() - 1

	if (Text.find('.') != std::string::npos) {

		while (Text[LastSymbol] == '0' || Text[LastSymbol] == '.') {

			if (Text[LastSymbol] == '.') {
				Text.erase(LastSymbol, 1);
				return Text;
			}

			Text.erase(LastSymbol, 1);

		}

		return Text;

	}

	return Text;

	#undef LastSymbol

}

BOOL Calculator::CreateCalculatorControls(HWND hCalculator) {

	SetLastError(0);

	HWND hwnd = { 0 };

	MathOutputs *Outputs = new MathOutputs;
	ZeroMemory(Outputs, sizeof(MathOutputs));

	SetWindowLongPtr(hCalculator, GWLP_USERDATA, (LONG_PTR)Outputs);

	std::vector <SHORT> CalculatorControlsID = { ID_CL_CLOSE, ID_CL_OPERATION, ID_CL_OUTPUT_RESULT,
		ID_CL_MULTIPLY, ID_CL_DEVIDE, ID_CL_CLEAR, ID_CL_BACK,
		ID_CL_NUM1, ID_CL_NUM2, ID_CL_NUM3, ID_CL_SUM,
		ID_CL_NUM4, ID_CL_NUM5, ID_CL_NUM6, ID_CL_MINUS,
		ID_CL_NUM7, ID_CL_NUM8, ID_CL_NUM9, ID_CL_PLUSMINUS,
		ID_CL_MODULE, ID_CL_NUM0, ID_CL_DOT, ID_CL_RESULT };

	std::vector <std::string> Captions = { "X", "", "0",
		"x", "/", "CE", "<-",
		"1", "2", "3", "+",
		"4", "5", "6", "-",
		"7", "8", "9", "+/-",
		"%", "0", ".", "=" };

	// ID_CL_OPERATION | ID_CL_OUTPUT_RESULT
	INT StaticY[] = { ButtonHeight + Padding * 2, ButtonHeight + StaticHeight + Padding * 3 };

	// NUMPAD CONTROLS
	POINTS NumPad = { Padding, ButtonHeight + StaticHeight * 2 + Padding * 4 };

	for (int i = 0; i < CalculatorControlsID.size(); i++) {

		if (i == 0) {

			// ID_CL_CLOSE
			hwnd = CreateWindowExA(WS_EX_STATICEDGE,
				"BUTTON",
				Captions[i].c_str(),
				WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER | BS_VCENTER,
				CalculatorDimensions.cx - ButtonWidth - Padding, Padding, ButtonWidth, ButtonHeight,
				hCalculator,
				(HMENU)CalculatorControlsID[i],
				HInstance(),
				NULL);

		}
		else if (i == 1 || i == 2) {

			// ID_CL_OPERATION | ID_CL_OUTPUT_RESULT
			hwnd = CreateWindowExA(WS_EX_STATICEDGE,
				"STATIC",
				Captions[i].c_str(),
				WS_CHILD | WS_BORDER | WS_VISIBLE | SS_OWNERDRAW,
				Padding, StaticY[i - 1], StaticWidth, StaticHeight,
				hCalculator,
				(HMENU)CalculatorControlsID[i],
				HInstance(),
				NULL);

			if (i == 1) Outputs->Opearation = hwnd;
			if (i == 2) Outputs->Result = hwnd;

		}
		else {

			//NUMPAD CONTROLS
			hwnd = CreateWindowExA(WS_EX_STATICEDGE,
				"BUTTON",
				Captions[i].c_str(),
				WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER | BS_VCENTER,
				NumPad.x, NumPad.y, ButtonWidth, ButtonHeight,
				hCalculator,
				(HMENU)CalculatorControlsID[i],
				HInstance(),
				NULL);

			NumPad.x = NumPad.x + (ButtonWidth + Padding); // Move In X Coordinate
			if (NumPad.x == CalculatorDimensions.cx) {
				NumPad.x = Padding;
				NumPad.y = NumPad.y + (ButtonHeight + Padding); // Move In Y Coordinate
			}

		}

		if (hwnd == NULL) return FALSE;

		SetFont(hwnd, CalculatorFont);

	}

	return TRUE;

}

#pragma endregion

#pragma region Events

VOID Calculator::onCreate(HWND hCalculator, LPARAM lParam) {

	LPCREATESTRUCT window = (LPCREATESTRUCT)lParam;

	if (window->hwndParent != NULL && (window->style & WS_CHILD) != NULL &&
		(window->style & WS_POPUP) == NULL && (window->style & WS_DLGFRAME) == NULL &&
		(window->style & WS_OVERLAPPED) == NULL && (window->style & WS_SYSMENU) == NULL &&
		(window->style & WS_THICKFRAME) == NULL) {

		if (window->cx != 0 && window->cy != 0) {
			SetWindowPos(hCalculator, NULL, window->x, window->y, CalculatorDimensions.cx, CalculatorDimensions.cy, SWP_SHOWWINDOW);
		}

		if (!CreateCalculatorControls(hCalculator)) {
			std::string ErrorMessage = "ERROR " + std::to_string(GetLastError()) + " - Out of Memory!";
			MessageBoxA(hCalculator, ErrorMessage.c_str(), "ERROR", MB_OK | MB_ICONERROR);
			DestroyWindow(hCalculator);
		}

	}
	else {

		DestroyWindow(hCalculator);

	}

}

VOID Calculator::onWindowPosChanging(HWND hCalculator, LPARAM lParam) {

	LPWINDOWPOS window = (LPWINDOWPOS)lParam;

	if (window->cx != 0 && window->cy != 0) {
		window->cx = CalculatorDimensions.cx, window->cy = CalculatorDimensions.cy;
	}

}

VOID Calculator::onDrawItem(HWND hCalculator, WPARAM wParam, LPARAM lParam) {

	LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lParam;

	if (item->CtlID == ID_CL_OPERATION || item->CtlID == ID_CL_OUTPUT_RESULT) {

		SIZE size = { 0 };
		WCHAR StaticText[MAX_CALCULATOR_STRING_CHAR] = { 0 };
		SetDCBrushColor(item->hDC, RGB(240, 240, 240));
		FillRect(item->hDC, &item->rcItem, (HBRUSH)GetStockObject(DC_BRUSH));
		SetBkMode(item->hDC, TRANSPARENT);
		INT TextLength = GetWindowText(item->hwndItem, StaticText, ARRAYSIZE(StaticText));
		GetTextExtentPoint(item->hDC, StaticText, TextLength, &size);
		TextOut(item->hDC, item->rcItem.right - size.cx, item->rcItem.bottom / 2 - size.cy / 2, StaticText, TextLength);

	}

}

VOID Calculator::onPaint(HWND hCalculator) {

	GetClientRect(hCalculator, &Dimensions);

	CalculatorDC = BeginPaint(hCalculator, &ps);

	MemoryDC = CreateCompatibleDC(CalculatorDC);
	Bitmap = CreateCompatibleBitmap(CalculatorDC, Dimensions.right, Dimensions.bottom);

	SelectObject(MemoryDC, Bitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	SetDCBrushColor(MemoryDC, CalculatorBackgroundColor);
	FillRect(MemoryDC, &Dimensions, (HBRUSH)GetStockObject(DC_BRUSH));
	SelectObject(MemoryDC, CalculatorFont);

	SIZE size = { 0 };
	WCHAR WindowTitle[MAX_CALCULATOR_STRING_CHAR] = { 0 };
	GetWindowText(hCalculator, WindowTitle, ARRAYSIZE(WindowTitle));
	GetTextExtentPoint(MemoryDC, WindowTitle, lstrlenW(WindowTitle), &size);

	if (lstrcmpW(WindowTitle, L"") == 0) {
		GetTextExtentPoint(MemoryDC, L"CALCULATOR", lstrlenW(L"CALCULATOR"), &size);
		TextOut(MemoryDC, Padding, Padding + ButtonWidth / 2 - size.cy / 2, L"CALCULATOR", lstrlenW(L"CALCULATOR"));
	}
	else {
		TextOut(MemoryDC, Padding, Padding + ButtonWidth / 2 - size.cy / 2, WindowTitle, lstrlenW(WindowTitle));
	}

	BitBlt(CalculatorDC, 0, 0, Dimensions.right, Dimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	DeleteDC(MemoryDC);
	DeleteObject(Bitmap);

	EndPaint(hCalculator, &ps);
	
}

VOID Calculator::onCommand(HWND hCalculator, WPARAM wParam, LPARAM lParam) {

	switch (LOWORD(wParam)) {
	case ID_CL_CLOSE:
	{

		//////////////////////////////////////////////////////
		//// +------------------------------------------+ ////
		//// |                                          | ////
		//// | [out] WPARAM - LOWORD(ID) | HIWORD(HWND) | ////
		//// | [out] LPARAM - DestroyWindow             | ////
		//// |                                          | ////
		//// +------------------------------------------+ ////
		//////////////////////////////////////////////////////

		PostMessage(GetParent(hCalculator), WM_COMMAND, MAKEWPARAM(GetWindowLong(hCalculator, GWL_ID), hCalculator), DestroyWindow(hCalculator));
		break;

	}
	case ID_CL_CLEAR:
	{

		LpMathOutputs Outputs = (LpMathOutputs)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		CHAR Result[MAX_CALCULATOR_STRING_CHAR] = { 0 };
		INT ResultLength = GetWindowTextA(Outputs->Result, Result, ARRAYSIZE(Result));

		if (strcmp(Result, "0") == 0) {
			SetWindowTextA(Outputs->Opearation, "");
		}
		else {
			SetWindowTextA(Outputs->Result, "0");
		}

		break;

	}
	case ID_CL_BACK:
	{

		LpMathOutputs Outputs = (LpMathOutputs)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		CHAR Result[MAX_CALCULATOR_STRING_CHAR] = { 0 };
		INT ResultLength = GetWindowTextA(Outputs->Result, Result, ARRAYSIZE(Result));

		if (ResultLength == 1 || (ResultLength == 2 && Result[0] == L'-') ||
			strcmp(Result, DEVISION_ZERO_BY_ZERO) == 0 || strcmp(Result, DEVISION_BY_ZERO) == 0) {
			SetWindowTextA(Outputs->Result, "0");
		}
		else {
			Result[ResultLength - 1] = '\0';
			SetWindowTextA(Outputs->Result, Result);
		}

		break;

	}
	case ID_CL_NUM0: case ID_CL_NUM1: case ID_CL_NUM2: case ID_CL_NUM3:
	case ID_CL_NUM4: case ID_CL_NUM5: case ID_CL_NUM6: case ID_CL_NUM7:
	case ID_CL_NUM8: case ID_CL_NUM9:
	{

		LpMathOutputs Outputs = (LpMathOutputs)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		CHAR Num[MIN_CALCULATOR_STRING_CHAR] = { 0 };
		GetWindowTextA((HWND)lParam, Num, ARRAYSIZE(Num));
		CHAR Result[MAX_CALCULATOR_STRING_CHAR] = { 0 };
		INT ResultLength = GetWindowTextA(Outputs->Result, Result, ARRAYSIZE(Result));

		if (strcmp(Result, DEVISION_ZERO_BY_ZERO) == 0 || strcmp(Result, DEVISION_BY_ZERO) == 0) {
			SetWindowTextA(Outputs->Result, Num);
			break;
		}
		
		if (ResultLength < MAX_RESULT_LENGTH) {
			if (strcmp(Result, "0") == 0) {
				SetWindowTextA(Outputs->Result, Num);
			}
			else if (strcmp(Result, "-0") == 0) {
				strcpy_s(Result, "-"), strcat_s(Result, Num);
				SetWindowTextA(Outputs->Result, Result);
			}
			else {
				strcat_s(Result, Num);
				SetWindowTextA(Outputs->Result, Result);
			}
		}
		else {
			MessageBeep(MB_ICONINFORMATION);
		}

		break;

	}
	case ID_CL_DOT:
	{

		LpMathOutputs Outputs = (LpMathOutputs)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		CHAR Result[MAX_CALCULATOR_STRING_CHAR] = { 0 };
		INT ResultLength = GetWindowTextA(Outputs->Result, Result, ARRAYSIZE(Result));

		if (strcmp(Result, DEVISION_ZERO_BY_ZERO) == 0 || strcmp(Result, DEVISION_BY_ZERO) == 0) {
			SetWindowTextA(Outputs->Result, "0.");
			break;
		}

		if (FindChar(Result, '.', ResultLength) == -1 && ResultLength < MAX_RESULT_LENGTH) {
			strcat_s(Result, ".");
			SetWindowTextA(Outputs->Result, Result);
		}
		else {
			MessageBeep(MB_ICONINFORMATION);
		}

		break;

	}
	case ID_CL_PLUSMINUS:
	{

		LpMathOutputs Outputs = (LpMathOutputs)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		CHAR Result[MAX_CALCULATOR_STRING_CHAR] = { 0 };
		INT ResultLength = GetWindowTextA(Outputs->Result, Result, ARRAYSIZE(Result));

		if (strcmp(Result, DEVISION_ZERO_BY_ZERO) == 0 || strcmp(Result, DEVISION_BY_ZERO) == 0) {
			SetWindowTextA(Outputs->Result, "-0");
			break;
		}

		if (FindChar(Result, '-', ResultLength) == -1) {
			std::string ResultMinus = Result;
			ResultMinus.insert(0, "-");
			SetWindowTextA(Outputs->Result, ResultMinus.c_str());
		}
		else {
			std::string ResultPlus = Result;
			ResultPlus.replace(0, 1, "");
			SetWindowTextA(Outputs->Result, ResultPlus.c_str());
		}

		break;

	}
	case ID_CL_MULTIPLY: case ID_CL_DEVIDE: case ID_CL_SUM: case ID_CL_MINUS: case ID_CL_MODULE:
	{

		LpMathOutputs Outputs = (LpMathOutputs)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		CHAR Operator[MIN_CALCULATOR_STRING_CHAR] = { 0 };
		GetWindowTextA((HWND)lParam, Operator, ARRAYSIZE(Operator));
		CHAR Result[MAX_CALCULATOR_STRING_CHAR] = { 0 };
		INT ResultLength = GetWindowTextA(Outputs->Result, Result, ARRAYSIZE(Result));

		if (Result[ResultLength - 1] != '.' && strcmp(Result, DEVISION_ZERO_BY_ZERO) != 0 &&
			strcmp(Result, DEVISION_BY_ZERO) != 0) {

			strcat_s(Result, " "), strcat_s(Result, Operator);

			SetWindowTextA(Outputs->Opearation, Result);
			SetWindowTextA(Outputs->Result, "0");

		}
		else {
			MessageBeep(MB_ICONINFORMATION);
		}

		break;

	}
	case ID_CL_RESULT:
	{

		LpMathOutputs Outputs = (LpMathOutputs)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		CHAR Operation[MAX_CALCULATOR_STRING_CHAR] = { 0 };
		INT OperationLength = GetWindowTextA(Outputs->Opearation, Operation, ARRAYSIZE(Operation));
		CHAR Result[MAX_CALCULATOR_STRING_CHAR] = { 0 };
		INT ResultLength = GetWindowTextA(Outputs->Result, Result, ARRAYSIZE(Result));

		if (Result[ResultLength - 1] != '.' && strcmp(Operation, "") != 0 &&
			strcmp(Result, DEVISION_ZERO_BY_ZERO) != 0 && strcmp(Result, DEVISION_BY_ZERO) != 0) {

			std::string SOperation = Operation, SResult = Result;

			#pragma region Multiply
			if (SOperation.find('x') != std::string::npos) {

				// Mathematical Operation
				DOUBLE Num1 = std::stod(SOperation);
				DOUBLE Num2 = std::stod(SResult);
				DOUBLE Multiply = Num1 * Num2;
				//
				
				// Operation Output
				SOperation = RoundDoubleString(std::to_string(Num1)) + " x " + RoundDoubleString(std::to_string(Num2)) + " =";
				//

				// Result Output
				SResult = RoundDoubleString(std::to_string(Multiply));
				//

				SetWindowTextA(Outputs->Opearation, SOperation.c_str());
				SetWindowTextA(Outputs->Result, SResult.c_str());

				break;

			}
			#pragma endregion

			#pragma region Devide
			if (SOperation.find('/') != std::string::npos) {

				// Mathematical Operation
				DOUBLE Num1 = std::stod(SOperation);
				DOUBLE Num2 = std::stod(SResult);
				DOUBLE Devide = 0;
				//

				// Operation Output
				SOperation = RoundDoubleString(std::to_string(Num1)) + " / " + RoundDoubleString(std::to_string(Num2)) + " =";
				//

				// Check For Invalid Operations
				if (Num1 == 0 && Num2 == 0) {
					SResult = DEVISION_ZERO_BY_ZERO;
					goto SetWindowTextADevide;
				}
				else if (Num2 == 0) {
					SResult = DEVISION_BY_ZERO;
					goto SetWindowTextADevide;
				}
				//

				// Result Output
				Devide = Num1 / Num2;
				SResult = RoundDoubleString(std::to_string(Devide));
				//

				SetWindowTextADevide:
				SetWindowTextA(Outputs->Opearation, SOperation.c_str());
				SetWindowTextA(Outputs->Result, SResult.c_str());

				break;

			}
			#pragma endregion

			#pragma region Sum
			if (SOperation.find('+') != std::string::npos) {

				// Mathematical Operation
				DOUBLE Num1 = std::stod(SOperation);
				DOUBLE Num2 = std::stod(SResult);
				DOUBLE Sum = Num1 + Num2;
				//

				// Operation Output
				SOperation = RoundDoubleString(std::to_string(Num1)) + " + " + RoundDoubleString(std::to_string(Num2)) + " =";
				//

				// Result Output
				SResult = RoundDoubleString(std::to_string(Sum));
				//

				SetWindowTextA(Outputs->Opearation, SOperation.c_str());
				SetWindowTextA(Outputs->Result, SResult.c_str());

				break;

			}
			#pragma endregion

			#pragma region Minus
			if (SOperation.find('-') != std::string::npos) {

				// Mathematical Operation
				DOUBLE Num1 = std::stod(SOperation);
				DOUBLE Num2 = std::stod(SResult);
				DOUBLE Minus = Num1 - Num2;
				//

				// Operation Output
				SOperation = RoundDoubleString(std::to_string(Num1)) + " - " + RoundDoubleString(std::to_string(Num2)) + " =";
				//

				// Result Output
				SResult = RoundDoubleString(std::to_string(Minus));
				//

				SetWindowTextA(Outputs->Opearation, SOperation.c_str());
				SetWindowTextA(Outputs->Result, SResult.c_str());

				break;

			}
			#pragma endregion

			#pragma region Module
			if (SOperation.find('%') != std::string::npos) {

				// Mathematical Operation
				DOUBLE Num1 = std::stod(SOperation);
				DOUBLE Num2 = std::stod(SResult);
				DOUBLE Module = 0;
				//

				// Operation Output
				SOperation = RoundDoubleString(std::to_string(Num1)) + " % " + RoundDoubleString(std::to_string(Num2)) + " =";
				//

				// Check For Invalid Operations
				if (Num1 == 0 && Num2 == 0) {
					SResult = DEVISION_ZERO_BY_ZERO;
					goto SetWindowTextAModule;
				}
				else if (Num2 == 0) {
					SResult = DEVISION_BY_ZERO;
					goto SetWindowTextAModule;
				}
				//

				// Result Output
				Module = (int)round(Num1) % (int)round(Num2);
				SResult = RoundDoubleString(std::to_string(Module));
				//

				SetWindowTextAModule:
				SetWindowTextA(Outputs->Opearation, SOperation.c_str());
				SetWindowTextA(Outputs->Result, SResult.c_str());

				break;

			}
			#pragma endregion

		}
		else {
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
	case WM_PAINT:
	{
		onPaint(hCalculator);
		return 0;
	}
	case WM_COMMAND:
	{
		onCommand(hCalculator, wParam, lParam);
		return 0;
	}
	case WM_DESTROY:
	{
		LpMathOutputs Outputs = (LpMathOutputs)GetWindowLongPtr(hCalculator, GWLP_USERDATA);
		delete[] Outputs;
		SetWindowLongPtr(hCalculator, GWLP_USERDATA, NULL);
		return 0;
	}
	}

	return DefWindowProc(hCalculator, Msg, wParam, lParam);

};
#pragma endregion
