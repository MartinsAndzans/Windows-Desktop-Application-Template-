#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "InitMainWindow.h"

#pragma region InitGlobalStaticMembers
MSG MainWindow::Msg = { 0 };
BOOL MainWindow::ExtraProcessing = FALSE;

WCHAR MainWindow::ApplicationDirectory[MAX_CHAR_STRING] = { 0 };
#pragma endregion

void BroadcastMessages();

int wmain(INT ArrgumentCount, LPWSTR Arrguments[]) {

	#ifdef APP_DEBUG
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	Console::setConsoleTitle("Debug Console");
	#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	Console::setConsoleTitle("Console");
	#endif // APP_DEBUG

	Console::Print("Debug Console Initilaized Correctly . . .", 0x0A);

	GetCurrentDirectory(ARRAYSIZE(MainWindow::ApplicationDirectory), MainWindow::ApplicationDirectory);

	if (MainWindow::InitMainWindowClass(MainWindowClass)) {
		if (MainWindow::CreateMainWindow(MainWindowClass, MainWindowTitle)) {
			BroadcastMessages();
		}
	}

	Console::Print("App Closed Successfully . . .", 0x0A);
	Console::Pause("Press any Key to continue . . .", 0x0A);

	return 0;

}

void BroadcastMessages() {

	while (true) {

		if (MainWindow::ExtraProcessing) {
			PeekMessage(&MainWindow::Msg, NULL, 0, 0, PM_REMOVE);
		} else {
			GetMessage(&MainWindow::Msg, NULL, 0, 0);
		}

		if (MainWindow::Msg.message == WM_QUIT)
			break;

		TranslateMessage(&MainWindow::Msg);
		DispatchMessage(&MainWindow::Msg);

		if (MainWindow::ExtraProcessing) {

			//TODO: Extra Processing

			Sleep(10);

		}

	}

}
