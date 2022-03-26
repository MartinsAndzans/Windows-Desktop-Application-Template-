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

	static std::string _itos(int number) {

		/// <summary>
		/// Converts Integer To String
		/// </summary>
		/// <param name="number">Integer Type Number To Be Converted</param>
		/// <returns>Converted Number</returns>

		char s[20] = "";
		std::string string = "";
		_itoa_s(number, s, 10);
		string.insert(0, s);
		return string;
	}

	static std::string _dtos(double number) {

		/// <summary>
		/// Converts Double To String
		/// </summary>
		/// <param name="number">Double Type Number To Be Converted</param>
		/// <returns>Converted Number</returns>

		char i[20] = "";
		char f[20] = "";
		std::string string = "";
		_itoa_s((int)number, i, 10);
		double dnumber = number - (int)number;
		double fnumber = dnumber * 1000000;
		_itoa_s((int)fnumber, f, 10);
		string.insert(0, i);
		string = string + ".";
		int64_t length = string.length();
		string.insert(length, f);
		return string;
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

	static VOID createDynamicControl(HWND *hwnd, LPWSTR Class, LPWSTR Caption, DWORD Styles, RECT Dimensions, HWND ParentWindow, HMENU ID) {

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

		hwnd = new HWND;

		if (!(*hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
			Class,
			Caption,
			Styles,
			Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom,
			ParentWindow,
			ID,
			NULL,
			NULL))) {
			int error;
			std::string si;
			error = GetLastError();
			std::string text = "ERROR " + (si = _itos(error)) + " - Child Window is not Created!";
			std::wstring wtext = std::wstring(text.begin(), text.end());
			MessageBox(HWND_DESKTOP, wtext.c_str(), L"ERROR", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
		}
	}

	static VOID ShowError(std::string AdditionalErrorMessage) {

		/// <summary>
		/// Shows Error Message
		/// </summary>
		/// <param name="AdditionalErroMessage">Additional Error Message</param>

		std::string SError;
		int Error = GetLastError();
		std::string SErrorMessage = "ERROR " + (SError = _itos(Error)) + AdditionalErrorMessage;

		MessageBoxA(HWND_DESKTOP, SErrorMessage.c_str(), "ERROR", MB_OK | MB_ICONERROR);

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
