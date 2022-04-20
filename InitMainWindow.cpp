#include "InitMainWindow.h"

#pragma region InitMainWindowStaticMembers
PAINTSTRUCT MainWindow::MainPS = { 0 };
HDC MainWindow::MainWindowDC = { 0 };

HDC MainWindow::MemoryDC = { 0 };
HBITMAP MainWindow::MainBitmap = { 0 };

HBRUSH MainWindow::MainWindowBackgroundBrush = CreateSolidBrush(MainWindowBackgroundColor);

HFONT MainWindow::MainFont = { 0 };

HWND MainWindow::hMainWindow = { 0 };
RECT MainWindow::MainWindowDimensions = { 0, 0, MainWindowWidth, MainWindowHeight };

#ifdef APP_DEBUG
HWND MainWindow::hDebugTool1 = { 0 };
HWND MainWindow::hDebugTool2 = { 0 };
#endif

POINT MainWindow::mousePosition = { 0 };
#pragma endregion

#pragma region InitMainWindow
BOOL MainWindow::InitMainWindowClass(std::wstring ClassName) {

	SetLastError(0);

	GetCurrentDirectory(ARRAYSIZE(ApplicationDirectory), ApplicationDirectory);

	WNDCLASSEX mainwcex = { 0 };

	mainwcex.cbClsExtra = 0;
	mainwcex.cbWndExtra = 0;
	mainwcex.cbSize = sizeof(WNDCLASSEX);
	mainwcex.hbrBackground = MainWindowBackgroundBrush;
	mainwcex.hCursor = LoadCursor(HInstance(), MAKEINTRESOURCE(IDC_WINDOWCURSOR));
	mainwcex.hIcon = LoadIcon(HInstance(), MAKEINTRESOURCE(IDI_WINDOWICON));
	mainwcex.hIcon = LoadIcon(HInstance(), MAKEINTRESOURCEW(IDI_WINDOWICON));
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
		Functions::ShowError(HWND_DESKTOP, " - Main Window not Created!");
		RETURN FALSE;
	}

	ShowWindow(hMainWindow, SW_SHOW);
	UpdateWindow(hMainWindow);

	RETURN TRUE;

}
#pragma endregion

#pragma region OverloadedOperators
static BOOL operator==(POINT &Left, POINT &Right) {

	if (Left.x == Right.x && Left.y == Right.y) {
		return TRUE;
	}
	else {
		return FALSE;
	}

}

static BOOL operator==(POINT &mousePosition, RECT &Rectangle) {

	if (mousePosition.x >= Rectangle.left && mousePosition.x <= Rectangle.right &&
		mousePosition.y >= Rectangle.top && mousePosition.y <= Rectangle.bottom) {
		return TRUE;
	}
	else {
		return FALSE;
	}

}
#pragma endregion

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
		L"Segoe UI");

}

#ifdef APP_DEBUG
VOID MainWindow::CreateDebugTools() {

	SetLastError(0);

	std::vector <HWND> DebugTools = { hDebugTool1, hDebugTool2 };
	std::vector <std::wstring> Captions = { L"X = 0 Y = 0", L"Width = 0 Height = 0" };
	std::vector <SHORT> DebugToolsID = { ID_DEBUG_TOOL_1, ID_DEBUG_TOOL_2 };

	for (int i = 0; i < DebugTools.size(); i++) {

		if (!(DebugTools[i] = CreateWindowEx(WS_EX_CLIENTEDGE,
			L"STATIC",
			Captions[i].c_str(),
			WS_CHILD | WS_BORDER | WS_VISIBLE | SS_OWNERDRAW,
			0, 0, 0, 0,
			hMainWindow,
			(HMENU)DebugToolsID[i],
			HInstance(),
			NULL))) {
			Functions::ShowError(hMainWindow, " - Child Window not Created!");
			PostQuitMessage(0);
		}

		SetFont(DebugTools[i], MainFont);

	}

	hDebugTool1 = DebugTools[0], hDebugTool2 = DebugTools[1];

}
#endif
#pragma endregion

