#include "InitMainWindow.h"

// INIT MAIN WINDOW STATIC MEMBERS

#pragma region InitMainWindowStaticMembers

PAINTSTRUCT MainWindow::MainPS = { 0 };
HDC MainWindow::MainWindowDC = { 0 };

HDC MainWindow::MemoryDC = { 0 };
HBITMAP MainWindow::MainBitmap = { 0 };

HBRUSH MainWindow::MainWindowBackgroundBrush = CreateSolidBrush(MainWindowBackgroundColor);

HFONT MainWindow::MainFont = { 0 };

HWND MainWindow::hMainWindow = { 0 };
RECT MainWindow::MainWindowDimensions = { 0, 0, MainWindowWidth, MainWindowHeight };

HWND MainWindow::hDebugTool1 = { 0 };
HWND MainWindow::hDebugTool2 = { 0 };

POINT MainWindow::mousePosition = { 0 };

#pragma endregion

// INIT MAIN WINDOW

#pragma region InitMainWindow

BOOL MainWindow::InitMainWindowClass(std::wstring ClassName) {

	SetLastError(0);

	GetCurrentDirectory(ARRAYSIZE(ApplicationDirectory), ApplicationDirectory);

	WCHAR CursorPath[MAX_CHAR_STRING] = { 0 };

	wcscpy_s(CursorPath, ApplicationDirectory);
	wcscat_s(CursorPath, MAIN_WINDOW_CURSOR);

	WCHAR IconPath[MAX_CHAR_STRING] = { 0 };

	wcscpy_s(IconPath, ApplicationDirectory);
	wcscat_s(IconPath, MAIN_WINDOW_ICON);

	WNDCLASSEX mainwcex = { 0 };

	mainwcex.cbClsExtra = 0;
	mainwcex.cbWndExtra = 0;
	mainwcex.cbSize = sizeof(WNDCLASSEX);
	mainwcex.hbrBackground = MainWindowBackgroundBrush;
	mainwcex.hCursor = LoadCursorFromFile(CursorPath);
	mainwcex.hIcon = (HICON)LoadImage(NULL, IconPath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	mainwcex.hIconSm = (HICON)LoadImage(NULL, IconPath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	mainwcex.hInstance = HInstance();
	mainwcex.lpfnWndProc = MainWindowProcedure;
	mainwcex.lpszClassName = ClassName.c_str();
	mainwcex.lpszMenuName = NULL;
	mainwcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_GLOBALCLASS;

	if (!RegisterClassEx(&mainwcex)) {
		Functions::ShowError(HWND_DESKTOP, L" - Main Window Class not Created!");
		RETURN FALSE;
	}

	RETURN TRUE;

}

BOOL MainWindow::CreateMainWindow(std::wstring ClassName, std::wstring WindowTitle) {

	SetLastError(0);

	INT ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	INT ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	hMainWindow = CreateWindowEx(NULL,
		ClassName.c_str(),
		WindowTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		ScreenWidth / 2 - MainWindowDimensions.right / 2,
		ScreenHeight / 2 - MainWindowDimensions.bottom / 2,
		MainWindowDimensions.right,
		MainWindowDimensions.bottom,
		HWND_DESKTOP,
		NULL,
		HInstance(),
		NULL);

	if (hMainWindow == NULL) {
		Functions::ShowError(HWND_DESKTOP, L" - Main Window not Created!");
		RETURN FALSE;
	}

	ShowWindow(hMainWindow, SW_SHOW);
	UpdateWindow(hMainWindow);

	RETURN TRUE;

}

#pragma endregion

// OVERLOADED OPERATORS

#pragma region OverloadedOperators

BOOL operator==(POINT &Left, POINT &Right) {

	if (Left.x == Right.x && Left.y == Right.y) {
		return TRUE;
	}
	else {
		return FALSE;
	}

}

BOOL operator==(POINT &mousePosition, RECT &Rectangle) {

	if (mousePosition.x >= Rectangle.left && mousePosition.x <= Rectangle.right &&
		mousePosition.y >= Rectangle.top && mousePosition.y >= Rectangle.bottom) {
		return TRUE;
	}
	else {
		return FALSE;
	}

}

#pragma endregion

// FUNCTIONS

#pragma region Functions

VOID MainWindow::CreateFonts() {

	MainFont = CreateFont(20, 0, 0, 0,
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

VOID MainWindow::CreateDebugTools() {

	SetLastError(0);

	std::vector <HWND> DebugTools = { hDebugTool1, hDebugTool2 };
	std::vector <std::wstring> Captions = { L"X = 0 Y = 0", L"Width = 0 Height = 0" };
	std::vector <HMENU> DebugToolsID = { (HMENU)ID_DEBUG_TOOL_1, (HMENU)ID_DEBUG_TOOL_2 };

	for (int i = 0; i < DebugTools.size(); i++) {

		if (!(DebugTools[i] = CreateWindowEx(WS_EX_CLIENTEDGE,
			L"STATIC",
			Captions[i].c_str(),
			WS_CHILD | WS_BORDER | WS_VISIBLE | SS_OWNERDRAW,
			0, 0, 0, 0,
			hMainWindow,
			DebugToolsID[i],
			HInstance(),
			NULL))) {
			Functions::ShowError(hMainWindow, L" - Child Window not Created!");
			PostQuitMessage(0);
		}

		SetFont(DebugTools[i], MainFont);

	}

	hDebugTool1 = DebugTools[0], hDebugTool2 = DebugTools[1];

}

#pragma endregion

// EVENTS

#pragma region Events

VOID MainWindow::onCreate(HWND hMainWindow, LPARAM lParam) {

	MainWindow::hMainWindow = hMainWindow;

	// INIT CUSTOM CONTROLS
	ColorPicker::InitColorPicker();
	AnimationStars::InitAnimationStars();
	Calculator::InitCalculator();
	DropFiles::InitDropFiles();
	////

	CreateFonts();
	CreateDebugTools();

	ASSTYLES asstyles = { 0 };
	asstyles.StarColor = RGB(0, 155, 255);
	asstyles.Proportion = 4;

	DFSTYLES dfstyles = { 0 };
	dfstyles.BackgroundColor = RGB(250, 250, 0);
	dfstyles.TextColor = RGB(0, 0, 255);

	CreateWindowEx(WS_EX_STATICEDGE, L"ANIMATION STARS", L"STARS", WS_CHILD | WS_BORDER | WS_VISIBLE, 5, 5, 140, 140, hMainWindow, (HMENU)ID_ANIMATION_STARS, HInstance(), &asstyles);
	CreateWindowEx(WS_EX_STATICEDGE, L"DROP FILES", L"", WS_CHILD | WS_BORDER | WS_VISIBLE, 260, 110, 220, 120, hMainWindow, (HMENU)ID_DROP_FILES, HInstance(), &dfstyles);
	CreateWindowEx(WS_EX_STATICEDGE, L"COLOR PICKER", L"LARGE", WS_CHILD | WS_BORDER | WS_VISIBLE, 150, 5, CP_SHOW, CP_SHOW, hMainWindow, (HMENU)ID_COLOR_PICKER, HInstance(), NULL);
	CreateWindowEx(WS_EX_STATICEDGE, L"CALCULATOR", L"SUPER CALCULATOR", WS_CHILD | WS_BORDER | WS_VISIBLE, 5, 150, CL_SHOW, CL_SHOW, hMainWindow, (HMENU)ID_CALCULATOR, HInstance(), NULL);
	
}

VOID MainWindow::onSize(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	MainWindowDimensions.right = LOWORD(lParam);
	MainWindowDimensions.bottom = HIWORD(lParam);

	#pragma region DebugTool2

	SetWindowPos(hDebugTool1, NULL, MainWindowDimensions.right - 160, 0, 160, 25, SWP_SHOWWINDOW);
	SetWindowPos(hDebugTool2, NULL, MainWindowDimensions.right - 240, 30, 240, 25, SWP_SHOWWINDOW);

	std::wstring WMainWindowDimensions = L"Width = " + Functions::_itow(MainWindowDimensions.right) + L" Height = " + Functions::_itow(MainWindowDimensions.bottom);
	SetWindowText(hDebugTool2, WMainWindowDimensions.c_str());

	#pragma endregion

	RedrawWindow(hMainWindow, &MainWindowDimensions, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);

}

VOID MainWindow::onMouseMove(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	GetCursorPos(&mousePosition);
	ScreenToClient(hMainWindow, &mousePosition);

	#pragma region DebugTool1

	std::wstring WMousePosition = L"X = " + Functions::_itow(mousePosition.x) + L" Y = " + Functions::_itow(mousePosition.y);
	SetWindowText(hDebugTool1, WMousePosition.c_str());

	#pragma endregion

}

VOID MainWindow::onDrawItem(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lParam;

	#pragma region DebugTools

	if (item->CtlID == ID_DEBUG_TOOL_1 || item->CtlID == ID_DEBUG_TOOL_2) {

		SIZE size = { 0 };
		WCHAR StaticText[MAX_CHAR_STRING] = { 0 };
		HBRUSH ItemBrush = CreateSolidBrush(GREEN_COLOR);
		FillRect(item->hDC, &item->rcItem, ItemBrush);
		DeleteObject(ItemBrush);
		SetBkMode(item->hDC, TRANSPARENT);
		SetTextColor(item->hDC, WHITE_COLOR);
		SetFont(item->hwndItem, MainFont);
		INT TextLength = GetWindowText(item->hwndItem, StaticText, ARRAYSIZE(StaticText));
		GetTextExtentPoint(item->hDC, StaticText, TextLength, &size);
		TextOut(item->hDC, item->rcItem.right / 2 - size.cx / 2, item->rcItem.bottom / 2 - size.cy / 2, StaticText, TextLength);

	}

	#pragma endregion

}

VOID MainWindow::onPaint(HWND hMainWindow) {

	MainWindowDC = BeginPaint(hMainWindow, &MainPS);

	MemoryDC = CreateCompatibleDC(MainWindowDC);
	MainBitmap = CreateCompatibleBitmap(MainWindowDC, MainWindowDimensions.right, MainWindowDimensions.bottom);

	SelectObject(MemoryDC, MainBitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	FillRect(MemoryDC, &MainWindowDimensions, MainWindowBackgroundBrush);

	SelectObject(MemoryDC, MainFont);

	Draw::drawGradientSmall(MemoryDC, MainWindowDimensions.right / 2 - 420 / 2, MainWindowDimensions.bottom / 2 - 40 / 2);

	SIZE size = { 0 };
	WCHAR Text[] = L"Hello World!";
	GetTextExtentPoint(MainWindowDC, Text, ARRAYSIZE(Text), &size);
	TextOut(MemoryDC, MainWindowDimensions.right / 2 - size.cx / 2, MainWindowDimensions.bottom / 2 - size.cy / 2, Text, ARRAYSIZE(Text) - 1);

	BitBlt(MainWindowDC, 0, 0, MainWindowDimensions.right, MainWindowDimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	DeleteDC(MemoryDC);
	DeleteObject(MainBitmap);

	EndPaint(hMainWindow, &MainPS);

}

VOID MainWindow::onCommand(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	#define ID_DEFAULT 0xFFFF

	switch (LOWORD(wParam)) {
	case ID_COLOR_PICKER:
	{

		COLORREF Color = (COLORREF)lParam;

		LOGW("R: " << Functions::_itow(GetGValue(Color)) << "\tG: " << Functions::_itow(GetGValue(Color)) << "\tB: " << Functions::_itow(GetBValue(Color)));

		break;

	}
	case ID_DROP_FILES:
	{

		HDROP DropedFiles = (HDROP)lParam;
		CHAR Buffer[MAX_CHAR_STRING] = { 0 };

		UINT FileCount = DragQueryFileA(DropedFiles, 0xFFFFFFFF, Buffer, ARRAYSIZE(Buffer));

		for (UINT counter = 0; counter < FileCount; counter++) {
			DragQueryFileA(DropedFiles, counter, Buffer, ARRAYSIZE(Buffer));
			LOG(Buffer);
		}

		break;

	}
	}

}

#pragma endregion

// MAIN WINDOW PROCEDURE

#pragma region MainWindowProcedure

LRESULT CALLBACK MainWindow::MainWindowProcedure(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{
		onCreate(hMainWindow, lParam);
		RETURN 0;
	}
	case WM_SIZE:
	{
		onSize(hMainWindow, wParam, lParam);
		RETURN 0;
	}
	case WM_MOUSEMOVE:
	{
		onMouseMove(hMainWindow, wParam, lParam);
		RETURN 0;
	}
	case WM_GETMINMAXINFO:
	{

		LPMINMAXINFO minmax = (LPMINMAXINFO)lParam;
		POINT minsize = { MainWindowWidth, MainWindowHeight };
		minmax->ptMinTrackSize = minsize;

		RETURN 0;
	}
	case WM_DRAWITEM:
	{
		onDrawItem(hMainWindow, wParam, lParam);
		RETURN 0;
	}
	case WM_PAINT:
	{
		onPaint(hMainWindow);
		RETURN 0;
	}
	case WM_COMMAND:
	{
		onCommand(hMainWindow, wParam, lParam);
		RETURN 0;
	}
	case WM_CLOSE:
	{
		IF(MessageBox(hMainWindow, L"Are You Serious!", L"INFORMATION", MB_YESNO | MB_ICONINFORMATION | MB_DEFBUTTON2) == IDYES) {
			DestroyWindow(hMainWindow);
		}
		RETURN 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		RETURN 0;
	}
	}

	RETURN DefWindowProc(hMainWindow, Msg, wParam, lParam);

}

#pragma endregion
