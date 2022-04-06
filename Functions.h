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

	static std::wstring _itow(int64_t Value) {

		/// <summary>
		/// Converts Integer To WStringValue
		/// </summary>
		/// <param name="Value">Integer Type Value To Be Converted</param>
		/// <returns>Converted Value</returns>

		// 1234 % 10 = 4 | 1234 / 10 = 123 || 123 % 10 = 3 | 123 / 10 = 12 || 12 % 10 = 2 | 12 / 10 = 1 | 1 % 10 = 1 | 1 / 10 = 0

		BOOL Minus = FALSE;
		CONST SHORT ASCI_VALUE_ZERO = 48;

		std::wstring WStringValue = L"";

		if (Value == INT64_MIN) { //
			return L"OVERFLOW";   // OVERFLOW
		}                         //

		if (Value * -1 > 0) { // -Value * -1 = Value | Value * -1 = -Value
			Minus = TRUE;
			Value = Value * -1;
		}

		do {

			CHAR Char = Value % 10 + ASCI_VALUE_ZERO; // Get Last Number - Char Value 0 | 48 - 9 | 57
			Value = Value / 10;  // Remove Last Value Number
			WStringValue.insert(WStringValue.begin(), 1, (wchar_t)Char); // WString Value

		} while (Value != 0);

		if (Minus == TRUE) {
			WStringValue.insert(WStringValue.begin(), 1, L'-');
		}

		return WStringValue;

	}

	static std::wstring _ftow(DOUBLE Value, UINT Precision) {

		/// <summary>
		/// Converts Double To WStringValue
		/// </summary>
		/// <param name="Value">Double Type Value To Be Converted</param>
		/// <param name="Precision"></param>
		/// <returns>Converted Value</returns>

		CONST SHORT ASCII_VALUE_ZERO = 48;

		std::wstring WStringValue = L"";

		if ((WStringValue = _itow((int64_t)Value)) == L"OVERFLOW") { return L"OVERFLOW"; } // Convert Integer Portion of Value - |1234|.1234
		Value = Value - (int64_t)Value; // Clear Integer Portion of Value - |0|.1234

		WStringValue = WStringValue + L"."; // Add Dot

		if (Value * -1 > 0) { Value = Value * -1; } // -Value * -1 = Value || Value * -1 = -Value

		for (UINT I = 0; I < Precision; I++) {
			Value = Value / 0.1; // 0.1234 / 0.1 = 1.234 <-
			if ((int64_t)Value == 0) {
				CHAR Char = (INT)Value + ASCII_VALUE_ZERO; // Char Value  0 | 48 - 9 | 57
				WStringValue = WStringValue + (wchar_t)Char; // Char Value To Symbol
			}
		}

		Value = round(Value);

		if ((WStringValue = WStringValue + _itow((int64_t)Value)) == L"OVERFLOW") { return L"OVERFLOW"; }  // Convert Decimal Portion of Value - 1234.|1234|

		return WStringValue;

	}

	static std::wstring EncryptText(std::wstring Text) {

		std::wstring EncryptedText = L"";

		for (size_t i = 0; i < Text.length(); i++) {

			INT CharValue = Text[i];
			EncryptedText = EncryptedText + _itow(CharValue) + L"|";

		}

	}

	static std::wstring DecryptText(std::wstring EncryptedText) {

		std::wstring Text = L"";

		while (EncryptedText.length() != 0) {

			INT CharValue = _wtoi(EncryptedText.c_str());
			Text = Text + (wchar_t)(char)CharValue;
			INT SeperatorPosition = (INT)EncryptedText.find_first_of(L'|', 0);
			for (INT i = SeperatorPosition; i >= 0; i--) {
				EncryptedText[i] = L' ';
			}

		}

	}

	static INT FindChar(LPWSTR Text, const wchar_t Char, INT TextLength) {

		for (int i = 0; i < TextLength; i++) {

			if (Text[i] == Char) {
				return i;
			}

		}

		return -1;

	}

	static bool copyTextToClipboard(HWND hwnd, std::wstring text) {

		/// <summary>
		/// This Function Puts Text into ClipBoard
		/// </summary>
		/// <param name="hwnd">newClipBoardOwner</param>
		/// <param name="text">Text into ClipBoard</param>
		/// <returns>If Function Succeeded Returns True, but If not False</returns>

		if (text.length() != 0) {
			if (OpenClipboard(hwnd)) {
				HLOCAL CopyData = { 0 };
				VOID *buffer = NULL;
				EmptyClipboard();
				CopyData = LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT | LMEM_VALID_FLAGS, sizeof(WCHAR) * text.length());
				if (CopyData != NULL) {
					buffer = LocalLock(CopyData);
				}
				if (buffer != NULL) {
					memcpy(buffer, (void*)text.c_str(), sizeof(WCHAR) * text.length());
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

	static bool copyTextToClipboard(HWND hwnd, std::string text) {

		/// <summary>
		/// This Function Puts Text into ClipBoard
		/// </summary>
		/// <param name="hwnd">newClipBoardOwner</param>
		/// <param name="text">Text into ClipBoard</param>
		/// <returns>If Function Succeeded Returns True, but If not False</returns>

		if (text.length()) {
			if (OpenClipboard(hwnd)) {
				HLOCAL CopyData = { 0 };
				VOID *buffer = NULL;
				EmptyClipboard();
				CopyData = LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT | LMEM_VALID_FLAGS, sizeof(CHAR) * text.length());
				if (CopyData != NULL) {
					buffer = LocalLock(CopyData);
				}
				if (buffer != NULL) {
					memcpy(buffer, (void*)text.c_str(), sizeof(CHAR) * text.length());
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
				LocalFree(ClipboardData);
				CloseClipboard();
				return TextFromClipboard;
			}
			CloseClipboard();
			std::wstring werror = L"ERROR " + _itow(GetLastError()) + L" - In Clipboard No Text Data!\r\n";
			OutputDebugString(werror.c_str());
			return L"ERROR -2";
		}
		CloseClipboard();
		std::wstring werror = L"ERROR" + _itow(GetLastError()) + L" - Clipboard is taken by another program!\r\n";
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
			VOID *buffer = NULL;
			std::string TextFromClipboard = "";
			ClipboardData = LocalAlloc(LMEM_MOVEABLE | LMEM_ZEROINIT | LMEM_VALID_FLAGS, sizeof(std::string));
			if (ClipboardData != NULL) {
				ClipboardData = GetClipboardData(CF_TEXT);
				buffer = LocalLock(ClipboardData);
			}
			if (buffer != NULL) {
				TextFromClipboard = (char*)buffer;
				LocalUnlock(ClipboardData);
				LocalFree(ClipboardData);
				CloseClipboard();
				return TextFromClipboard;
			}
			CloseClipboard();
			std::wstring werror = L"ERROR " + _itow(GetLastError()) + L" - In Clipboard No Text Data!\r\n";
			OutputDebugString(werror.c_str());
			return "ERROR -2";
		}
		CloseClipboard();
		std::wstring werror = L"ERROR" + _itow(GetLastError()) + L" - Clipboard is taken by another program!\r\n";
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
		ZeroMemory(hwnd, sizeof(HWND));

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

	static VOID ShowError(HWND ParentWindow = HWND_DESKTOP, std::wstring AdditionalErrorMessage = L" - Error") {

		/// <summary>
		/// Shows Error Message
		/// </summary>
		/// <param name="ParentWindow">Parent Window</param>
		/// <param name="AdditionalErroMessage">Additional Error Message</param>
		
		std::wstring ErrorMessage = L"ERROR " + _itow(GetLastError()) + AdditionalErrorMessage;

		MessageBox(ParentWindow, ErrorMessage.c_str(), L"ERROR", MB_OK | MB_ICONERROR);

	}

	static VOID SaveBitmapToFile(HBITMAP Bitmap, std::string FilePath) {

		std::fstream image;

		SIZE BitmapSize = { 0 };
		GetBitmapDimensionEx(Bitmap, &BitmapSize);

		BITMAPFILEHEADER type = { 0 };
		type.bfSize = sizeof(Bitmap);
		type.bfType = IMAGE_BITMAP;
		type.bfOffBits = sizeof(BITMAPFILEHEADER);

		BITMAPINFOHEADER info = { 0 };
		info.biSize = sizeof(info);


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
