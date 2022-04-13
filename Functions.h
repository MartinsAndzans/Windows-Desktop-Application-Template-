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

	/// <summary>
	/// Converts Integer To WStringValue
	/// <para>1234 % 10 = 4 | 1234 / 10 = 123 || 123 % 10 = 3 | 123 / 10 = 12 || 12 % 10 = 2 | 12 / 10 = 1 | 1 % 10 = 1 | 1 / 10 = 0</para>
	/// </summary>
	/// <param name="Value">- Integer Type [int64_t] Value To Be Converted</param>
	/// <returns>Converted Value</returns>
	static std::wstring _itow(int64_t Value) {

		BOOL Minus = FALSE;
		CONST SHORT ASCI_VALUE_ZERO = 48;

		std::wstring WStringValue = L"";

		if (Value == INT64_MIN) { //
			return L"OVERFLOW";   // OVERFLOW
		}                         

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

	/// <summary>
	/// Converts Double To WStringValue
	/// </summary>
	/// <param name="Value">- Double Type Value To Be Converted</param>
	/// <param name="Precision">- Decimal Precision</param>
	/// <returns>Converted Value</returns>
	static std::wstring _ftow(DOUBLE Value, UINT Precision) {

		CONST SHORT ASCII_VALUE_ZERO = 48;

		std::wstring WStringValue = L"";

		if ((WStringValue = _itow((int64_t)Value)) == L"OVERFLOW") { return L"OVERFLOW"; } // Convert Integer Portion of Value - |1234|.1234
		Value = Value - (int64_t)Value; // Clear Integer Portion of Value - |0|.1234

		WStringValue = WStringValue + L"."; // Add Dot

		if (Value * -1 > 0) { Value = Value * -1; } // -Value * -1 = Value || Value * -1 = -Value

		for (UINT I = 0; I < Precision; I++) {
			Value = Value / 0.1; // 0.1234 / 0.1 = 1.234 <-
			if ((int64_t)Value == 0) {
				CHAR Char = (int)Value + ASCII_VALUE_ZERO; // Char Value  0 | 48 - 9 | 57
				WStringValue = WStringValue + (wchar_t)Char; // Char Value To Symbol
			}
		}

		Value = round(Value);

		if ((WStringValue = WStringValue + _itow((int64_t)Value)) == L"OVERFLOW") { return L"OVERFLOW"; }  // Convert Decimal Portion of Value - 1234.|1234|

		return WStringValue;

	}

	/// <summary>
	/// Encrypt Text To ASCII Value Code
	/// </summary>
	/// <param name="Text">- Text To Be Encrypted</param>
	/// <returns>Encrypted Text</returns>
	static std::wstring EncryptText(std::wstring Text) {

		std::wstring EncryptedText = L"";

		for (size_t i = 0; i < Text.length(); i++) {
			INT CharValue = Text[i];
			EncryptedText = EncryptedText + _itow(CharValue) + L"|";
		}

		return EncryptedText;

	}

	/// <summary>
	/// Decrypted Text From ASCII Value Code
	/// </summary>
	/// <param name="EncryptedText">- Text To Be Decrypted</param>
	/// <returns>Decrypted Text</returns>
	static std::wstring DecryptText(std::wstring EncryptedText) {

		std::wstring Text = L"";

		while (EncryptedText.length() != 0) {
			INT CharValue = _wtoi(EncryptedText.c_str());
			Text = Text + (wchar_t)(char)CharValue;
			size_t SeperatorPosition = EncryptedText.find_first_of(L'|', 0);
			for (int i = (int)SeperatorPosition; i >= 0; i--) {
				EncryptedText.erase(i);
			}
		}

		return Text;

	}

	/// <summary>
	/// Finds Character Position Into String
	/// </summary>
	/// <param name="Text">- [WCHAR] Text</param>
	/// <param name="Char">- Character To Find</param>
	/// <param name="TextLength">- Text Length Without [null character] - [\0]</param>
	/// <returns>If Character Has Been Found Returns Character Position, but If not -1</returns>
	static INT FindChar(LPWSTR Text, const wchar_t Char, INT TextLength) {

		for (int i = 0; i < TextLength; i++) {
			if (Text[i] == Char) {
				return i;
			}
		}

		return -1;

	}

	/// <summary>
	/// This Function Copy Unicode Text into Clipboard
	/// </summary>
	/// <param name="Hwnd">- New Clipboard Owner</param>
	/// <param name="WText">- WText into Clipboard</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL CopyTextToClipboard(HWND Hwnd, std::wstring WText) {

		if (WText.length() != 0) {

			if (OpenClipboard(Hwnd)) {
				EmptyClipboard();
				HLOCAL CopyData = LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT | LMEM_VALID_FLAGS, sizeof(WCHAR) * WText.length() + 1);
				if (CopyData == NULL) {
					CloseClipboard();
					return FALSE;
				}
				VOID *buffer = LocalLock(CopyData);
				if (buffer == NULL) {
					LocalFree(CopyData);
					CloseClipboard();
					return FALSE;
				}
				memcpy(buffer, (void*)WText.c_str(), sizeof(WCHAR) * WText.length() + 1);
				LocalUnlock(CopyData);
				SetClipboardData(CF_UNICODETEXT, CopyData);
				LocalFree(CopyData);
				CloseClipboard();
				return TRUE;
			}

		}

		return FALSE;

	}

	/// <summary>
	/// This Function Copy Text into Clipboard
	/// </summary>
	/// <param name="Hwnd">- New Clipboard Owner</param>
	/// <param name="Text">- Text into Clipboard</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL CopyTextToClipboard(HWND Hwnd, std::string Text) {

		if (Text.length() != 0) {

			if (OpenClipboard(Hwnd)) {
				EmptyClipboard();
				HLOCAL CopyData = LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT | LMEM_VALID_FLAGS, sizeof(CHAR) * Text.length() + 1);
				if (CopyData == NULL) {
					CloseClipboard();
					return FALSE;
				}
				VOID *buffer = LocalLock(CopyData);
				if (buffer == NULL) {
					LocalFree(CopyData);
					CloseClipboard();
					return FALSE;
				}
				memcpy(buffer, (void*)Text.c_str(), sizeof(CHAR) * Text.length() + 1);
				LocalUnlock(CopyData);
				SetClipboardData(CF_TEXT, CopyData);
				LocalFree(CopyData);
				CloseClipboard();
				return TRUE;
			}

		}

		return FALSE;

	}

	/// <summary>
	/// This Function Gets WText From Clipboard
	/// </summary>
	/// <param name="Hwnd">- New Clipboard Owner</param>
	/// <returns>
	/// If Function Succeeded Returns Received WText, but If not Error Code:
	/// <para>"-1" - Clipboard Is Taken by Another Program!</para>
	/// <para>"-2" - Clipboard Is Empty!</para>
	/// </returns>
	static std::wstring GetClipboardWText(HWND Hwnd) {

		if (OpenClipboard(Hwnd)) {
			std::wstring WText = L"";
			HLOCAL ClipboardData = GetClipboardData(CF_UNICODETEXT);
			if (ClipboardData == NULL) {
				CloseClipboard();
				return L"-2";
			}
			VOID *buffer = LocalLock(ClipboardData);
			if (buffer == NULL) {
				LocalFree(ClipboardData);
				CloseClipboard();
				return L"-2";
			}
			WText = (wchar_t*)buffer;
			LocalUnlock(ClipboardData);
			LocalFree(ClipboardData);
			CloseClipboard();
			return WText;
		}

		return L"-1";

	}

	/// <summary>
	/// This Function Gets Text From Clipboard
	/// </summary>
	/// <param name="Hwnd">- New Clipboard Owner</param>
	/// <returns>
	/// If Function Succeeded Returns Received Text, but If not Error Code:
	/// <para>"-1" - Clipboard Is Taken by Another Program!</para>
	/// <para>"-2" - Clipboard Is Empty!</para>
	/// </returns>
	static std::string GetClipboardText(HWND Hwnd) {

		if (OpenClipboard(Hwnd)) {
			std::string Text = "";
			HLOCAL ClipboardData = GetClipboardData(CF_TEXT);
			if (ClipboardData == NULL) {
				CloseClipboard();
				return "-2";
			}
			VOID *buffer = LocalLock(ClipboardData);
			if (buffer == NULL) {
				LocalFree(ClipboardData);
				CloseClipboard();
				return "-2";
			}
			Text = (char*)buffer;
			LocalUnlock(ClipboardData);
			LocalFree(ClipboardData);
			CloseClipboard();
			return Text;
		}

		return "-1";

	}

	/// <summary>
	/// Creates Control On Heap Memory
	/// <para>After Destroying This Control - Invoke This function [Delete Hwnd]</para>
	/// </summary>
	/// <param name="Class">- Control Class</param>
	/// <param name="Caption">- Control Caption</param>
	/// <param name="Styles">- Control Styles</param>
	/// <param name="Dimensions">- Control Dimensions</param>
	/// <param name="ParentWindow">- Parent Window</param>
	/// <param name="Id">- Control Id</param>
	/// <returns>Pointer To Control Handle</returns>
	static  HWND* CreateControlOnHeapMemory(LPWSTR Class, LPWSTR Caption, DWORD Styles, RECT Dimensions, HWND ParentWindow, HMENU ID) {

		HWND *Hwnd = new HWND;
		ZeroMemory(Hwnd, sizeof(HWND));

		if (!(*Hwnd = CreateWindowEx(WS_EX_STATICEDGE,
			Class,
			Caption,
			Styles,
			Dimensions.left, Dimensions.top, Dimensions.right, Dimensions.bottom,
			ParentWindow,
			ID,
			NULL,
			NULL))) {
			return Hwnd;
		}

		delete Hwnd;
		return NULL;

	}

	/// <summary>
	/// Shows Error Message
	/// </summary>
	/// <param name="ParentWindow">- Parent Window</param>
	/// <param name="AdditionalErroMessage">- Additional Error Message</param>
	static VOID ShowError(HWND ParentWindow = HWND_DESKTOP, std::wstring AdditionalErrorMessage = L" - Error") {
		
		std::wstring ErrorMessage = L"ERROR " + _itow(GetLastError()) + AdditionalErrorMessage;
		MessageBox(ParentWindow, ErrorMessage.c_str(), L"ERROR", MB_OK | MB_ICONERROR);

	}

	/// <summary>
	/// Saves Bitmap To File
	/// </summary>
	/// <param name="Bitmap">- Bitmap</param>
	/// <param name="FilePath">- File Path With ".bmp" Extension</param>
	/// <param name="BitmapSize">- Bitmap Size In Pixels</param>
	/// <returns>If Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL SaveBitmapToFile(HBITMAP Bitmap, std::string FilePath, SIZE &BitmapSize) {

		#define BITMAP_SIZE_IN_PIXELS BitmapSize.cx * BitmapSize.cy
		#define BITMAP_SIZE_IN_BYTES sizeof(COLORREF) * ((int64_t)BitmapSize.cx * (int64_t)BitmapSize.cy)

		std::fstream image;

		CONST WORD BM = 0x4D42; // ASCII B - Decimal = 66 / Hex = 42 | M - Decimal = 77 / Hex = 4D

		BITMAPFILEHEADER bmfheader = { 0 };
		bmfheader.bfType = BM;
		bmfheader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + BITMAP_SIZE_IN_BYTES; // File Size
		bmfheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // Offset To Color Bits
		bmfheader.bfReserved1 = NULL;
		bmfheader.bfReserved2 = NULL;

		BITMAPINFOHEADER bmiheader = { 0 };
		bmiheader.biSize = sizeof(BITMAPINFOHEADER);
		bmiheader.biWidth = BitmapSize.cx;
		bmiheader.biHeight = BitmapSize.cy;
		bmiheader.biPlanes = 0x01; // 1 - Bitmap has a one plane
		bmiheader.biBitCount = 0x20; // 32 - Bitmap has a maximum of 2^32 colors
		bmiheader.biCompression = BI_RGB;
		bmiheader.biSizeImage = BITMAP_SIZE_IN_PIXELS;
		bmiheader.biXPelsPerMeter = BitmapSize.cx;
		bmiheader.biYPelsPerMeter = BitmapSize.cy;
		bmiheader.biClrUsed = NULL;
		bmiheader.biClrImportant = NULL;

		COLORREF *BitmapBits = new COLORREF[BITMAP_SIZE_IN_PIXELS];
		if (BitmapBits == NULL) { return FALSE; }
		ZeroMemory(BitmapBits, BITMAP_SIZE_IN_BYTES);

		HDC ScreenDC = GetDC(NULL);
		HDC MemoryDC = CreateCompatibleDC(ScreenDC);

		BITMAPINFO bminfo = { 0 };
		bminfo.bmiHeader = bmiheader;

		GetDIBits(MemoryDC, Bitmap, 0, BitmapSize.cy, BitmapBits, &bminfo, DIB_RGB_COLORS);

		ReleaseDC(HWND_DESKTOP, ScreenDC);
		DeleteDC(MemoryDC);

		image.open(FilePath, std::ios::out | std::ios::binary); // Open File

		if (!image.is_open()) {
			delete[] BitmapBits;
			return FALSE;
		}
		else {
			image.write((char*)&bmfheader, sizeof(BITMAPFILEHEADER)); // BITMAP FILE HEADER
			image.write((char*)&bmiheader, sizeof(BITMAPINFOHEADER)); // BITMAP INFO HEADER
			image.write((char*)BitmapBits, BITMAP_SIZE_IN_BYTES); // BYTE ARRAY
		}

		image.close(); // Close File
		delete[] BitmapBits; // Delete Bitmap Byte Array

		return TRUE;

		#undef BITMAP_SIZE_IN_PIXELS
		#undef BITMAP_SIZE_IN_BYTES

	}

	/// <summary>
	/// Finds ALL Lines in File With Matching Symbol
	/// </summary>
	/// <param name="FilePath">- File Path</param>
	/// <param name="Symbol">- Symbol To Be Checked</param>
	/// <returns>If Succeeded Returns Vector with Strings, but If not NULL</returns>
	static std::vector <std::string> FindTextFromFileBySymbol(std::string FilePath, const char Symbol[]) {

		std::fstream file;

		std::vector <std::string> TextArray;
		TextArray.reserve(1);

		file.open(FilePath, std::ios::in);

		if (!file.is_open()) {
			return TextArray;
		}
		else {

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

class Sound {

public:

	typedef DWORD MCISTATUS;

	static MCIERROR Open(HWND CallbackWindow, std::wstring FilePath, const wchar_t *Alias) {

		MCI_OPEN_PARMS open = { 0 };
		open.dwCallback = (DWORD_PTR)CallbackWindow;
		open.lpstrAlias = Alias;
		open.lpstrDeviceType = NULL;
		open.lpstrElementName = FilePath.c_str();
		open.wDeviceID = NULL;

		MCIERROR Error = mciSendCommand(NULL, MCI_OPEN, MCI_WAIT | MCI_OPEN_ELEMENT | MCI_OPEN_ALIAS, (DWORD_PTR)&open);

		return Error;

	}

	/// <summary>
	/// > This Function Gets Playback Status
	/// <para>> MCI_STATUS_POSITION - Gets Current Playback Position</para>
	/// <para>> MCI_STATUS_LENGTH - Gets Total Media Length</para>
	/// <para>> MCI_STATUS_MODE - Gets Current Mode of The Device</para>
	/// <para>>> [MODE EXAMPLE] - MCI_MODE_[]</para>
	/// <para>> MCI_STATUS_TIME_FORMAT - Gets Current Time Format of The Device</para>
	/// <para>>> [TIME FORMAT EXAMPLE] = MCI_FORMAT_[]</para>
	/// </summary>
	/// <param name="Alias">- Alias of MCIDevice</param>
	/// <param name="StatusCode">- [EXAMPLE] - MCI_STATUS_[]</param>
	/// <returns>If Succeeded Returns Specified Status Information, but If not Returns MAXDWORD</returns>
	static MCISTATUS GetPlaybackStatus(const wchar_t *Alias, MCISTATUS StatusCode) {
		
		MCIDEVICEID ID = mciGetDeviceID(Alias);

		MCI_STATUS_PARMS status = { 0 };
		status.dwItem = StatusCode;
		status.dwReturn = NULL;

		MCIERROR Error = mciSendCommand(ID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD_PTR)&status);

		if (Error != 0) {
			status.dwReturn = MAXDWORD;
		}

		return (DWORD)status.dwReturn;

	}

	static MCIERROR Play(HWND CallbackWindow, const wchar_t *Alias, BOOL Repeat = FALSE) {

		MCIDEVICEID ID = mciGetDeviceID(Alias);

		MCI_PLAY_PARMS play = { 0 };
		play.dwCallback = (DWORD_PTR)CallbackWindow;
		play.dwFrom = 0;

		MCIERROR Error = mciSendCommand(ID, MCI_PLAY, MCI_FROM, (DWORD_PTR)&play);

		return Error;
	}

	static MCIERROR Seek(const wchar_t *Alias, DWORD SeekTo) {

		MCI_SEEK_PARMS seek = { 0 };
		seek.dwTo = SeekTo;

		MCIDEVICEID ID = mciGetDeviceID(Alias);
		MCIERROR Error = mciSendCommand(ID, MCI_SEEK, MCI_WAIT, (DWORD_PTR)&seek);

	}

	static MCIERROR Pause(const wchar_t *Alias) {

		MCIDEVICEID ID = mciGetDeviceID(Alias);
		MCIERROR Error = mciSendCommand(ID, MCI_PAUSE, MCI_WAIT, NULL);

		return Error;

	}

	static MCIERROR Resume(const wchar_t *Alias) {

		MCIDEVICEID ID = mciGetDeviceID(Alias);
		MCIERROR Error = mciSendCommand(ID, MCI_RESUME, MCI_WAIT, NULL);

		return Error;

	}

	static MCIERROR Stop(const wchar_t *Alias) {

		MCIDEVICEID ID = mciGetDeviceID(Alias);
		MCIERROR Error = mciSendCommand(ID, MCI_STOP, MCI_WAIT, NULL);

		return Error;

	}

	static MCIERROR Close(const wchar_t *Alias) {

		MCIDEVICEID ID = mciGetDeviceID(Alias);
		MCIERROR Error = mciSendCommand(ID, MCI_CLOSE, MCI_WAIT, NULL);

		return Error;

	}

};

#endif
