#include "Calculator.h"

// INIT CALCULATOR STATIC MEMBERS

#pragma region InitCalculatorStaticMembers

HDC Calculator::CalculatorDC = { 0 };
PAINTSTRUCT Calculator::ps = { 0 };

HDC Calculator::MemoryDC = { 0 };
HBITMAP Calculator::Bitmap = { 0 };

HBRUSH Calculator::CalculatorBackroundBrush = CreateSolidBrush(CalculatorBackgroundColor);

HFONT Calculator::CalculatorFont = { 0 };

HMENU Calculator::CalculatorControlsID[] = {(HMENU)ID_CL_CLOSE, (HMENU)ID_CL_OPERATION, (HMENU)ID_CL_OUTPUT_RESULT,
		(HMENU)ID_CL_MULTIPLY, (HMENU)ID_CL_DEVIDE, (HMENU)ID_CL_CLEAR, (HMENU)ID_CL_BACK,
		(HMENU)ID_CL_NUM1, (HMENU)ID_CL_NUM2, (HMENU)ID_CL_NUM3, (HMENU)ID_CL_SUM,
		(HMENU)ID_CL_NUM4, (HMENU)ID_CL_NUM5, (HMENU)ID_CL_NUM6, (HMENU)ID_CL_MINUS,
		(HMENU)ID_CL_NUM7, (HMENU)ID_CL_NUM8, (HMENU)ID_CL_NUM9, (HMENU)ID_CL_PLUSMINUS,
		(HMENU)ID_CL_MODULE, (HMENU)ID_CL_NUM0, (HMENU)ID_CL_DOT, (HMENU)ID_CL_RESULT };

CONST INT Calculator::PADDING = 5;

CONST INT Calculator::BUTTONWIDTH = 50;
CONST INT Calculator::BUTTONHEIGHT = 50;

CONST INT Calculator::STATICWIDTH = Calculator::BUTTONWIDTH * 4 + Calculator::PADDING * 3;
CONST INT Calculator::STATICHEIGHT = 35;

CONST POINT Calculator::CalculatorDimensions = { Calculator::BUTTONWIDTH * 4 + Calculator::PADDING * 5, Calculator::BUTTONHEIGHT * 6 + Calculator::STATICHEIGHT * 2 + Calculator::PADDING * 9 };

RECT Calculator::Dimensions = { 0 };

#pragma endregion

// INIT CALCULATOR

#pragma region InitCalculator

