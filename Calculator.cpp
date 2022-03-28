#include "Calculator.h"

// INIT CALCULATOR STATIC MEMBERS

#pragma region InitCalculatorStaticMembers

HDC Calculator::CalculatorDC = { 0 };
PAINTSTRUCT Calculator::ps = { 0 };

HDC Calculator::MemoryDC = { 0 };
HBITMAP Calculator::Bitmap = { 0 };

HBRUSH Calculator::CalculatorBackroundBrush = CreateSolidBrush(CalculatorBackgroundColor);

HFONT Calculator::CalculatorFont = { 0 };

std::vector <HMENU> Calculator::CalculatorControlsID = { (HMENU)ID_CL_CLOSE, (HMENU)ID_CL_OPERATION, (HMENU)ID_CL_OUTPUT_RESULT,
		(HMENU)ID_CL_MULTIPLY, (HMENU)ID_CL_DEVIDE, (HMENU)ID_CL_CLEAR, (HMENU)ID_CL_BACK,
		(HMENU)ID_CL_NUM1, (HMENU)ID_CL_NUM2, (HMENU)ID_CL_NUM3, (HMENU)ID_CL_SUM,
		(HMENU)ID_CL_NUM4, (HMENU)ID_CL_NUM5, (HMENU)ID_CL_NUM6, (HMENU)ID_CL_MINUS,
		(HMENU)ID_CL_NUM7, (HMENU)ID_CL_NUM8, (HMENU)ID_CL_NUM9, (HMENU)ID_CL_PLUSMINUS,
		(HMENU)ID_CL_MODULE, (HMENU)ID_CL_NUM0, (HMENU)ID_CL_DOT, (HMENU)ID_CL_RESULT };

CONST POINT Calculator::CalculatorDimensions = { 195, 340 };

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
	
	createCalculatorFont(CalculatorFont);

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

VOID Calculator::createCalculatorFont(HFONT Font) {

	Font = CreateFont(32, 0, 0, 0,
		FW_BOLD,
		false,
		false,
		false,
		ANSI_FIXED_FONT,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		VARIABLE_PITCH,
		L"Times New Roman");

}

VOID Calculator::createCalculatorControls(HWND hCalculator) {

	CONST INT PADDING = 5;
	INT BUTTONW = 40, STATICW = CalculatorDimensions.x - PADDING * 4, BUTTONH = 40, STATICH = 25;

	INT XISTATIC = 0, XIBUTTON = 0, YIBUTTON = 0;

	std::vector <HWND> *CalculatorControls = (std::vector <HWND>*)GetWindowLongPtr(hCalculator, GWLP_USERDATA);
	std::vector <std::wstring> Captions = { L"X", L"", L"",
		L"*", L"/", L"CE", L"<-",
		L"1", L"2", L"3", L"+",
		L"4", L"5", L"6", L"-",
		L"7", L"8", L"9", L"+/-",
		L"%", L"0", L".", L"="};

	// ID_CL_OPERATION | ID_CL_OUTPUT_RESULT

	std::vector <INT> STATICY = { 0 + PADDING * 2 + BUTTONH, 0 + PADDING * 3 + STATICH + BUTTONH };

	// OTHER CONTROLS

	std::vector <INT> BUTTONX = { 0 + PADDING, 0 + PADDING * 2 + BUTTONW, 0 + PADDING * 3 + BUTTONW * 2, 0 + PADDING * 4 + BUTTONW * 3 };
	std::vector <INT> BUTTONY = { 0 + PADDING * 4 + STATICH * 2 + BUTTONH, 0 + PADDING * 5 + STATICH * 2 + BUTTONH * 2, 0 + PADDING * 6 + STATICH * 2 + BUTTONH * 3, 0 + PADDING * 7 + STATICH * 2 + BUTTONH * 4, 0 + PADDING * 8 + STATICH * 2 + BUTTONH * 5 };

	for (int i = 0; i < CalculatorControlsID.size(); i++) {

		if (i == 0) {

			// ID_CL_CLOSE
		
			CalculatorControls->push_back(CreateWindowEx(WS_EX_STATICEDGE,
				L"BUTTON",
				Captions[i].c_str(),
				WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER | BS_VCENTER,
				CalculatorDimensions.x - BUTTONW - PADDING * 3, 0 + PADDING, BUTTONW, BUTTONH,
				hCalculator,
				CalculatorControlsID[i],
				HInstance(),
				NULL));

			SetFont(CalculatorControls->front() + i, CalculatorFont);

		}
		else if (i == 1 || i == 2) {

			// ID_CL_OPERATION | ID_CL_OUTPUT_RESULT

			CalculatorControls->push_back(CreateWindowEx(WS_EX_STATICEDGE,
				L"STATIC",
				Captions[i].c_str(),
				WS_CHILD | WS_BORDER | WS_VISIBLE | SS_OWNERDRAW,
				0 + PADDING, STATICY[XISTATIC], STATICW, STATICH,
				hCalculator,
				CalculatorControlsID[i],
				HInstance(),
				NULL));

			XISTATIC++;

			SetFont(CalculatorControls->front() + i, CalculatorFont);

		}
		else {

			// OTHER CONTROLS

			CalculatorControls->push_back(CreateWindowEx(WS_EX_STATICEDGE,
				L"BUTTON",
				Captions[i].c_str(),
				WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER | BS_VCENTER,
				BUTTONX[XIBUTTON], BUTTONY[YIBUTTON], BUTTONW, BUTTONH,
				hCalculator,
				CalculatorControlsID[i],
				HInstance(),
				NULL));

			XIBUTTON++;
			if (XIBUTTON == BUTTONX.size()) {
				XIBUTTON = 0;
				YIBUTTON++;
			}

			SetFont(CalculatorControls->front() + i, CalculatorFont);

		}

	}


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

		std::vector <HWND> *CalculatorControls = new std::vector <HWND>;
		if (CalculatorControls != NULL) {
			SetWindowLongPtr(hCalculator, GWLP_USERDATA, (LONG_PTR)CalculatorControls);
			createCalculatorControls(hCalculator);
		}
		else {
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
	TextOut(MemoryDC, 0 + 5, 5 + 40 / 2 - size.cy / 2, WindowTitle, lstrlenW(WindowTitle));

	BitBlt(CalculatorDC, 0, 0, Dimensions.right, Dimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	DeleteDC(MemoryDC);
	DeleteObject(Bitmap);

	EndPaint(hCalculator, &ps);
	
}

VOID Calculator::onCommand(HWND hCalculator, WPARAM wParam, LPARAM lParam) {

	switch (LOWORD(wParam)) {
	case ID_CL_CLOSE:
	{
		DestroyWindow(hCalculator);
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
		std::vector <HWND> *CalculatorControls = (std::vector <HWND>*)GetWindowLongPtr(hCalculator, GWLP_USERDATA);
		delete CalculatorControls;
		return 0;
	}
	}

	return DefWindowProc(hCalculator, Msg, wParam, lParam);

};

#pragma endregion
