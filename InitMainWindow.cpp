#include "InitMainWindow.h"

#pragma region InitMainWindowStaticMembers
PAINTSTRUCT MainWindow::ps = { 0 };
HDC MainWindow::MainWindowDC = { 0 };

HDC MainWindow::MemoryDC = { 0 };
HBITMAP MainWindow::MainBitmap = { 0 };

HFONT MainWindow::MainFont = { 0 };
HBRUSH MainWindow::MainWindowBackgroundBrush = CreateSolidBrush(MainWindowBackgroundColor);

HWND MainWindow::hMainWindow = { 0 };
RECT MainWindow::MainWindowDimensions = { 0, 0, MainWindowWidth, MainWindowHeight };

#ifdef APP_DEBUG
HWND MainWindow::hDebugTool1 = { 0 };
HWND MainWindow::hDebugTool2 = { 0 };
#endif // APP_DEBUG

POINT MainWindow::mousePosition = { 0 };
#pragma endregion

#pragma region InitMainWindow
BOOL MainWindow::InitMainWindowClass(LPCWSTR ClassName) {

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
		ScreenWidth / 2 - MainWindowDimensions.right / 2,
		ScreenHeight / 2 - MainWindowDimensions.bottom / 2,
		MainWindowDimensions.right,
		MainWindowDimensions.bottom,
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
BOOL operator==(POINT& Left, POINT& Right) {

	if (Left.x == Right.x and Left.y == Right.y) {
		return TRUE;
	}
	else {
		return FALSE;
	}

}

BOOL operator==(POINT& Position, RECT& Rectangle) {

	if (Position.x >= Rectangle.left and Position.x <= Rectangle.right and
		Position.y >= Rectangle.top and Position.y <= Rectangle.bottom) {
		return TRUE;
	}
	else {
		return FALSE;
	}

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

#ifdef APP_DEBUG
VOID MainWindow::CreateDebugTools(HWND ParentWindow) {

	std::vector <HWND> DebugTools = { hDebugTool1, hDebugTool2 };
	std::vector <std::string> Captions = { "X = 0 Y = 0", "Width = 0 Height = 0" };
	std::vector <SHORT> DebugToolsID = { ID_DEBUG_TOOL_1, ID_DEBUG_TOOL_2 };

	for (SIZE_T I = 0; I < DebugTools.size(); I++) {

		if (!(DebugTools[I] = CreateWindowExA(WS_EX_CLIENTEDGE,
			"STATIC",
			Captions[I].c_str(),
			WS_CHILD | WS_BORDER | WS_VISIBLE | SS_OWNERDRAW,
			0, 0, 0, 0,
			ParentWindow,
			(HMENU)DebugToolsID[I],
			HInstance(),
			nullptr))) {
			Functions::ShowLastError(hMainWindow, " - Child Window not Created!");
			PostQuitMessage(0);
		}

		SetFont(DebugTools[I], MainFont);

	}

	hDebugTool1 = DebugTools[0], hDebugTool2 = DebugTools[1];

}
#endif // APP_DEBUG
#pragma endregion

#pragma region Events
VOID MainWindow::onCreate(HWND hMainWindow, LPARAM lParam) {

	#pragma region InitCustomControls
	Animation::InitAnimation();
	DropFiles::InitDropFiles();
	ColorPicker::InitColorPicker();
	Calculator::InitCalculator();
	#pragma endregion

	#ifdef APP_DEBUG
	CreateDebugTools(hMainWindow);
	#endif // APP_DEBUG

	#pragma region Examples
	Animation::AnimationStyle as = { 0 };
	as.SymbolColor = Colors::BlueColor;
	as.Proportion = 4;
	as.Symbol = '+';

	DropFiles::DropFilesStyle dfs = { 0 };
	dfs.BackgroundColor = Colors::OrangeColor;
	dfs.ForegroundColor = Colors::BlueColor;

	CreateWindowEx(WS_EX_STATICEDGE, L"ANIMATION", L"ANIMATION", WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 10, 140, 140, hMainWindow, (HMENU)ControlsIDs::ID_ANIMATION_STARS, HInstance(), &as);
	CreateWindowEx(WS_EX_STATICEDGE, L"DROP FILES", L"Drop File/s Here", WS_CHILD | WS_BORDER | WS_VISIBLE, 270, 120, 240, 140, hMainWindow, (HMENU)ControlsIDs::ID_DROP_FILES, HInstance(), &dfs);
	CreateWindowEx(WS_EX_STATICEDGE, L"COLOR PICKER", L"LARGE", WS_CHILD | WS_BORDER | WS_VISIBLE, 160, 10, CP_SHOW, CP_SHOW, hMainWindow, (HMENU)ControlsIDs::ID_COLOR_PICKER, HInstance(), nullptr);
	CreateWindowEx(WS_EX_STATICEDGE, L"CALCULATOR", L"", WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 160, CL_SHOW, CL_SHOW, hMainWindow, (HMENU)ControlsIDs::ID_CALCULATOR, HInstance(), nullptr);
	#pragma endregion

}

VOID MainWindow::onSize(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	MainWindowDimensions.right = LOWORD(lParam);
	MainWindowDimensions.bottom = HIWORD(lParam);

	#ifdef APP_DEBUG
	SetWindowPos(hDebugTool1, NULL, MainWindowDimensions.right - 160, 0, 160, 25, SWP_SHOWWINDOW);
	SetWindowPos(hDebugTool2, NULL, MainWindowDimensions.right - 240, 30, 240, 25, SWP_SHOWWINDOW);

	#pragma region DebugTool2
	std::string SMainWindowDimensions = "Width = " + std::to_string(MainWindowDimensions.right) + " Height = " + std::to_string(MainWindowDimensions.bottom);
	SetWindowTextA(hDebugTool2, SMainWindowDimensions.c_str());
	#endif // APP_DEBUG
	#pragma endregion

	RedrawWindow(hMainWindow, NULL, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE);

}

VOID MainWindow::onMouseMove(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	GetCursorPos(&mousePosition);
	ScreenToClient(hMainWindow, &mousePosition);

	#pragma region DebugTool1
	#ifdef APP_DEBUG
	std::string SMousePosition = "X = " + std::to_string(mousePosition.x) + " Y = " + std::to_string(mousePosition.y);
	SetWindowTextA(hDebugTool1, SMousePosition.c_str());
	#endif // APP_DEBUG
	#pragma endregion

}

VOID MainWindow::onDrawItem(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	LPDRAWITEMSTRUCT item = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);

	#pragma region DebugTools
	#ifdef APP_DEBUG
	if (item->CtlID == ID_DEBUG_TOOL_1 or item->CtlID == ID_DEBUG_TOOL_2) {

		SIZE size = { 0 };
		WCHAR StaticText[MAX_CHAR_STRING] = { 0 };
		SetDCBrushColor(item->hDC, Colors::DarkGreenColor);
		FillRect(item->hDC, &item->rcItem, static_cast<HBRUSH>(GetStockObject(DC_BRUSH)));
		SetBkMode(item->hDC, TRANSPARENT);
		SetTextColor(item->hDC, Colors::WhiteColor);
		INT TextLength = GetWindowText(item->hwndItem, StaticText, ARRAYSIZE(StaticText));
		GetTextExtentPoint(item->hDC, StaticText, TextLength, &size);
		TextOut(item->hDC, item->rcItem.right / 2 - size.cx / 2, item->rcItem.bottom / 2 - size.cy / 2, StaticText, TextLength);

	}
	#endif
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

	D2D1_RENDER_TARGET_PROPERTIES RenderTargetProperties = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		0.0F, 0.0F, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT);

	ID2D1Factory *Factory = nullptr;
	HRESULT hrFactory = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory);
	
	if (SUCCEEDED(hrFactory)) {

		ID2D1DCRenderTarget *RenderTarget = nullptr;
		HRESULT hrRenderTarget = Factory->CreateDCRenderTarget(&RenderTargetProperties, &RenderTarget);
		
		if (SUCCEEDED(hrRenderTarget)) {

			RenderTarget->BindDC(MemoryDC, &MainWindowDimensions);
			D2D1_SIZE_U RenderTargetSize = RenderTarget->GetPixelSize();
			RenderTarget->BeginDraw();

			std::vector<ID2D1Geometry*> Geometry{};

			for (FLOAT R = 10.0F; R <= 600.0F; R += 10.0F) {
				ID2D1RectangleGeometry *RectanglePtr = nullptr;
				HRESULT hrRectangleGeometry = Factory->CreateRectangleGeometry(D2D1::RectF(RenderTargetSize.width / 2.0F - R, RenderTargetSize.height / 2.0F - R,
					RenderTargetSize.width / 2.0F + R, RenderTargetSize.height / 2.0F + R), &RectanglePtr);
				if (FAILED(hrRectangleGeometry)) break;
				Geometry.push_back(RectanglePtr);
			}
			
			ID2D1GeometryGroup *GeometryGroup = nullptr;
			HRESULT hrGeometryGroup = Factory->CreateGeometryGroup(D2D1_FILL_MODE_ALTERNATE, Geometry.data(), static_cast<UINT32>(Geometry.size()), &GeometryGroup);

			if (SUCCEEDED(hrGeometryGroup)) {

				ID2D1SolidColorBrush *GoldBrush = nullptr;
				HRESULT hrAntiqueWhiteBrush = RenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Enum::Gold), &GoldBrush);

				if (SUCCEEDED(hrAntiqueWhiteBrush)) {
					RenderTarget->FillGeometry(GeometryGroup, GoldBrush);
					GoldBrush->Release();
				}

				GeometryGroup->Release();

			}

			for (SIZE_T I = 0; I < Geometry.size(); I++) {
				static_cast<ID2D1RectangleGeometry*>(Geometry[I])->Release();
			}

			RenderTarget->EndDraw();
			RenderTarget->Release();

		}

		Factory->Release();

	}

	Draw::drawSmallGradient(MemoryDC, MainWindowDimensions.right / 2 - 420 / 2, MainWindowDimensions.bottom / 2 - 40 / 2);

	SIZE size = { 0 };
	CHAR Text[] = "Hello World!";
	GetTextExtentPointA(MainWindowDC, Text, ARRAYSIZE(Text), &size);
	TextOutA(MemoryDC, MainWindowDimensions.right / 2 - size.cx / 2, MainWindowDimensions.bottom / 2 - size.cy / 2, Text, ARRAYSIZE(Text) - 1);

	BitBlt(MainWindowDC, 0, 0, MainWindowDimensions.right, MainWindowDimensions.bottom, MemoryDC, 0, 0, SRCCOPY);

	DeleteDC(MemoryDC);
	DeleteObject(MainBitmap);

	EndPaint(hMainWindow, &ps);

}

