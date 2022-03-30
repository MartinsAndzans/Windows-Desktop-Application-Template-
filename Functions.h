#pragma once

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#ifndef _FUNCTIONS_
#define _FUNCTIONS_

#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

class Functions {

public:

	static INT FindChar(LPWSTR Text, const wchar_t Char, INT TextLength) {

		for (int i = 0; i < TextLength; i++) {

			if (Text[i] == Char) {
				return i;
			}

		}

		return -1;

	}

	static DOUBLE _atod(LPWSTR Text) {

		CONST POINTS ASCII_NUMBERS = { 48, 57 }; // 0 - 9
		CONST SHORT ASCII_MINUS = 45; // -

		UINT LEFT = 1;
		DOUBLE RIGHT = 0.1;

		BOOL MINUS = FALSE;

		DOUBLE Number = 0.00;

		INT DotPosition = FindChar(Text, L'.', lstrlenW(Text));

		if (DotPosition != -1 || 0) {

			// 123456789.123456789

			for (int i = DotPosition - 1; i >= 0; i--) {
				INT ASCII = (int)Text[i];
				if (ASCII >= ASCII_NUMBERS.x && ASCII <= ASCII_NUMBERS.y) {
					Number = Number + (ASCII - ASCII_NUMBERS.x) * LEFT;
					LEFT = LEFT * 10;
				}
				else {
					(ASCII == ASCII_MINUS) ? MINUS = TRUE : MINUS = FALSE;
					break;
				}
			}
			for (int i = DotPosition + 1; i < lstrlenW(Text); i++) {
				INT ASCII = (int)Text[i];
				if (ASCII >= ASCII_NUMBERS.x && ASCII <= ASCII_NUMBERS.y) {
					Number = Number + (ASCII - ASCII_NUMBERS.x) * RIGHT;
					RIGHT = RIGHT / 10;
				}
				else {
					break;
				}
			}

			if (MINUS == TRUE) {
				Number = Number * -1;
			}

			return Number;

		}
		if (DotPosition != -1) {

			//.123456789

			for (int i = DotPosition + 1; i < lstrlenW(Text); i++) {
				INT ASCII = (int)Text[i];
				if (ASCII >= ASCII_NUMBERS.x && ASCII <= ASCII_NUMBERS.y) {
					Number = Number + (ASCII - ASCII_NUMBERS.x) * RIGHT;
					RIGHT = RIGHT / 10;
				}
				else {
					break;
				}
			}

			if (MINUS == TRUE) {
				Number = Number * -1;
			}

			return Number;

		}
		if (DotPosition == -1) {

			// 123456789

			INT NumberLength = -1;

			for (int i = 0; i < lstrlenW(Text); i++) {
				INT ASCII = (int)Text[i];
				if (ASCII >= ASCII_NUMBERS.x && ASCII <= ASCII_NUMBERS.y || ASCII == ASCII_MINUS) {
					NumberLength++;
				}
				else {
					break;
				}
			}
			for (int i = NumberLength; i >= 0; i--) {
				INT ASCII = (int)Text[i];
				if (ASCII >= ASCII_NUMBERS.x && ASCII <= ASCII_NUMBERS.y) {
					Number = Number + (ASCII - ASCII_NUMBERS.x) * LEFT;
					LEFT = LEFT * 10;
				}
				else {
					(ASCII == ASCII_MINUS) ? MINUS = TRUE : MINUS = FALSE;
					break;
				}
			}

			if (MINUS == TRUE) {
				Number = Number * -1;
			}

			return Number;

		}

		return Number;

	}

	static std::string _itos(int number, int base) {

		/// <summary>
		/// Converts Integer To String
		/// </summary>
		/// <param name="number">Integer Type Number To Be Converted</param>
		/// <returns>Converted Number</returns>

		CHAR String[256] = { 0 };
		_itoa_s(number, String, base);
		std::string RString = String;
		return RString;

	}

	static std::string _dtos(double number, int NumbersR) {

		/// <summary>
		/// Converts Double To String
		/// </summary>
		/// <param name="number">Double Type Number To Be Converted</param>
		/// <returns>Converted Number</returns>

		int FNumber = 1;

		for (int i = NumbersR; i > 1; i--) {
			FNumber = FNumber * 10;
		}

		CHAR StringL[256] = { 0 }, StringR[256] = { 0 };
		_itoa_s((int)number, StringL, 10);
		double RNumberD = number - (int)number;
		int RNumberI = (int)(RNumberD * FNumber);
		_itoa_s(RNumberI, StringR, 10);
		CHAR Double[256] = { 0 };
		strcpy_s(Double, StringL), strcat_s(Double, "."), strcat_s(Double, StringR);
		std::string RDouble = Double;
		return RDouble;

	}