BOOL Calculator::InitCalculator() {

	/// <summary>
	/// OPTIONAL FUNCTION
	/// CREATES CLASS "CALCULATOR"
	/// </summary>
	/// <returns>If Function Succeeded Returns True, but If not False</returns>
	
	CalculatorFont = createCalculatorFont();

	WNDCLASSEX CalculatorEx = { 0 };

	CalculatorEx.cbClsExtra = 0;
	CalculatorEx.cbWndExtra = 0;
	CalculatorEx.cbSize = sizeof(WNDCLASSEX);
	CalculatorEx.hbrBackground = CalculatorBackroundBrush;
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

HFONT Calculator::createCalculatorFont() {

	HFONT Font = CreateFont(20, 0, 0, 0,
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

	return Font;

}

DOUBLE Calculator::_atod(LPWSTR Text) {

	CONST POINTS ASCII_NUMBERS = { 48, 57 }; // 0 - 9
	CONST SHORT ASCII_MINUS = 45; // -

	UINT LEFT = 1;
	DOUBLE RIGHT = 0.1;

	BOOL MINUS = FALSE;

	DOUBLE Number = 0.00;

	INT DotPosition = FindChar(Text, L'.', lstrlenW(Text));

	if (DotPosition != -1 || 0) {

		// 123456789.123456789

		for (int i = DotPosition - 1; i >= 0; i--) {
			INT ASCII = (int)Text[i];
			if (ASCII >= ASCII_NUMBERS.x && ASCII <= ASCII_NUMBERS.y) {
				Number = Number + (ASCII - ASCII_NUMBERS.x) * LEFT;
				LEFT = LEFT * 10;
			}
			else {
				(ASCII == ASCII_MINUS) ? MINUS = TRUE : MINUS = FALSE;
				break;
			}
		}
		for (int i = DotPosition + 1; i < lstrlenW(Text); i++) {
			INT ASCII = (int)Text[i];
			if (ASCII >= ASCII_NUMBERS.x && ASCII <= ASCII_NUMBERS.y) {
				Number = Number + (ASCII - ASCII_NUMBERS.x) * RIGHT;
				RIGHT = RIGHT / 10;
			}
			else {
				break;
			}
		}

		if (MINUS == TRUE) {
			Number = Number * -1;
		}

		return Number;

	}
	if (DotPosition != -1) {

		//.123456789

		for (int i = DotPosition + 1; i < lstrlenW(Text); i++) {
			INT ASCII = (int)Text[i];
			if (ASCII >= ASCII_NUMBERS.x && ASCII <= ASCII_NUMBERS.y) {
				Number = Number + (ASCII - ASCII_NUMBERS.x) * RIGHT;
				RIGHT = RIGHT / 10;
			}
			else {
				break;
			}
		}

		if (MINUS == TRUE) {
			Number = Number * -1;
		}

		return Number;

	}
	if (DotPosition == -1) {

		// 123456789

		INT NumberLength = -1;

		for (int i = 0; i < lstrlenW(Text); i++) {
			INT ASCII = (int)Text[i];
			if (ASCII >= ASCII_NUMBERS.x && ASCII <= ASCII_NUMBERS.y || ASCII == ASCII_MINUS) {
				NumberLength++;
			}
			else {
				break;
			}
		}
		for (int i = NumberLength; i >= 0; i--) {
			INT ASCII = (int)Text[i];
			if (ASCII >= ASCII_NUMBERS.x && ASCII <= ASCII_NUMBERS.y) {
				Number = Number + (ASCII - ASCII_NUMBERS.x) * LEFT;
				LEFT = LEFT * 10;
			}
			else {
				(ASCII == ASCII_MINUS) ? MINUS = TRUE : MINUS = FALSE;
				break;
			}
		}

		if (MINUS == TRUE) {
			Number = Number * -1;
		}

		return Number;

	}

	return Number;
	
}

INT Calculator::FindChar(LPWSTR Text, const wchar_t Char, INT TextLength) {

	for (int i = 0; i < TextLength; i++) {

		if (Text[i] == Char) {
			return i;
		}

	}

	return -1;

}

LPWSTR Calculator::RoundDouble(LPWSTR Text, INT TextLength) {

	if (FindChar(Text, L'.', TextLength) != -1) {

		for (int i = 0; i < TextLength; i++) {

			if (Text[TextLength - i - 1] == L'0' || Text[TextLength - i - 1] == L'.') {
				if (Text[TextLength - i - 1] == L'.') {
					Text[TextLength - i - 1] = L'\0';
					return Text;
				}
				Text[TextLength - i - 1] = L'\0';
			}
			else {
				return Text;
			}

		}

	}

	return Text;

}

BOOL Calculator::createCalculatorControls(HWND hCalculator) {

	HWND hwnd = { 0 };

	HWND *Outputs = new HWND[2];

	if (Outputs == NULL) {
		return FALSE;
	}

	INT XISTATIC = 0, XIBUTTON = 0, YIBUTTON = 0;

	std::wstring Captions[] = {L"X", L"", L"0",
		L"x", L"/", L"CE", L"<-",
		L"1", L"2", L"3", L"+",
		L"4", L"5", L"6", L"-",
		L"7", L"8", L"9", L"+/-",
		L"%", L"0", L".", L"="};

	// ID_CL_OPERATION | ID_CL_OUTPUT_RESULT

	INT STATICY[] = {PADDING * 2 + BUTTONHEIGHT, PADDING * 3 + STATICHEIGHT + BUTTONHEIGHT };

	// OTHER CONTROLS

	INT BUTTONX[] = {PADDING, PADDING * 2 + BUTTONWIDTH, PADDING * 3 + BUTTONWIDTH * 2, PADDING * 4 + BUTTONWIDTH * 3};
	INT BUTTONY[] = {PADDING * 4 + STATICHEIGHT * 2 + BUTTONHEIGHT, PADDING * 5 + STATICHEIGHT * 2 + BUTTONHEIGHT * 2,
		PADDING * 6 + STATICHEIGHT * 2 + BUTTONHEIGHT * 3, PADDING * 7 + STATICHEIGHT * 2 + BUTTONHEIGHT * 4, PADDING * 8 + STATICHEIGHT * 2 + BUTTONHEIGHT * 5 };

	for (int i = 0; i < ARRAYSIZE(CalculatorControlsID); i++) {

		if (i == 0) {

			// ID_CL_CLOSE
			
			hwnd = CreateWindowEx(WS_EX_STATICEDGE,
				L"BUTTON",
				Captions[i].c_str(),
				WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER | BS_VCENTER,
				CalculatorDimensions.x - BUTTONWIDTH - PADDING, PADDING, BUTTONWIDTH, BUTTONHEIGHT,
				hCalculator,
				CalculatorControlsID[i],
				HInstance(),
				NULL);

		}
		else if (i == 1 || i == 2) {

			// ID_CL_OPERATION | ID_CL_OUTPUT_RESULT

			hwnd = CreateWindowEx(WS_EX_STATICEDGE,
				L"STATIC",
				Captions[i].c_str(),
				WS_CHILD | WS_BORDER | WS_VISIBLE | SS_OWNERDRAW,
				PADDING, STATICY[XISTATIC], STATICWIDTH, STATICHEIGHT,
				hCalculator,
				CalculatorControlsID[i],
				HInstance(),
				NULL);

			*(Outputs + (i - 1)) = hwnd;
			
			XISTATIC++;

		}
		else {

			// OTHER CONTROLS

			hwnd = CreateWindowEx(WS_EX_STATICEDGE,
				L"BUTTON",
				Captions[i].c_str(),
				WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER | BS_VCENTER,
				BUTTONX[XIBUTTON], BUTTONY[YIBUTTON], BUTTONWIDTH, BUTTONHEIGHT,
				hCalculator,
				CalculatorControlsID[i],
				HInstance(),
				NULL);

			XIBUTTON++;

			if (XIBUTTON == ARRAYSIZE(BUTTONX)) {
				XIBUTTON = 0;
				YIBUTTON++;
			}

		}

		if (hwnd == NULL) {
			return FALSE;
		}

		SetFont(hwnd, CalculatorFont);

	}

	SetWindowLongPtr(hCalculator, GWLP_USERDATA, (LONG_PTR)Outputs);

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
			SetWindowPos(hCalculator, NULL, window->x, window->y, CalculatorDimensions.x, CalculatorDimensions.y, SWP_SHOWWINDOW);
		}

		if (!createCalculatorControls(hCalculator)) {
			std::wstring WErrorMessage = L"ERROR " + std::to_wstring(GetLastError()) + L" - Out of Memory!";
			MessageBox(hCalculator, WErrorMessage.c_str(), L"ERROR", MB_OK | MB_ICONERROR);
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
		window->cx = CalculatorDimensions.x, window->cy = CalculatorDimensions.y;
	}

}

VOID Calculator::onDrawItem(HWND hCalculator, WPARAM wParam, LPARAM lParam) {

	LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lParam;

	if (item->CtlID == ID_CL_OPERATION || item->CtlID == ID_CL_OUTPUT_RESULT) {

		SIZE size = { 0 };
		WCHAR StaticText[MAX_CLTITLE_CHAR] = { 0 };
		HBRUSH ItemBrush = CreateSolidBrush(RGB(240, 240, 240));
		FillRect(item->hDC, &item->rcItem, ItemBrush);
		DeleteObject(ItemBrush);
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
	HBRUSH BackgroundBrush = CreateSolidBrush(CalculatorBackgroundColor);
	FillRect(MemoryDC, &Dimensions, BackgroundBrush);
	DeleteObject(BackgroundBrush);

	SelectObject(MemoryDC, CalculatorFont);

	SIZE size = { 0 };
	WCHAR WindowTitle[MAX_CLTITLE_CHAR] = { 0 };
	GetWindowText(hCalculator, WindowTitle, ARRAYSIZE(WindowTitle));
	GetTextExtentPoint(MemoryDC, WindowTitle, lstrlenW(WindowTitle), &size);

	if (lstrcmpW(WindowTitle, L"") == 0) {
		wcscpy_s(WindowTitle, L"CALCULATOR");
		GetTextExtentPoint(MemoryDC, WindowTitle, lstrlenW(WindowTitle), &size);
		TextOut(MemoryDC, PADDING, PADDING + BUTTONWIDTH / 2 - size.cy / 2, WindowTitle, lstrlenW(WindowTitle));
	}
	else {
		TextOut(MemoryDC, PADDING, PADDING + BUTTONWIDTH / 2 - size.cy / 2, WindowTitle, lstrlenW(WindowTitle));
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

		DWORD ID = GetWindowLong(hCalculator, GWL_ID);
		PostMessage(GetParent(hCalculator), WM_COMMAND, MAKEWPARAM(ID, hCalculator), DestroyWindow(hCalculator));
		break;

	}
	case ID_CL_CLEAR:
	{

		HWND *Outputs = (HWND*)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		WCHAR Result[MAX_CLTITLE_CHAR] = { 0 };
		INT ResultLength = GetWindowText(*(Outputs + 1), Result, ARRAYSIZE(Result));

		if (lstrcmpW(Result, L"0") == 0) {
			SetWindowText(*(Outputs + 0), L"");
		}
		else {
			SetWindowText(*(Outputs + 1), L"0");
		}

		break;

	}
	case ID_CL_BACK:
	{

		HWND *Outputs = (HWND*)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		WCHAR Result[MAX_CLTITLE_CHAR] = { 0 };
		INT ResultLength = GetWindowText(*(Outputs + 1), Result, ARRAYSIZE(Result));

		if (ResultLength == 1 || lstrcmpW(Result, WINFINITY) == 0 || lstrcmpW(Result, WDEVISION_BY_ZERO) == 0 || (ResultLength == 2 && Result[0] == L'-')) {
			SetWindowText(*(Outputs + 1), L"0");
		}
		else {
			Result[ResultLength - 1] = L'\0';
			SetWindowText(*(Outputs + 1), Result);
		}

		break;

	}
	case ID_CL_NUM0: case ID_CL_NUM1: case ID_CL_NUM2: case ID_CL_NUM3:
	case ID_CL_NUM4: case ID_CL_NUM5: case ID_CL_NUM6: case ID_CL_NUM7:
	case ID_CL_NUM8: case ID_CL_NUM9:
	{

		HWND *Outputs = (HWND*)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		WCHAR Num[MAX_CLTITLE_CHAR] = { 0 };
		GetWindowText((HWND)lParam, Num, ARRAYSIZE(Num));
		WCHAR Result[MAX_CLTITLE_CHAR] = { 0 };
		INT ResultLength = GetWindowText(*(Outputs + 1), Result, ARRAYSIZE(Result));

		if (lstrcmpW(Result, WINFINITY) == 0 || lstrcmpW(Result, WDEVISION_BY_ZERO) == 0) {
			wcscpy_s(Result, Num);
			SetWindowText(*(Outputs + 1), Result);
			break;
		}
		
		if (ResultLength < MAX_RESULT_LENGTH) {
			if (lstrcmpW(Result, L"0") == 0) {
				wcscpy_s(Result, Num);
				SetWindowText(*(Outputs + 1), Result);
			}
			else {
				wcscat_s(Result, Num);
				SetWindowText(*(Outputs + 1), Result);
			}
		}
		else {
			MessageBeep(MB_ICONINFORMATION);
		}

		break;

	}
	case ID_CL_DOT:
	{

		HWND *Outputs = (HWND*)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		WCHAR Dot[MAX_CLTITLE_CHAR] = { 0 };
		GetWindowText((HWND)lParam, Dot, ARRAYSIZE(Dot));
		WCHAR Result[MAX_CLTITLE_CHAR] = { 0 };
		INT ResultLength = GetWindowText(*(Outputs + 1), Result, ARRAYSIZE(Result));

		if (lstrcmpW(Result, WINFINITY) == 0 || lstrcmpW(Result, WDEVISION_BY_ZERO) == 0) {
			wcscpy_s(Result, L"0.");
			SetWindowText(*(Outputs + 1), Result);
			break;
		}

		if (FindChar(Result, L'.', ResultLength) == -1 && ResultLength < MAX_RESULT_LENGTH) {
			wcscat_s(Result, Dot);
			SetWindowText(*(Outputs + 1), Result);
		}
		else {
			MessageBeep(MB_ICONINFORMATION);
		}

		break;

	}
	case ID_CL_PLUSMINUS:
	{

		HWND *Outputs = (HWND*)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		WCHAR Result[MAX_CLTITLE_CHAR] = { 0 };
		INT ResultLength = GetWindowText(*(Outputs + 1), Result, ARRAYSIZE(Result));

		if (lstrcmpW(Result, WINFINITY) == 0 || lstrcmpW(Result, WDEVISION_BY_ZERO) == 0) {
			wcscpy_s(Result, L"-0");
			SetWindowText(*(Outputs + 1), Result);
			break;
		}

		if (FindChar(Result, L'-', ResultLength) == -1) {
			WCHAR ResultMinus[MAX_CLTITLE_CHAR] = { 0 };
			wcscpy_s(ResultMinus, L"-");
			wcscat_s(ResultMinus, Result);
			SetWindowText(*(Outputs + 1), ResultMinus);
		}
		else {
			WCHAR ResultPlus[MAX_CLTITLE_CHAR] = { 0 };
			for (int i = 0; i < ResultLength - 1; i++) {
				ResultPlus[i] = Result[i + 1];
			}
			SetWindowText(*(Outputs + 1), ResultPlus);
		}

		break;

	}
	case ID_CL_MULTIPLY: case ID_CL_DEVIDE: case ID_CL_SUM: case ID_CL_MINUS: case ID_CL_MODULE:
	{

		HWND *Outputs = (HWND*)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		WCHAR Operator[MAX_CLTITLE_CHAR] = { 0 };
		GetWindowText((HWND)lParam, Operator, ARRAYSIZE(Operator));
		WCHAR Result[MAX_CLTITLE_CHAR] = { 0 };
		INT ResultLength = GetWindowText(*(Outputs + 1), Result, ARRAYSIZE(Result));

		if (Result[ResultLength - 1] != L'.' && lstrcmpW(Result, WINFINITY) != 0 && lstrcmpW(Result, WDEVISION_BY_ZERO) != 0) {

			wcscat_s(Result, L" "), wcscat_s(Result, Operator);

			SetWindowText(*(Outputs + 0), Result);
			SetWindowText(*(Outputs + 1), L"0");

		}
		else {
			MessageBeep(MB_ICONINFORMATION);
		}

		break;

	}
	case ID_CL_RESULT:
	{

		HWND *Outputs = (HWND*)GetWindowLongPtr(hCalculator, GWLP_USERDATA);

		WCHAR Operation[MAX_CLTITLE_CHAR] = { 0 };
		INT OperationLength = GetWindowText(*(Outputs + 0), Operation, ARRAYSIZE(Operation));
		WCHAR Result[MAX_CLTITLE_CHAR] = { 0 };
		INT ResultLength = GetWindowText(*(Outputs + 1), Result, ARRAYSIZE(Result));

		if (Result[ResultLength - 1] != L'.' && Operation[0] != L'\0' && lstrcmpW(Result, WINFINITY) != 0 && lstrcmpW(Result, WDEVISION_BY_ZERO) != 0) {
			if (FindChar(Operation, L'x', OperationLength) != -1) {
				INT OperatorPosition = Calculator::FindChar(Operation, L'x', lstrlenW(Operation));
				for (int i = OperatorPosition + 1; i < lstrlenW(Operation); i++) {
					Operation[i] = L'\0';
				}
				DOUBLE Num1 = Calculator::_atod(Operation);
				DOUBLE Num2 = Calculator::_atod(Result);
				DOUBLE Multiply = Num1 * Num2;
				wcscat_s(Operation, L" "), wcscat_s(Operation, Result), wcscat_s(Operation, L" =");
				std::wstring WMultiply = std::to_wstring(Multiply);
				wcscpy_s(Result, Calculator::RoundDouble((LPWSTR)WMultiply.c_str(), (int)WMultiply.size()));
				SetWindowText(*(Outputs + 0), Operation);
				SetWindowText(*(Outputs + 1), Result);
				break;
			}
			if (FindChar(Operation, L'/', OperationLength) != -1) {
				INT OperatorPosition = Calculator::FindChar(Operation, L'/', lstrlenW(Operation));
				for (int i = OperatorPosition + 1; i < lstrlenW(Operation); i++) {
					Operation[i] = L'\0';
				}
				DOUBLE Num1 = Calculator::_atod(Operation);
				DOUBLE Num2 = Calculator::_atod(Result);
				DOUBLE Devide = 0;
				if (Num1 == 0 && Num2 == 0) {
					wcscat_s(Operation, L" "), wcscat_s(Operation, Result), wcscat_s(Operation, L" =");
					wcscpy_s(Result, WINFINITY);
				}
				else if (Num2 == 0) {
					wcscat_s(Operation, L" "), wcscat_s(Operation, Result), wcscat_s(Operation, L" =");
					wcscpy_s(Result, WDEVISION_BY_ZERO);
				}
				else {
					Devide = Num1 / Num2;
					wcscat_s(Operation, L" "), wcscat_s(Operation, Result), wcscat_s(Operation, L" =");
					std::wstring WDevide = std::to_wstring(Devide);
					wcscpy_s(Result, Calculator::RoundDouble((LPWSTR)WDevide.c_str(), (int)WDevide.size()));
				}
				SetWindowText(*(Outputs + 0), Operation);
				SetWindowText(*(Outputs + 1), Result);
				break;
			}
			if (FindChar(Operation, L'+', OperationLength) != -1) {
				INT OperatorPosition = Calculator::FindChar(Operation, L'+', lstrlenW(Operation));
				for (int i = OperatorPosition + 1; i < lstrlenW(Operation); i++) {
					Operation[i] = L'\0';
				}
				DOUBLE Num1 = Calculator::_atod(Operation);
				DOUBLE Num2 = Calculator::_atod(Result);
				DOUBLE Sum = Num1 + Num2;
				wcscat_s(Operation, L" "), wcscat_s(Operation, Result), wcscat_s(Operation, L" =");
				std::wstring WSum = std::to_wstring(Sum);
				wcscpy_s(Result, Calculator::RoundDouble((LPWSTR)WSum.c_str(), (int)WSum.size()));
				SetWindowText(*(Outputs + 0), Operation);
				SetWindowText(*(Outputs + 1), Result);
				break;
			}
			if (FindChar(Operation, L'-', OperationLength) != -1) {
				INT OperatorPosition = Calculator::FindChar(Operation, L'-', lstrlenW(Operation));
				for (int i = OperatorPosition + 1; i < lstrlenW(Operation); i++) {
					Operation[i] = L'\0';
				}
				DOUBLE Num1 = Calculator::_atod(Operation);
				DOUBLE Num2 = Calculator::_atod(Result);
				DOUBLE Minus = Num1 - Num2;
				wcscat_s(Operation, L" "), wcscat_s(Operation, Result), wcscat_s(Operation, L" =");
				std::wstring WMinus = std::to_wstring(Minus);
				wcscpy_s(Result, Calculator::RoundDouble((LPWSTR)WMinus.c_str(), (int)WMinus.size()));
				SetWindowText(*(Outputs + 0), Operation);
				SetWindowText(*(Outputs + 1), Result);
				break;
			}
			if (FindChar(Operation, L'%', OperationLength) != -1) {
				INT OperatorPosition = Calculator::FindChar(Operation, L'%', lstrlenW(Operation));
				for (int i = OperatorPosition + 1; i < lstrlenW(Operation); i++) {
					Operation[i] = L'\0';
				}
				DOUBLE Num1 = Calculator::_atod(Operation);
				DOUBLE Num2 = Calculator::_atod(Result);
				INT Module = 0;
				if (Num1 == 0 && Num2 == 0) {
					wcscat_s(Operation, L" "), wcscat_s(Operation, Result), wcscat_s(Operation, L" =");
					wcscpy_s(Result, WINFINITY);
				}
				else if (Num2 == 0) {
					wcscat_s(Operation, L" "), wcscat_s(Operation, Result), wcscat_s(Operation, L" =");
					wcscpy_s(Result, WDEVISION_BY_ZERO);
				}
				else {
					Module = (int)Num1 % (int)Num2;
					wcscat_s(Operation, L" "), wcscat_s(Operation, Result), wcscat_s(Operation, L" =");
					std::wstring WModule = std::to_wstring(Module);
					wcscpy_s(Result, Calculator::RoundDouble((LPWSTR)WModule.c_str(), (int)WModule.size()));
				}
				SetWindowText(*(Outputs + 0), Operation);
				SetWindowText(*(Outputs + 1), Result);
				break;
			}
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
		HWND *Outputs = (HWND*)GetWindowLongPtr(hCalculator, GWLP_USERDATA);
		delete[] Outputs;
		return 0;
	}
	}

	return DefWindowProc(hCalculator, Msg, wParam, lParam);

};

#pragma endregion