VOID MainWindow::onCommand(HWND hMainWindow, WPARAM wParam, LPARAM lParam) {

	#define DEFAULT_ID 0xFFFF

	switch (LOWORD(wParam)) {
	case ID_COLOR_PICKER:
	{

		std::string Color = "R: " + std::to_string(GetRValue((COLORREF)lParam)) +
			"\tG: " + std::to_string(GetGValue((COLORREF)lParam)) +
			"\tB: " + std::to_string(GetBValue((COLORREF)lParam));
		PRINT(0x0B, Color.c_str());

		break;

	}
	case ID_DROP_FILES:
	{

		CHAR Buffer[MAX_CHAR_STRING] = { 0 };
		UINT FileCount = DragQueryFileA((HDROP)lParam, 0xFFFFFFFF, Buffer, ARRAYSIZE(Buffer));
		for (UINT File = 0; File < FileCount; File++) {
			DragQueryFileA((HDROP)lParam, File, Buffer, ARRAYSIZE(Buffer));
			PRINT(0x09, Buffer);
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

		LPMINMAXINFO minmax = (LPMINMAXINFO)lParam;
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
		if (MessageBox(hMainWindow, L"Are You Serious!", L"INFORMATION", MB_YESNO | MB_ICONINFORMATION | MB_DEFBUTTON2) == IDYES) {
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