	static bool copyTextToClipboard(HWND hwnd, std::wstring text, SIZE_T textLength) {

		/// <summary>
		/// This Function Puts Text into ClipBoard
		/// </summary>
		/// <param name="hwnd">newClipBoardOwner</param>
		/// <param name="text">Text into ClipBoard</param>
		/// <param name="textLength">Text Length Without null Terminator</param>
		/// <returns>If Function Succeeded Returns True, but If not False</returns>

		if (textLength != 0) {
			if (OpenClipboard(hwnd)) {
				HLOCAL CopyData = { 0 };
				VOID *buffer = NULL;
				EmptyClipboard();
				CopyData = LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT | LMEM_VALID_FLAGS, sizeof(WCHAR) * textLength + 1);
				if (CopyData != NULL) {
					buffer = LocalLock(CopyData);
				}
				if (buffer != NULL) {
					memcpy(buffer, (VOID*)text.c_str(), sizeof(WCHAR) * textLength + 1);
					LocalUnlock(CopyData);
					SetClipboardData(CF_UNICODETEXT, CopyData);
					LocalFree(CopyData);
					CloseClipboard();
					return true;
				}
				CloseClipboard();
				return false;
			}
		}
		return false;

	}

	static bool copyTextToClipboard(HWND hwnd, std::string text, SIZE_T textLength) {

		/// <summary>
		/// This Function Puts Text into ClipBoard
		/// </summary>
		/// <param name="hwnd">newClipBoardOwner</param>
		/// <param name="text">Text into ClipBoard</param>
		/// <param name="textLength">Text Length Without null Terminator</param>
		/// <returns>If Function Succeeded Returns True, but If not False</returns>

		if (textLength != 0) {
			if (OpenClipboard(hwnd)) {
				HLOCAL CopyData = { 0 };
				VOID *buffer = NULL;
				EmptyClipboard();
				CopyData = LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT | LMEM_VALID_FLAGS, sizeof(CHAR) * textLength + 1);
				if (CopyData != NULL) {
					buffer = LocalLock(CopyData);
				}
				if (buffer != NULL) {
					memcpy(buffer, (VOID*)text.c_str(), sizeof(CHAR) * textLength + 1);
					LocalUnlock(CopyData);
					SetClipboardData(CF_TEXT, CopyData);
					LocalFree(CopyData);
					CloseClipboard();
					return true;
				}
				CloseClipboard();
				return false;
			}
		}
		return false;

	}

	static std::wstring copyClipboardWText(HWND hwnd) {

		/// <summary>
		/// This Function Puts WText From ClipBoard To WText Variable
		/// </summary>
		/// <param name="hwnd">newClipBoardOwner</param>
		/// <returns>
		/// If Succeeded Returns Received Text, but If not Error Code
		/// "ERROR -1" - Clipboard is taken by another program!
		/// "ERROR -2" - In Clipboard No Text Data!
		/// </returns>

		if (OpenClipboard(hwnd)) {
			HLOCAL ClipboardData = { 0 };
			VOID *buffer = NULL;
			std::wstring TextFromClipboard = L"";
			ClipboardData = LocalAlloc(LMEM_MOVEABLE | LMEM_ZEROINIT | LMEM_VALID_FLAGS, sizeof(std::wstring));
			if(ClipboardData != NULL) {
				ClipboardData = GetClipboardData(CF_UNICODETEXT);
				buffer = LocalLock(ClipboardData);
			}
			if (buffer != NULL) {
				TextFromClipboard = (wchar_t*)buffer;
				LocalUnlock(ClipboardData);
				CloseClipboard();
				return TextFromClipboard;
			}
			CloseClipboard();
			INT error = 0;
			std::wstring werror = L"ERROR " + std::to_wstring(error = GetLastError()) + L" - In Clipboard No Text Data!\r\n";
			OutputDebugString(werror.c_str());
			return L"ERROR -2";
		}
		CloseClipboard();
		INT error = 0;
		std::wstring werror = L"ERROR" + std::to_wstring(error = GetLastError()) + L" - Clipboard is taken by another program!\r\n";
		OutputDebugString(werror.c_str());
		return L"ERROR -1";
	}

	static std::string copyClipboardText(HWND hwnd) {

		/// <summary>
		/// This Function Puts Text From ClipBoard To Text Variable
		/// </summary>
		/// <param name="hwnd">newClipBoardOwner</param>
		/// <returns>
		/// If Succeeded Returns Received Text, but If not Error Code
		/// "ERROR -1" - Clipboard is taken by another program!
		/// "ERROR -2" - In Clipboard No Text Data!
		/// </returns>

		if (OpenClipboard(hwnd)) {
			HLOCAL ClipboardData = { 0 };
			VOID* buffer = NULL;
			std::string TextFromClipboard = "";
			ClipboardData = LocalAlloc(LMEM_MOVEABLE | LMEM_ZEROINIT | LMEM_VALID_FLAGS, sizeof(std::string));
			if (ClipboardData != NULL) {
				ClipboardData = GetClipboardData(CF_TEXT);
				buffer = LocalLock(ClipboardData);
			}
			if (buffer != NULL) {
				TextFromClipboard = (char*)buffer;
				LocalUnlock(ClipboardData);
				CloseClipboard();
				return TextFromClipboard;
			}
			CloseClipboard();
			INT error = 0;
			std::wstring werror = L"ERROR " + std::to_wstring(error = GetLastError()) + L" - In Clipboard No Text Data!\r\n";
			OutputDebugString(werror.c_str());
			return "ERROR -2";
		}
		CloseClipboard();
		INT error = 0;
		std::wstring werror = L"ERROR" + std::to_wstring(error = GetLastError()) + L" - Clipboard is taken by another program!\r\n";
		OutputDebugString(werror.c_str());
		return "ERROR -1";
	}

	static  HWND* createDynamicControl(LPWSTR Class, LPWSTR Caption, DWORD Styles, RECT Dimensions, HWND ParentWindow, HMENU ID) {

		/// <summary>
		/// Creates Dynamic Control
		/// After Destroying This Control - Invoke This function "Delete hwnd"
		/// </summary>
		/// <param name="hwnd">Pointer To Control Handler</param>
		/// <param name="Class">Control Class</param>
		/// <param name="Caption">Control Caption</param>
		/// <param name="Styles">Control Styles</param>
		/// <param name="Dimensions">Control Dimensions</param>
		/// <param name="ParentWindow">Parent Window</param>
		/// <param name="Id">Control Id</param>

		HWND *hwnd = new HWND;

		if (!(*hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
			Class,
			Caption,
			Styles,
			Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom,
			ParentWindow,
			ID,
			NULL,
			NULL))) {
			return hwnd;
		}
	}

	static VOID ShowError(HWND ParentWindow = HWND_DESKTOP, std::string AdditionalErrorMessage = " - Error") {

		/// <summary>
		/// Shows Error Message
		/// </summary>
		/// <param name="ParentWindow">Parent Window</param>
		/// <param name="AdditionalErroMessage">Additional Error Message</param>

		std::string SError;
		std::string SErrorMessage = "ERROR " + (SError = _itos(GetLastError(), 10)) + AdditionalErrorMessage;

		MessageBoxA(ParentWindow, SErrorMessage.c_str(), "ERROR", MB_OK | MB_ICONERROR);

	}

	static std::vector <std::string> FindTextFromFileBySymbol(std::string FilePath, const char Symbol[]) {

		/// <summary>
		/// Finds all Lines in File with matching Symbol
		/// </summary>
		/// <param name="FilePath">File Path</param>
		/// <param name="Symbol">Symbol To Be Checked</param>
		/// <returns>If Succeeded Returns Vector with Strings, but If not "ERROR"</returns>

		std::fstream file;

		std::vector <std::string> TextArray = { "ERROR" };

		file.open(FilePath, std::ios::in);

		if (!file.is_open()) {

			return TextArray;

		}
		else {

			TextArray.clear();
			TextArray.shrink_to_fit();

			std::string buffer = "";

			while (!file.eof()) {

				std::getline(file, buffer);

				if (buffer.find(Symbol, 0) != std::string::npos) {

					TextArray.push_back(buffer);

				}

			}

			file.close();

			return TextArray;

		}

	}

};

#endif
