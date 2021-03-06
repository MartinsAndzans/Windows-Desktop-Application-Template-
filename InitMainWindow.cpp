#include "InitMainWindow.h"

#pragma region InitMainWindowStaticMembers
PAINTSTRUCT MainWindow::ps = { 0 };
HDC MainWindow::MainWindowDC = { 0 };

HDC MainWindow::MemoryDC = { 0 };
HBITMAP MainWindow::MainBitmap = { 0 };

HFONT MainWindow::MainFont = { 0 };
HBRUSH MainWindow::MainWindowBackgroundBrush = CreateSolidBrush(MainWindowBackgroundColor);
POINT MainWindow::mousePosition = { 0 };

HWND MainWindow::hMainWindow = { 0 };
RECT MainWindow::MainWindowDimensions = { 0 };

HWND MainWindow::hDebugTool1 = nullptr;
HWND MainWindow::hDebugTool2 = nullptr;

HWND MainWindow::Test = nullptr;
#pragma endregion

#pragma region InitMainWindow
BOOL MainWindow::RegisterMainWindowClass(LPCWSTR ClassName) {

	CreateMainWindowFont();

	WNDCLASSEX mainwcex = { 0 };

	mainwcex.cbClsExtra = 0;
	mainwcex.cbWndExtra = 0;
	mainwcex.cbSize = sizeof(WNDCLASSEX);
	mainwcex.hbrBackground = MainWindowBackgroundBrush;
	mainwcex.hCursor = LoadCursor(HInstance(), MAKEINTRESOURCE(IDC_MAINWINDOWCURSOR));
	mainwcex.hIcon = LoadIcon(HInstance(), MAKEINTRESOURCE(IDI_MAINWINDOWICON));
	mainwcex.hIcon = LoadIcon(HInstance(), MAKEINTRESOURCEW(IDI_MAINWINDOWICON));
	mainwcex.hInstance = HInstance();
	mainwcex.lpfnWndProc = MainWindowProcedure;
	mainwcex.lpszClassName = ClassName;
	mainwcex.lpszMenuName = NULL;
	mainwcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_GLOBALCLASS;
	
	if (!RegisterClassEx(&mainwcex)) {
		Functions::ShowLastError(HWND_DESKTOP, " - Main Window Class not Created!");
		return FALSE;
	}

	return TRUE;

}

BOOL MainWindow::CreateMainWindow(LPCWSTR ClassName, LPCWSTR WindowTitle) {

	INT ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	INT ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (!(hMainWindow = CreateWindowEx(NULL,
		ClassName,
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		ScreenWidth / 2 - MainWindowWidth / 2,
		ScreenHeight / 2 - MainWindowHeight / 2,
		MainWindowWidth,
		MainWindowHeight,
		HWND_DESKTOP,
		NULL,
		HInstance(),
		nullptr))) {
		Functions::ShowLastError(HWND_DESKTOP, " - Main Window not Created!");
		return FALSE;
	}

	ShowWindow(hMainWindow, SW_SHOW);
	UpdateWindow(hMainWindow);

	return TRUE;

}
#pragma endregion

#pragma region OverloadedOperators
BOOL operator==(POINT &Left, POINT &Right) {

	if (Left.x == Right.x and Left.y == Right.y) {
		return TRUE;
	}
	return FALSE;

}
#pragma endregion

#pragma region Functions
VOID MainWindow::CreateMainWindowFont() {

	MainFont = CreateFont(20, 0, 0, 0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		ANSI_FIXED_FONT,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		VARIABLE_PITCH,
		L"Segoe UI");

}

VOID MainWindow::CreateDebugTools(HWND ParentWindow) {

	std::vector <HWND*> DebugTools = { &hDebugTool1, &hDebugTool2 };
	std::vector <std::string> Captions = { "X = 0 Y = 0", "Width = 0 Height = 0" };
	std::vector <SHORT> DebugToolsID = { ID_DEBUG_TOOL_1, ID_DEBUG_TOOL_2 };

	for (SIZE_T I = 0; I < DebugTools.size(); I++) {

		if (!(*DebugTools[I] = CreateWindowExA(WS_EX_CLIENTEDGE,
			"STATIC",
			Captions[I].c_str(),
			WS_CHILD | WS_BORDER | WS_VISIBLE | SS_OWNERDRAW,
			0, 0, 0, 0,
			ParentWindow,
			reinterpret_cast<HMENU>(DebugToolsID[I]),
			HInstance(),
			nullptr))) {
			Functions::ShowLastError(hMainWindow, " - Child Window not Created!");
			PostQuitMessage(0);
		}

		SetFont(*DebugTools[I], MainFont);

	}

}
#pragma endregion