#pragma region Events
VOID MainWindow::onCreate(HWND hMainWindow, LPARAM lParam) {

	MainWindow::hMainWindow = hMainWindow;

	// INIT CUSTOM CONTROLS
	Animation::InitAnimation();
	DropFiles::InitDropFiles();
	ColorPicker::InitColorPicker();
	Calculator::InitCalculator();
	////

	CreateFonts();

	#ifdef APP_DEBUG
	CreateDebugTools();
	#endif

	#pragma region Examples
	Animation::AnimationStyle as = { 0 };
	as.SymbolColor = RGB(25, 255, 60);
	as.Proportion = 4;
	as.Symbol = '+';

	DropFiles::DropFilesStyle dfs = { 0 };
	dfs.BackgroundColor = RGB(255, 195, 30);
	dfs.ForegroundColor = RGB(0, 55, 255);

	CreateWindowEx(WS_EX_STATICEDGE, L"ANIMATION", L"STARS", WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 10, 140, 140, hMainWindow, (HMENU)ID_ANIMATION_STARS, HInstance(), &as);
	CreateWindowEx(WS_EX_STATICEDGE, L"DROP FILES", L"Drop File/s Here", WS_CHILD | WS_BORDER | WS_VISIBLE, 270, 120, 240, 140, hMainWindow, (HMENU)ID_DROP_FILES, HInstance(), &dfs);
	CreateWindowEx(WS_EX_STATICEDGE, L"COLOR PICKER", L"LARGE", WS_CHILD | WS_BORDER | WS_VISIBLE, 160, 10, CP_SHOW, CP_SHOW, hMainWindow, (HMENU)ID_COLOR_PICKER, HInstance(), NULL);
	CreateWindowEx(WS_EX_STATICEDGE, L"CALCULATOR", L"SUPER CALCULATOR", WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 160, CL_SHOW, CL_SHOW, hMainWindow, (HMENU)ID_CALCULATOR, HInstance(), NULL);
	#pragma endregion

}

VOID MainWindow::onSize(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	MainWindowDimensions.right = LOWORD(lParam);
	MainWindowDimensions.bottom = HIWORD(lParam);

	#pragma region DebugTool2
	#ifdef APP_DEBUG
	SetWindowPos(hDebugTool1, NULL, MainWindowDimensions.right - 160, 0, 160, 25, SWP_SHOWWINDOW);
	SetWindowPos(hDebugTool2, NULL, MainWindowDimensions.right - 240, 30, 240, 25, SWP_SHOWWINDOW);

	std::string SMainWindowDimensions = "Width = " + Functions::_itos(MainWindowDimensions.right) + " Height = " + Functions::_itos(MainWindowDimensions.bottom);
	SetWindowTextA(hDebugTool2, SMainWindowDimensions.c_str());
	#endif
	#pragma endregion

	RedrawWindow(hMainWindow, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);

}

VOID MainWindow::onMouseMove(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	GetCursorPos(&mousePosition);
	ScreenToClient(hMainWindow, &mousePosition);

	#pragma region DebugTool1
	#ifdef APP_DEBUG
	std::string SMousePosition = "X = " + Functions::_itos(mousePosition.x) + " Y = " + Functions::_itos(mousePosition.y);
	SetWindowTextA(hDebugTool1, SMousePosition.c_str());
	#endif
	#pragma endregion

}

