#pragma once

#ifndef _MAIN_WINDOW_
#define _MAIN_WINDOW_

#include "Headers.h"

#pragma region LocalMacros

#define ID_DEBUG_TOOL_1 -1
#define ID_DEBUG_TOOL_2 -2

#pragma endregion

class MainWindow {

private:

	#pragma region MainWindowStaticMembers

	static PAINTSTRUCT MainPS;
	static HDC MainWindowDC;

	static HDC MemoryDC;
	static HBITMAP MainBitmap;

	static HBRUSH MainWindowBackgroundBrush;

	static HFONT MainFont;

	static HWND hMainWindow;
	static RECT MainWindowDimensions;

	static HWND hDebugTool1;
	static HWND hDebugTool2;

	static POINT mousePosition;

	#pragma endregion

private:

	#pragma region Functions

	static VOID CreateFonts();

	static VOID CreateDebugTools();

	#pragma endregion

	#pragma region Events

	static VOID onCreate(HWND hMainWindow, LPARAM lParam);
	static VOID onSize(HWND hMainWindow, WPARAM wParam, LPARAM lParam);
	static VOID onMouseMove(HWND hMainWindow, WPARAM wParam, LPARAM lParam);
	static VOID onDrawItem(HWND hMainWindow, WPARAM wParam, LPARAM lParam);
	static VOID onPaint(HWND hMainWindow);
	static VOID onCommand(HWND hMainWindow, WPARAM wParam, LPARAM lParam);
	static VOID onDropFiles(HWND hMainWindow, WPARAM wParam);

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

	static BOOL InitMainWindowClass(std::wstring ClassName);
	static BOOL CreateMainWindow(std::wstring ClassName, std::wstring WindowTitle);

	#pragma endregion

};

#endif
