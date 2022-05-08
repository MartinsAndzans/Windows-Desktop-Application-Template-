#ifndef _HEADERS_
#define _HEADERS_

#pragma region Headers
#include <ciso646>
#include <Windows.h> // Win32Api
#include <d2d1.h> // DirectX2D
#include <string> // std::string
#include <fstream> // File Read/Write
#include <vector> // std::vector
#include <math.h>

#include "resource.h" // Resources

#include "Functions.h" // Useful Functions
#include "Draw.h" // GDI Draw
#include "Console.h" // Console API

#pragma region CostomCotrols
#include "ColorPicker.h"
#include "Animation.h"
#include "Calculator.h"
#include "DropFiles.h"
#pragma endregion

#pragma endregion

#pragma region Macros

#pragma region Basic
#define MAX_CHAR_STRING 256
#define HInstance() GetModuleHandle(NULL)
#define MainWindowBackgroundColor RGB(15, 160, 255)
#pragma endregion

#pragma region ColorArithmetic
#define LighterColor(SourcwColor, LighterTo)  // Returns LighterColor
#define DarkerColor(SourceColor, DarkerTo) RGB(GetRValue(SourceColor) - DarkerTo, GetGValue(SourceColor) - DarkerTo, GetBValue(SourceColor) - DarkerTo) // Returns DarkerColor
#pragma endregion

#pragma region MainWindow|Class|Title
#define MainWindowClass L"MAIN WINDOW CLASS"
#define MainWindowTitle L"Window -TEMPLATE-"
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

#pragma region CheckBoxMacros
#define CheckBox_GetCheck(hwnd) SendMessage(hwnd, BM_GETCHECK, NULL, NULL) // Return Value is Checkbox State
#define CheckBox_Check(hwnd) SendMessage(hwnd, BM_SETCHECK, static_cast<WPARAM>(BST_CHECKED), NULL)
#define CheckBox_UnCheck(hwnd) SendMessage(hwnd, BM_SETCHECK, static_cast<WPARAM>(BST_UNCHECKED), NULL)
#pragma endregion

#pragma region ListBoxMacros
#define ListBox_AddString(hwnd, string) SendMessage(hwnd, LB_ADDSTRING, NULL, static_cast<LPARAM>(string))
#define ListBox_DeleteString(hwnd, index) SendMessage(hwnd, LB_DELETESTRING, static_cast<WPARAM>(index), NULL)
#define ListBox_SetItemData(hwnd, index, value) SendMessage(hwnd, LB_SETITEMDATA, static_cast<WPARAM>(index), static_cast<LPARAM>(value))
#define ListBox_GetItemData(hwnd, index) SendMessage(hwnd, LB_GETITEMDATA, static_cast<WPARAM>(index), NULL) // Return Value is Value Associated With that Item
#define ListBox_SelectItem(hwnd, index) SendMessage(hwnd, LB_SETCURSEL, static_cast<WPARAM>(index), NULL)
#define ListBox_GetSelectedItemIndex(hwnd) SendMessage(hwnd, LB_GETCURSEL, NULL, NULL) // Return Value is Selected Item Index
#define ListBox_FindString(hwnd, string) SendMessage(hwnd, LB_FINDSTRING, static_cast<WPARAM>(-1), static_cast<LPARAM>(string)) // Return Value is String Index
#define ListBox_SetTopItem(hwnd, index) SendMessage(hwnd, LB_SETTOPINDEX, static_cast<WPARAM>(index), NULL)
#pragma endregion

#pragma endregion

#endif // _HEADERS_
