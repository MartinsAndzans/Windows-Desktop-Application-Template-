#pragma once

#ifndef _HEADERS_
#define _HEADERS_

#pragma region Headers
#include <Windows.h>
#include <future>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <conio.h>

#include "resource.h"

#include "Functions.h"
#include "Draw.h"
#include "Console.h"

#include "ColorPicker.h"
#include "Animation.h"
#include "Calculator.h"
#include "DropFiles.h"
#pragma endregion

#pragma region Macros
#pragma region Basic
#define IF if
#define ELSE else
#define RETURN return
#define MAX_CHAR_STRING 256
#define HInstance() GetModuleHandle(NULL)
#define MainWindowBackgroundColor RGB(15, 160, 255)
#pragma endregion

#pragma region Colors
#define WHITE_COLOR RGB(255, 255, 255)
#define BLACK_COLOR RGB(0, 0, 0)
#define ORANGE_COLOR RGB(240, 190, 0)
#define RED_COLOR RGB(255, 0, 0)
#define BLUE_COLOR RGB(0, 0, 255)
#define GREEN_COLOR RGB(0, 255, 0)
#define DARK_GREEN_COLOR RGB(0, 145, 0)

#define LighterColor(SourceColor, LighterTo) RGB(GetRValue(SourceColor) + LighterTo, GetGValue(SourceColor) + LighterTo, GetBValue(SourceColor) + LighterTo) // Returns LighterColor
#define DarkerColor(SourceColor, DarkerTo) RGB(GetRValue(SourceColor) - DarkerTo, GetGValue(SourceColor) - DarkerTo, GetBValue(SourceColor) - DarkerTo) // Returns DarkerColor
#pragma endregion

#pragma region MainWindow|Class|Title
#define MAIN_WINDOW_CLASS L"MAIN WINDOW CLASS"
#define MAIN_WINDOW_TITLE L"Window [TEMPLATE]"
#pragma endregion

#pragma region MainWindowDimensions
#define MainWindowWidth 1400
#define MainWindowHeight 800
#pragma endregion

#pragma region DebugLog
#ifdef APP_DEBUG
#define PRINT(color, text) Console::setConsoleTextColor(color),\
std::cout << text << std::endl
#define PRINTW(color, text) Console::setConsoleTextColor(color),\
std::wcout << text << std::endl
#else
#define PRINT(color, text)
#define PRINTW(color, text)
#endif
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

#pragma region CheckBoxMacros
#define CheckBox_GetCheckboxState(hwnd) SendMessage(hwnd, BM_GETCHECK, NULL, NULL) // Return Value is Checkbox State
#define CheckBox_Check(hwnd) SendMessage(hwnd, BM_SETCHECK, (WPARAM)BST_CHECKED, NULL)
#define CheckBox_UnCheck(hwnd) SendMessage(hwnd, BM_SETCHECK, (WPARAM)BST_UNCHECKED, NULL)
#pragma endregion

#pragma region ListBoxMacros
#define ListBox_AddString(hwnd, string) SendMessage(hwnd, LB_ADDSTRING, NULL, (LPARAM)string)
#define ListBox_DeleteString(hwnd, index) SendMessage(hwnd, LB_DELETESTRING, (WPARAM)index, NULL)
#define ListBox_SetItemData(hwnd, index, value) SendMessage(hwnd, LB_SETITEMDATA, (WPARAM)index, (LPARAM)value)
#define ListBox_GetItemData(hwnd, index) SendMessage(hwnd, LB_GETITEMDATA, (WPARAM)index, NULL) // Return Value is Value Associated With that Item
#define ListBox_SetSelectedItemIndex(hwnd, index) SendMessage(hwnd, LB_SETCURSEL, (WPARAM)index, NULL)
#define ListBox_GetSelectedItemIndex(hwnd) SendMessage(hwnd, LB_GETCURSEL, NULL, NULL)  // Return Value is Selected Item Index
#define ListBox_FindString(hwnd, string) SendMessage(hwnd, LB_FINDSTRING, (WPARAM)-1, (LPARAM)string)  // Return Value is String Index
#define ListBox_SetTopListBoxItem(hwnd, index) SendMessage(hwnd, LB_SETTOPINDEX, (WPARAM)index, NULL)
#pragma endregion
#pragma endregion

#endif
