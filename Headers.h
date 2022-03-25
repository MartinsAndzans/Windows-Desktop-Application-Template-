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

#include "ColorPicker.h"
#include "AnimationStars.h"

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

#pragma endregion

#endif
