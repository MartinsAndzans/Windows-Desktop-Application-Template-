#ifndef _MAIN_WINDOW_
#define _MAIN_WINDOW_

#include "Headers.h"

class MainWindow {

private:

	/// <summary>
	/// * All Controls IDs *
	/// </summary>
	enum ControlsIDs {
		ID_DEBUG_TOOL_1 = -1,
		ID_DEBUG_TOOL_2 = -2,
		ID_ANIMATION_STARS = 1,
		ID_DROP_FILES = 2,
		ID_COLOR_PICKER = 3,
		ID_CALCULATOR = 4
	};

	/// <summary>
	/// * Converted Time From Milliseconds *
	/// </summary>
	enum Time {
		Second = 1000,
		Minute = 1000 * 60,
		Hour = 1000 * 120
	};

	#pragma region MainWindowStaticMembers
	static PAINTSTRUCT ps;
	static HDC MainWindowDC;

	static HDC MemoryDC;
	static HBITMAP MainBitmap;

	static HFONT MainFont;
	static HBRUSH MainWindowBackgroundBrush;
	static POINT mousePosition;

	static HWND hMainWindow;
	static RECT MainWindowDimensions;

	static HWND hDebugTool1;
	static HWND hDebugTool2;

	static HWND Test;
	#pragma endregion

private:

	#pragma region Functions
	static VOID CreateMainWindowFont();
	static VOID CreateDebugTools(HWND ParentWindow);
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
	static BOOL RegisterMainWindowClass(LPCWSTR ClassName);
	static BOOL CreateMainWindow(LPCWSTR ClassName, LPCWSTR WindowTitle);
	#pragma endregion

};

#endif // _MAIN_WINDOW_