#pragma region Events
VOID MainWindow::onCreate(HWND hMainWindow, LPARAM lParam) {

	#pragma region RegisterCustomControls
	Animation::RegisterAnimationClass();
	DropFiles::RegisterDropFilesClass();
	ColorPicker::RegisterColorPickerClass();
	Calculator::RegisterCalculatorClass();
	#pragma endregion

	CreateDebugTools(hMainWindow);

	#pragma region Examples
	Animation::AnimationStyle as = { 0 };
	as.SymbolColor = Graphics::Colors::Orange;
	as.Proportion = 4;
	as.Symbol = '+';

	DropFiles::DropFilesStyle dfs = { 0 };
	dfs.BackgroundColor = Graphics::Colors::Orange;
	dfs.ForegroundColor = Graphics::Colors::Blue;

	CreateWindowEx(WS_EX_STATICEDGE, L"Animation", L"Animation", WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 10, 140, 140, hMainWindow, reinterpret_cast<HMENU>(ControlsIDs::ID_ANIMATION_STARS), HInstance(), &as);
	CreateWindowEx(WS_EX_STATICEDGE, L"DropFiles", L"Drop File/s Here", WS_CHILD | WS_BORDER | WS_VISIBLE, 270, 120, 240, 140, hMainWindow, reinterpret_cast<HMENU>(ControlsIDs::ID_DROP_FILES), HInstance(), &dfs);
	CreateWindowEx(WS_EX_STATICEDGE, L"ColorPicker", L"Large", WS_CHILD | WS_BORDER | WS_VISIBLE, 160, 10, ColorPicker::SHOW, ColorPicker::SHOW, hMainWindow, reinterpret_cast<HMENU>(ControlsIDs::ID_COLOR_PICKER), HInstance(), nullptr);
	CreateWindowEx(WS_EX_STATICEDGE, L"Calculator", L"", WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 160, Calculator::SHOW, Calculator::SHOW, hMainWindow, reinterpret_cast<HMENU>(ControlsIDs::ID_CALCULATOR), HInstance(), nullptr);
	#pragma endregion

	Test = CreateWindow(L"BUTTON", L"Click", WS_CHILD | WS_BORDER | WS_VISIBLE, 0, 0, 0, 0, hMainWindow, reinterpret_cast<HMENU>(1111), HInstance(), nullptr);

}

VOID MainWindow::onSize(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	MainWindowDimensions.right = LOWORD(lParam);
	MainWindowDimensions.bottom = HIWORD(lParam);

	SetWindowPos(Test, nullptr, MainWindowDimensions.right / 2 - 50, MainWindowDimensions.bottom / 2 - 80, 100, 40, SWP_SHOWWINDOW);

	SetWindowPos(hDebugTool1, nullptr, MainWindowDimensions.right - 160, 0, 160, 25, SWP_SHOWWINDOW);
	SetWindowPos(hDebugTool2, nullptr, MainWindowDimensions.right - 240, 30, 240, 25, SWP_SHOWWINDOW);

	#pragma region DebugTool2
	std::string MainWindowDimensionsWH = "Width = " + std::to_string(MainWindowDimensions.right) + " Height = " + std::to_string(MainWindowDimensions.bottom);
	SetWindowTextA(hDebugTool2, MainWindowDimensionsWH.c_str());
	#pragma endregion

	RedrawWindow(hMainWindow, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);

}

VOID MainWindow::onMouseMove(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	mousePosition.x = LOWORD(lParam);
	mousePosition.y = HIWORD(lParam);

	#pragma region DebugTool1
	std::string MousePositionXY = "X = " + std::to_string(mousePosition.x) + " Y = " + std::to_string(mousePosition.y);
	SetWindowTextA(hDebugTool1, MousePositionXY.c_str());
	#pragma endregion

}

VOID MainWindow::onDrawItem(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	LPDRAWITEMSTRUCT item = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);

	#pragma region DebugToolsStyle
	if (item->CtlID == ID_DEBUG_TOOL_1 or item->CtlID == ID_DEBUG_TOOL_2) {

		SIZE size = { 0 };
		WCHAR StaticText[MAX_CHAR_STRING] = { 0 };
		SetDCBrushColor(item->hDC, Graphics::Colors::DarkGreen);
		FillRect(item->hDC, &item->rcItem, static_cast<HBRUSH>(GetStockObject(DC_BRUSH)));
		SetBkMode(item->hDC, TRANSPARENT);
		SetTextColor(item->hDC, Graphics::Colors::White);
		INT TextLength = GetWindowText(item->hwndItem, StaticText, ARRAYSIZE(StaticText));
		GetTextExtentPoint(item->hDC, StaticText, TextLength, &size);
		TextOut(item->hDC, item->rcItem.right / 2 - size.cx / 2, item->rcItem.bottom / 2 - size.cy / 2, StaticText, TextLength);

	}
	#pragma endregion

}

