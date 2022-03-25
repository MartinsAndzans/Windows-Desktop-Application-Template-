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

	ShowWindow(GetConsoleWindow(), SW_SHOW);
	
	if (MainWindow::InitMainWindowClass(MAIN_WINDOW_CLASS)) {
		if (MainWindow::CreateMainWindow(MAIN_WINDOW_CLASS, MAIN_WINDOW_TITLE)) {
			BroadcastMessages();
		}
	}

	return 0;

}

VOID BroadcastMessages() {

	while (1) {

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
