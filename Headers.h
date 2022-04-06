#pragma once

#ifndef _HEADERS_
#define _HEADERS_

#pragma region Headers

#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>

#include "Functions.h"
#include "Draw.h"
#include "Console.h"

#include "ColorPicker.h"
#include "AnimationStars.h"
#include "Calculator.h"
#include "DropFiles.h"

#pragma endregion

#pragma region Macros

#ifndef UNICODE
#define UNICODE
#endif

#pragma region Important

#define IF if
#define ELSE else

#define RETURN return

#define MAX_CHAR_STRING 256

#define HInstance() GetModuleHandle(NULL)
#define MainWindowBackgroundColor RGB(252, 200, 43)

#pragma endregion

#pragma region Colors

#define WHITE_COLOR RGB(255, 255, 255)
#define BLACK_COLOR RGB(0, 0, 0)
#define ORANGE_COLOR RGB(214, 152, 45)
#define RED_COLOR RGB(238, 20, 20)
#define BLUE_COLOR RGB(40, 34, 214)
#define GREEN_COLOR RGB(45, 125, 15)

#pragma endregion

#pragma region MainWindow|Class|Title

#define MAIN_WINDOW_CLASS L"MAIN CLASS"
#define MAIN_WINDOW_TITLE L"Window"

#pragma endregion

#pragma region Cursor|Icon

#define MAIN_WINDOW_CURSOR L"\\Crosshair.cur"
#define MAIN_WINDOW_ICON L"\\WindowIcon.ico"

#pragma endregion

#pragma region MainWindowDimensions

#define MainWindowWidth 1400
#define MainWindowHeight 800

#pragma endregion

#pragma region SetMacros

#define SetFont(hwnd, font) SendMessage(hwnd, WM_SETFONT, (WPARAM)font, NULL)
#define SetIcon(hwnd, type, hicon) SendMessage(hwnd, WM_SETICON, (WPARAM)type, (LPARAM)hicon)

#pragma endregion

#pragma region WindowMacros

#define MaximizeWindow(hwnd) SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, NULL)
#define MinimizeWindow(hwnd) SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, NULL)
#define RestoreWindow(hwnd) SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, NULL)

#pragma endregion

#pragma region ListBoxMacros

#define ListBox_AddString(hwnd, string) SendMessage(hwnd, LB_ADDSTRING, NULL, (LPARAM)string)
#define ListBox_SetItemData(hwnd, index, value) SendMessage(hwnd, LB_SETITEMDATA, (WPARAM)index, (LPARAM)value)
#define ListBox_GetItemData(hwnd, index) SendMessage(hwnd, LB_GETITEMDATA, (WPARAM)index, NULL)
#define ListBox_SetSelectedItemIndex(hwnd, index) SendMessage(hwnd, LB_SETCURSEL, (WPARAM)index, NULL)
#define ListBox_GetSelectedItemIndex(hwnd) SendMessage(hwnd, LB_GETCURSEL, NULL, NULL)
#define ListBox_FindString(hwnd, string) SendMessage(hwnd, LB_FINDSTRING, (WPARAM)-1, (LPARAM)string)
#define ListBox_SetTopListBoxItem(hwnd, index) SendMessage(hwnd, LB_STETOPINDEX, (WPARAM)index, NULL)

#pragma endregion

#pragma endregion

#endif
