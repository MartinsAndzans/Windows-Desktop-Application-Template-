#include "InitMainWindow.h"

// INIT MAIN WINDOW STATIC MEMBERS

#pragma region InitMainWindowStaticMembers

PAINTSTRUCT MainWindow::MainPS = { 0 };
HDC MainWindow::MainWindowDC = { 0 };

HDC MainWindow::MemoryDC = { 0 };
HBITMAP MainWindow::MainBitmap = { 0 };

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
	mainwcex.hbrBackground = CreateSolidBrush(MainWindowBackgroundColor);
	mainwcex.hCursor = LoadCursorFromFile(CursorPath);
	mainwcex.hIcon = (HICON)LoadImage(NULL, IconPath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	mainwcex.hIconSm = (HICON)LoadImage(NULL, IconPath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	mainwcex.hInstance = HInstance();
	mainwcex.lpfnWndProc = MainWindowProcedure;
	mainwcex.lpszClassName = ClassName.c_str();
	mainwcex.lpszMenuName = NULL;
	mainwcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_GLOBALCLASS;

	if (!RegisterClassEx(&mainwcex)) {
		Functions::ShowError(HWND_DESKTOP, " - Main Window Class not Created!");
		RETURN FALSE;
	}

	RETURN TRUE;

}

BOOL MainWindow::CreateMainWindow(std::wstring ClassName, std::wstring WindowTitle) {

	SetLastError(0);

	INT ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	INT ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	hMainWindow = CreateWindowEx(WS_EX_ACCEPTFILES,
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
		Functions::ShowError(HWND_DESKTOP, " - Main Window not Created!");
		RETURN FALSE;
	}

	ShowWindow(hMainWindow, SW_SHOW);
	UpdateWindow(hMainWindow);

	RETURN TRUE;

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
			Functions::ShowError(hMainWindow, " - Child Window not Created!");
			PostQuitMessage(0);
		}

	}

	hDebugTool1 = DebugTools[0], hDebugTool2 = DebugTools[1];

}

#pragma endregion

// EVENTS

#pragma region Events

VOID MainWindow::onCreate(HWND hMainWindow, LPARAM lParam) {

	MainWindow::hMainWindow = hMainWindow;

	ColorPicker::InitColorPicker();

	AnimationStars::InitAnimationStars();

	CreateFonts();

	CreateDebugTools();

	CreateWindowEx(WS_EX_DLGMODALFRAME, L"COLOR PICKER", L"SMALL", WS_CHILD | WS_BORDER | WS_VISIBLE, 160, 10, CP_SHOW, CP_SHOW, hMainWindow, (HMENU)0, HInstance(), NULL);
	CreateWindowEx(WS_EX_DLGMODALFRAME, L"ANIMATION STARS", L"STARS", WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 10, 140, 140, hMainWindow, (HMENU)4, HInstance(), (LPVOID)RGB(0, 155, 255));

}

VOID MainWindow::onSize(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	MainWindowDimensions.right = LOWORD(lParam);
	MainWindowDimensions.bottom = HIWORD(lParam);

	#pragma region DebugTool2

	SetWindowPos(hDebugTool1, NULL, MainWindowDimensions.right - 160, 0, 160, 25, SWP_SHOWWINDOW);
	SetWindowPos(hDebugTool2, NULL, MainWindowDimensions.right - 240, 30, 240, 25, SWP_SHOWWINDOW);

	SetFont(hDebugTool1, MainFont);
	SetFont(hDebugTool2, MainFont);

	std::string SMainWindowDimensions = "Width = " + (Functions::_itos(MainWindowDimensions.right, 10)) + " Height = " + (Functions::_itos(MainWindowDimensions.bottom, 10));
	SetWindowTextA(hDebugTool2, SMainWindowDimensions.c_str());

	#pragma endregion

}

VOID MainWindow::onMouseMove(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	GetCursorPos(&mousePosition);
	ScreenToClient(hMainWindow, &mousePosition);

	#pragma region DebugTool1

	std::string SMousePosition = "X = " + (Functions::_itos(mousePosition.x, 10)) + " Y = " + (Functions::_itos(mousePosition.y, 10));
	SetWindowTextA(hDebugTool1, SMousePosition.c_str());

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
	HBRUSH BackgroundBrush = CreateSolidBrush(MainWindowBackgroundColor);
	FillRect(MemoryDC, &MainWindowDimensions, BackgroundBrush);
	DeleteObject(BackgroundBrush);

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
	case 0:
	{

		DWORD ID = (DWORD)LOWORD(wParam);
		DWORD hwnd = (DWORD)HIWORD(wParam);
		COLORREF Color = (COLORREF)lParam;

		std::cout << "ID: " << Functions::_itos(ID, 10) << "\tHWND: 0x" << Functions::_itos(hwnd, 10) << "\tCOLOR: 0x" << Functions::_itos(Color, 16) << std::endl;

		break;

	}
	}

}

VOID MainWindow::onDropFiles(HWND hMainWindow, WPARAM wParam) {

	HDROP DroppedFile = (HDROP)wParam;
	CHAR buffer[MAX_CHAR_STRING] = { 0 };
	INT FileCount = DragQueryFileA(DroppedFile, 0xFFFFFFFF, buffer, ARRAYSIZE(buffer));
	for (int i = 0; i < FileCount; i++) {
		DragQueryFileA(DroppedFile, i, buffer, ARRAYSIZE(buffer));
		std::cout << buffer << std::endl;
	}
	if (DroppedFile != NULL) {
		DragFinish(DroppedFile);
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
	case WM_DROPFILES:
	{
		onDropFiles(hMainWindow, wParam);
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
