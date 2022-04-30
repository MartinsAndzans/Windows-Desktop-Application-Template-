#ifndef _MAIN_WINDOW_
#define _MAIN_WINDOW_

#include "Headers.h"

#pragma region LocalMacros
#define ID_DEBUG_TOOL_1 -10
#define ID_DEBUG_TOOL_2 -20

#define ID_ANIMATION_STARS 01
#define ID_DROP_FILES 02
#define ID_COLOR_PICKER 03
#define ID_CALCULATOR 04
#pragma endregion

class MainWindow {

private:

	/// <summary>
	/// * Predefined Colors *
	/// </summary>
	enum Colors {
		WhiteColor = RGB(255, 255, 255),
		BlackColor = RGB(0, 0, 0),
		OrangeColor = RGB(240, 190, 0),
		RedColor = RGB(255, 0, 0),
		BlueColor = RGB(0, 0, 255),
		GreenColor = RGB(0, 255, 0),
		DarkGreenColor = RGB(0, 145, 0)
	};

	#pragma region MainWindowStaticMembers
	static PAINTSTRUCT ps;
	static HDC MainWindowDC;

	static HDC MemoryDC;
	static HBITMAP MainBitmap;

	static HFONT MainFont;
	static HBRUSH MainWindowBackgroundBrush;

	static HWND hMainWindow;
	static RECT MainWindowDimensions;

	#ifdef APP_DEBUG
	static HWND hDebugTool1;
	static HWND hDebugTool2;
	#endif // APP_DEBUG

	static POINT mousePosition;
	#pragma endregion

private:

	#pragma region Functions
	static VOID CreateFonts();
	#ifdef APP_DEBUG
	static VOID CreateDebugTools();
	#endif // APP_DEBUG
	#pragma endregion

	#pragma region Events
	static VOID onCreate(HWND hMainWindow, LPARAM lParam);
	static VOID onSize(HWND hMainWindow, WPARAM wParam, LPARAM lParam);
	static VOID onMouseMove(HWND hMainWindow, WPARAM wParam, LPARAM lParam);
	static VOID onDrawItem(HWND hMainWindow, WPARAM wParam, LPARAM lParam);
	static VOID onPaint(HWND hMainWindow);
	static VOID onCommand(HWND hMainWindow, WPARAM wParam, LPARAM lParam);
	static VOID onKeyDown(HWND hMainWindow, WPARAM wParam, LPARAM lParam);
	static VOID onMCINotify(HWND hMainWindow, LPARAM lParam);
	#pragma endregion

	#pragma region MainWindowProcedure
	static LRESULT CALLBACK MainWindowProcedure(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam);
	#pragma endregion

public:

	#pragma region GlobalStaticMembers
	static MSG Msg;
	static BOOL ExtraProcessing;

	static WCHAR ApplicationDirectory[MAX_CHAR_STRING];
	#pragma endregion

public:

	#pragma region InitMainWindow
	static BOOL InitMainWindowClass(LPCSTR ClassName);
	static BOOL CreateMainWindow(LPCSTR ClassName, LPCSTR WindowTitle);
	#pragma endregion

};

#endif // _MAIN_WINDOW_