VOID MainWindow::onDrawItem(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lParam;

	#pragma region DebugTools
	#ifdef APP_DEBUG
	if (item->CtlID == ID_DEBUG_TOOL_1 || item->CtlID == ID_DEBUG_TOOL_2) {

		SIZE size = { 0 };
		WCHAR StaticText[MAX_CHAR_STRING] = { 0 };
		SetDCBrushColor(item->hDC, DARK_GREEN_COLOR);
		FillRect(item->hDC, &item->rcItem, (HBRUSH)GetStockObject(DC_BRUSH));
		SetBkMode(item->hDC, TRANSPARENT);
		SetTextColor(item->hDC, WHITE_COLOR);
		INT TextLength = GetWindowText(item->hwndItem, StaticText, ARRAYSIZE(StaticText));
		GetTextExtentPoint(item->hDC, StaticText, TextLength, &size);
		TextOut(item->hDC, item->rcItem.right / 2 - size.cx / 2, item->rcItem.bottom / 2 - size.cy / 2, StaticText, TextLength);

	}
	#endif
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

	Draw::drawX(MemoryDC, MainWindowDimensions.right / 2 - 40 / 2, MainWindowDimensions.bottom / 4 - 40 / 2, 40, 40);

	BitBlt(MainWindowDC, 0, 0, MainWindowDimensions.right, MainWindowDimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	DeleteDC(MemoryDC);
	DeleteObject(MainBitmap);

	EndPaint(hMainWindow, &MainPS);

}

VOID MainWindow::onMCINotify(HWND hMainWindow, LPARAM lParam) {

	if (mciGetDeviceID(L"Music")) {

		Sound::Play(hMainWindow, L"Music", TRUE);

	}

}

VOID MainWindow::onCommand(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	#define DEFAULT_ID 0xFFFF

	switch (LOWORD(wParam)) {
	case ID_COLOR_PICKER:
	{

		PRINT("R: " << Functions::_itos(GetRValue((COLORREF)lParam)) <<
			"\tG: " << Functions::_itos(GetGValue((COLORREF)lParam)) <<
			"\tB: " << Functions::_itos(GetBValue((COLORREF)lParam)));

		break;

	}
	case ID_DROP_FILES:
	{

		WCHAR Buffer[MAX_CHAR_STRING] = { 0 };
		UINT FileCount = DragQueryFile((HDROP)lParam, 0xFFFFFFFF, Buffer, ARRAYSIZE(Buffer));
		for (UINT counter = 0; counter < FileCount; counter++) {
			DragQueryFile((HDROP)lParam, counter, Buffer, ARRAYSIZE(Buffer));
			PRINTW(Buffer);
		}

		DragFinish((HDROP)lParam);

		break;

	}
	}

}

VOID MainWindow::onKeyDown(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	#define DEFAULT_VK 0xFFFFFFFF

	switch (wParam) {
	case VK_UP:
	{

		WCHAR File[MAX_CHAR_STRING] = { 0 };

		OPENFILENAME ofn = { 0 };
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hMainWindow;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_READONLY | OFN_NOLONGNAMES | OFN_HIDEREADONLY;
		ofn.hInstance = NULL;
		ofn.lpstrFilter = L"Music Files\0*.mp3;*.wma;*.wav\0MP3 Files\0*.mp3\0WMA Files\0*.wma\0WAV Files\0*.wav\0\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrTitle = L"Open Music";
		ofn.lpstrInitialDir = L"%USERPROFILE%";
		ofn.lpstrFile = File;
		ofn.nMaxFile = MAX_CHAR_STRING;

		GetOpenFileName(&ofn);

		Sound::Open(L"Music", File);

		break;

	}
	case VK_RIGHT:
	{
		if(Sound::GetPlaybackStatus(L"Music", MCI_STATUS_MODE) != MCI_MODE_PLAY)
			Sound::Play(hMainWindow, L"Music");

		break;

	}
	case VK_LEFT:
	{

		if (Sound::GetPlaybackStatus(L"Music", MCI_STATUS_MODE) == MCI_MODE_PLAY)
			Sound::Pause(L"Music");
		else if (Sound::GetPlaybackStatus(L"Music", MCI_STATUS_MODE) == MCI_MODE_PAUSE)
			Sound::Resume(L"Music");

		break;

	}
	case VK_DOWN:
	{

		if (Sound::GetPlaybackStatus(L"Music", MCI_STATUS_MODE) == MCI_MODE_PLAY)
			Sound::Stop(L"Music");
		else if (Sound::GetPlaybackStatus(L"Music", MCI_STATUS_MODE) == MCI_MODE_STOP)
			Sound::Close(L"Music");

		break;

	}
	}

}
#pragma endregion

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
	case MM_MCINOTIFY:
	{
		onMCINotify(hMainWindow, lParam);
		RETURN 0;
	}
	case WM_COMMAND:
	{
		onCommand(hMainWindow, wParam, lParam);
		RETURN 0;
	}
	case WM_KEYDOWN:
	{
		onKeyDown(hMainWindow, wParam, lParam);
		RETURN 0;
	}
	case WM_CLOSE:
	{
		IF(MessageBox(hMainWindow, L"Are You Serious!", L"INFORMATION", MB_YESNO | MB_ICONINFORMATION | MB_DEFBUTTON2) == IDYES) {
			mciSendCommand(MCI_ALL_DEVICE_ID, MCI_CLOSE, NULL, NULL);
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