VOID MainWindow::onPaint(HWND hMainWindow) {

	MainWindowDC = BeginPaint(hMainWindow, &ps);

	MemoryDC = CreateCompatibleDC(MainWindowDC);
	MainBitmap = CreateCompatibleBitmap(MainWindowDC, MainWindowDimensions.right, MainWindowDimensions.bottom);

	SelectObject(MemoryDC, MainBitmap);
	SetBkMode(MemoryDC, TRANSPARENT);
	SetDCBrushColor(MemoryDC, MainWindowBackgroundColor);
	FillRect(MemoryDC, &MainWindowDimensions, static_cast<HBRUSH>(GetStockObject(DC_BRUSH)));

	SelectObject(MemoryDC, MainFont);

	Graphics::FillSmallGradient(MemoryDC, MainWindowDimensions.right / 2 - 420 / 2, MainWindowDimensions.bottom / 2 - 40 / 2);

	SIZE size = { 0 };
	CHAR Text[] = "Hello World!";
	GetTextExtentPoint32A(MainWindowDC, Text, static_cast<INT>(strlen(Text)), &size);
	TextOutA(MemoryDC, MainWindowDimensions.right / 2 - size.cx / 2, MainWindowDimensions.bottom / 2 - size.cy / 2, Text, static_cast<INT>(strlen(Text)));

	POINT SP = { MainWindowDimensions.right / 2, MainWindowDimensions.bottom / 2 };
	POINT EP = { MainWindowDimensions.right / 2, MainWindowDimensions.bottom / 2 };

	for (SIZE_T I = 0; I < 1000; I++) {

		

	}

	//Functions::SaveBitmapToFile(MainBitmap, "File.bmp", { MainWindowDimensions.right, MainWindowDimensions.bottom });

	BitBlt(MainWindowDC, 0, 0, MainWindowDimensions.right, MainWindowDimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	DeleteDC(MemoryDC);
	DeleteObject(MainBitmap);

	EndPaint(hMainWindow, &ps);

}

VOID MainWindow::onCommand(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	switch (LOWORD(wParam)) {
	case ID_COLOR_PICKER:
	{

		std::string Color = "R: " + std::to_string(GetRValue((COLORREF)lParam)) +
			"\tG: " + std::to_string(GetGValue((COLORREF)lParam)) +
			"\tB: " + std::to_string(GetBValue((COLORREF)lParam));
		Console::Print(Color.c_str(), 0x0B);

		break;

	}
	case ID_DROP_FILES:
	{

		CHAR Buffer[MAX_CHAR_STRING] = { 0 };
		UINT FileCount = DragQueryFileA((HDROP)lParam, 0xFFFFFFFF, Buffer, ARRAYSIZE(Buffer));
		for (UINT File = 0; File < FileCount; File++) {
			DragQueryFileA((HDROP)lParam, File, Buffer, ARRAYSIZE(Buffer));
			Console::Print(Buffer, 0x09);
		}

		DragFinish((HDROP)lParam);

		break;

	}
	}

}

VOID MainWindow::onKeyDown(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {



}

VOID MainWindow::onMCINotify(HWND hMainWindow, LPARAM lParam) {



}
#pragma endregion

#pragma region MainWindowProcedure
LRESULT CALLBACK MainWindow::MainWindowProcedure(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{
		onCreate(hMainWindow, lParam);
		return 0;
	}
	case WM_SIZE:
	{
		onSize(hMainWindow, wParam, lParam);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		onMouseMove(hMainWindow, wParam, lParam);
		return 0;
	}
	case WM_GETMINMAXINFO:
	{

		LPMINMAXINFO minmax = reinterpret_cast<LPMINMAXINFO>(lParam);
		POINT minsize = { MainWindowWidth, MainWindowHeight };
		minmax->ptMinTrackSize = minsize;

		return 0;
	}
	case WM_DRAWITEM:
	{
		onDrawItem(hMainWindow, wParam, lParam);
		return 0;
	}
	case WM_PAINT:
	{
		onPaint(hMainWindow);
		return 0;
	}
	case WM_COMMAND:
	{
		onCommand(hMainWindow, wParam, lParam);
		return 0;
	}
	case WM_KEYDOWN:
	{
		onKeyDown(hMainWindow, wParam, lParam);
		return 0;
	}
	case MM_MCINOTIFY:
	{
		onMCINotify(hMainWindow, lParam);
		return 0;
	}
	case WM_CLOSE:
	{
		if (MessageBoxA(hMainWindow, "Are You Serious!", "INFORMATION", MB_YESNO | MB_ICONINFORMATION | MB_DEFBUTTON2) == IDYES) {
			mciSendCommand(MCI_ALL_DEVICE_ID, MCI_CLOSE, NULL, NULL);
			DestroyWindow(hMainWindow);
		}
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProc(hMainWindow, Msg, wParam, lParam);

}
#pragma endregion
