#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "InitMainWindow.h"

#pragma region InitGlobalStaticMembers
MSG MainWindow::Msg = { 0 };
BOOL MainWindow::ExtraProcessing = FALSE;

WCHAR MainWindow::ApplicationDirectory[MAX_CHAR_STRING] = { 0 };
#pragma endregion

VOID BroadcastMessages();

int wmain(int ArrgumentCount, wchar_t *Arrguments[]) {

	#ifdef APP_DEBUG
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	Console::setConsoleTitle("Debug Console");
	#else defined(APP_RELEASE)
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	Console::setConsoleTitle("Don't Close");
	#endif

	#ifdef APP_DEBUG
	SetConsoleColor("1F");
	PRINT("Debug Console Is Initilaized Correctly ...");
	#endif

	Console::setCursorPosition(0, 0, FALSE, FALSE, FALSE);
	
	if (MainWindow::InitMainWindowClass(MAIN_WINDOW_CLASS)) {
		if (MainWindow::CreateMainWindow(MAIN_WINDOW_CLASS, MAIN_WINDOW_TITLE)) {
			BroadcastMessages();
		}
	}

	#ifdef APP_DEBUG
	PRINT("App Closed Successfully ...");
	#endif

	return 0;

}

VOID BroadcastMessages() {

	while (true) {

		if (MainWindow::ExtraProcessing) {
			PeekMessage(&MainWindow::Msg, NULL, 0, 0, PM_REMOVE);
		}
		else {
			GetMessage(&MainWindow::Msg, NULL, 0, 0);
		}

		if (MainWindow::Msg.message == WM_QUIT) {
			break;
		}

		TranslateMessage(&MainWindow::Msg);
		DispatchMessage(&MainWindow::Msg);

		if (MainWindow::ExtraProcessing) {

			//TODO: Extra Processing

			Sleep(10);

		}

	}

}
