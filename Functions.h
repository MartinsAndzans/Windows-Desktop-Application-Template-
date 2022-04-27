
#pragma once

#ifndef _FUNCTIONS_
#define _FUNCTIONS_

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#include <Windows.h>
#include <ciso646>
#include <string>
#include <fstream>
#include <math.h>

class Functions {

public:

	/// <summary>
	/// Converts Integer To Char Array
	/// <para>1234 % 10 = 4 | 1234 / 10 = 123 || 123 % 10 = 3 | 123 / 10 = 12 || 12 % 10 = 2 | 12 / 10 = 1 || 1 % 10 = 1 | 1 / 10 = 0</para>
	/// </summary>
	/// <param name="Value">- Integer Type [int64_t] Value To Be Converted</param>
	/// <param name="Buffer">Char Buffer</param>
	/// <param name="BufferSize">Buffer Size</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL _itoa(int64_t Value, LPSTR Buffer, SIZE_T BufferSize) {

		BOOL Minus = FALSE;
		CONST SHORT ASCI_VALUE_ZERO = 48; // * Char Value 0 / 48 *

		std::string StringValue{};

		if (Value == INT64_MIN) {
			OutputDebugString(L"\'[Functions::_itoa] - Value \"OVERFLOW\"\'\r\n");
			return FALSE;
		}

		if (Value * -1 > 0) Minus = TRUE, Value *= -1; // -Value * -1 = (Value > 0) | Value * -1 = (-Value <= 0)

		do {

			CHAR Char = Value % 10 + ASCI_VALUE_ZERO; // Get Last Number - Char Value 0 / 48 - 9 / 57
			Value = Value / 10;  // Remove Last Number
			StringValue.insert(0, 1, Char);

		} while (Value != 0);
		
		if (Minus) StringValue.insert(0, "-");

		if (BufferSize >= strlen(StringValue.c_str()) + 1) {
			strcpy_s(Buffer, BufferSize, StringValue.c_str());
		} else {
			OutputDebugString(L"\'[Functions::_itoa] - Buffer To Small\'\r\n");
			return FALSE;
		}

		return TRUE;

	}

	/// <summary>
	/// Encrypt Text To ASCII Value Code
	/// </summary>
	/// <param name="Text">- Text To Be Encrypted</param>
	/// <returns>Encrypted Text</returns>
	static std::string EncryptText(CONST std::string &Text) {

		std::string EncryptedText{};

		for (CHAR Character : Text) {
			SHORT ASCII_VALUE = Character;
			EncryptedText += std::to_string(ASCII_VALUE) + "|";
		}

		return EncryptedText;

	}

	/// <summary>
	/// Decrypted Text From ASCII Value Code
	/// </summary>
	/// <param name="EncryptedText">- Text To Be Decrypted</param>
	/// <returns>Decrypted Text</returns>
	static std::string DecryptText(std::string EncryptedText) {

		std::string DecryptedText{};

		while (EncryptedText.length() != 0) {
			if (EncryptedText.back() == '|') {
				CHAR Character = std::stoi(EncryptedText);
				DecryptedText += Character;
				EncryptedText.replace(0, EncryptedText.find('|') + 1, "");
			} else {
				return DecryptedText;
			}
		}

		return DecryptedText;

	}

	/// <summary>
	/// Finds Character Position Into String
	/// </summary>
	/// <param name="Text">- [CHAR] Text</param>
	/// <param name="Char">- Character To Find</param>
	/// <param name="TextLength">- Text Length Without [null character] - [\0]</param>
	/// <returns>If Character Has Been Found returns Character Position, but If not -1</returns>
	static SIZE_T FindChar(LPSTR Text, CONST CHAR Char, SIZE_T TextLength) {

		for (SIZE_T I = 0; I < TextLength; I++) {
			if (Text[I] == Char) {
				return I;
			}
		}

		return -1;

	}

	/// <summary>
	/// Rounds Double String
	/// </summary>
	/// <param name="Text">- Double String</param>
	/// <returns>Rounded Double String</returns>
	static std::string RoundDoubleString(std::string DoubleString) {

		if (DoubleString.find('.') != std::string::npos) {

			while (DoubleString.back() == '0' or DoubleString.back() == '.') {

				if (DoubleString.back() == '.') {
					DoubleString.pop_back();
					return DoubleString;
				}
				DoubleString.pop_back();
			}
			return DoubleString;
		}
		return DoubleString;
	}

	/// <summary>
	/// This Function Copy Text into Clipboard
	/// </summary>
	/// <param name="Hwnd">- New Clipboard Owner</param>
	/// <param name="Text">- Text into Clipboard</param>
	/// <returns>If Function Succeeded returns TRUE, but If not returns FALSE</returns>
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
	/// This Function Copy Unicode Text into Clipboard
	/// </summary>
	/// <param name="Hwnd">- New Clipboard Owner</param>
	/// <param name="WText">- WText into Clipboard</param>
	/// <returns>If Function Succeeded returns TRUE, but If not returns FALSE</returns>
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
	/// This Function Gets Text From Clipboard
	/// </summary>
	/// <param name="Hwnd">- New Clipboard Owner</param>
	/// <returns>
	/// If Function Succeeded returns Received Text, but If not Error Code:
	/// <para>"-1" - Clipboard Is Taken by Another Program!</para>
	/// <para>"-2" - Clipboard Is Empty!</para>
	/// </returns>
	static std::string GetClipboardText(HWND Hwnd) {

		if (OpenClipboard(Hwnd)) {
			std::string Text{};
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
	/// This Function Gets WText From Clipboard
	/// </summary>
	/// <param name="Hwnd">- New Clipboard Owner</param>
	/// <returns>
	/// If Function Succeeded returns Received WText, but If not Error Code:
	/// <para>"-1" - Clipboard Is Taken by Another Program!</para>
	/// <para>"-2" - Clipboard Is Empty!</para>
	/// </returns>
	static std::wstring GetClipboardWText(HWND Hwnd) {

		if (OpenClipboard(Hwnd)) {
			std::wstring WText{};
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
	/// Shows Error Message
	/// </summary>
	/// <param name="ParentWindow">- Parent Window</param>
	/// <param name="AdditionalErroMessage">- Additional Error Message</param>
	static VOID ShowError(HWND ParentWindow = HWND_DESKTOP, std::string AdditionalErrorMessage = " - Error") {

		std::string ErrorMessage = "ERROR " + std::to_string(GetLastError()) + AdditionalErrorMessage;
		MessageBoxA(ParentWindow, ErrorMessage.c_str(), "ERROR", MB_OK | MB_ICONERROR);

	}

	/// <summary>
	/// Saves Bitmap To File
	/// </summary>
	/// <param name="Bitmap">- Bitmap</param>
	/// <param name="FilePath">- File Path With ".bmp" Extension</param>
	/// <param name="BitmapSize">- Bitmap Size In Pixels</param>
	/// <returns>If Succeeded returns TRUE, but If not returns FALSE</returns>
	static BOOL SaveBitmapToFile(HBITMAP Bitmap, CONST CHAR* FilePath, SIZE &BitmapSize) {

		std::ofstream image;

		CONST WORD BM = 0x4D42; // ASCII "B" = 66 / 0x42 | "M" = 77 / 0x4D
		CONST DWORD BitmapSizeCXxCY = BitmapSize.cx * BitmapSize.cy; // Bitmap Size [CX * CY]

		BITMAPFILEHEADER bmfheader = { 0 };
		bmfheader.bfType = BM;
		bmfheader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(COLORREF) * BitmapSizeCXxCY; // File Size
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
		bmiheader.biSizeImage = BitmapSizeCXxCY;
		bmiheader.biXPelsPerMeter = BitmapSize.cx;
		bmiheader.biYPelsPerMeter = BitmapSize.cy;
		bmiheader.biClrUsed = NULL;
		bmiheader.biClrImportant = NULL;

		std::unique_ptr<COLORREF> BitmapBytes(new COLORREF[BitmapSizeCXxCY]{});

		#pragma region GetBitmapColorBytes
		HDC ScreenDC = GetDC(HWND_DESKTOP); // ScreenDC
		HDC MemoryDC = CreateCompatibleDC(ScreenDC); // MemoryDC From ScreenDC

		BITMAPINFO bminfo = { 0 };
		bminfo.bmiHeader = bmiheader;

		GetDIBits(MemoryDC, Bitmap, 0, BitmapSize.cy, BitmapBytes.get(), &bminfo, DIB_RGB_COLORS);

		ReleaseDC(HWND_DESKTOP, ScreenDC);
		DeleteDC(MemoryDC);
		#pragma endregion

		image.open(FilePath, std::ios::binary); // Open File

		if (image.is_open()) {

			image.write((char*)&bmfheader, sizeof(BITMAPFILEHEADER)); // BITMAP FILE HEADER
			image.write((char*)&bmiheader, sizeof(BITMAPINFOHEADER)); // BITMAP INFO HEADER
			image.write((char*)BitmapBytes.get(), sizeof(COLORREF) * BitmapSizeCXxCY); // COLOR BYTE ARRAY

			image.close(); // Close File

			return TRUE;

		}

		return FALSE;

	}

};

class Sound {

public:

	typedef DWORD MCISTATUS; // * MCISTATUS [DWORD] is a 32-bit unsigned integer *

	/// <summary>
	/// > This Function Opens MCIDevice
	/// </summary>
	/// <param name="FilePath">- Music File Path "*.wav" / "*.wma" / "*.mp3" || Video Formats not Supported Yet ||</param>
	/// <param name="Alias">- Alias for MCIDevice</param>
	/// <returns>If Succeeded Returns 0, but If not Returns MCIERROR Error Code</returns>
	static MCIERROR Open(LPCWSTR Alias, LPCWSTR FilePath) {

		MCI_OPEN_PARMS open = { 0 };
		open.lpstrAlias = Alias;
		open.lpstrElementName = FilePath;

		return mciSendCommand(NULL, MCI_OPEN, MCI_WAIT | MCI_OPEN_ELEMENT | MCI_OPEN_ALIAS, (DWORD_PTR)&open);

	}

	/// <summary>
	/// > This Function Obtains Playback Status
	/// <para>> MCI_STATUS_POSITION - Obtains Current Playback Position</para>
	/// <para>> MCI_STATUS_LENGTH - Obtains Total Media Length</para>
	/// <para>> MCI_STATUS_MODE - Obtains Current Mode of The Device</para>
	/// <para>> MCI_STATUS_TIME_FORMAT - Obtains Current Time Format of The Device</para>
	/// </summary>
	/// <param name="Alias">- Alias for MCIDevice</param>
	/// <param name="StatusCode">- Status Code</param>
	/// <param name="AdditionalFlags">- Additional Flags</param>
	/// <returns>If Succeeded Returns Requested Status Information, but If not Returns MCIERROR Error Code</returns>
	static MCISTATUS GetPlaybackStatus(LPCWSTR Alias, MCISTATUS StatusCode) {

		MCI_STATUS_PARMS status = { 0 };
		status.dwItem = StatusCode; // Status Code
		status.dwReturn = NULL; // Return

		MCIERROR Error = mciSendCommand(mciGetDeviceID(Alias), MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD_PTR)&status);

		if (Error != 0) {
			return Error;
		} else {
			return (DWORD)status.dwReturn;
		}

	}

	/// <summary>
	/// > This Function Plays Music From The Beginning
	/// <para>> If Notify is TRUE 'MCI Sends To Callback Window [MM_MCINOTIFY] Message "LOWORD(lParam) - MCIDeviceID"'</para>
	/// </summary>
	/// <param name="CallbackWindow">- Callback Window</param>
	/// <param name="Alias">- Alias for MCIDevice</param>
	/// <param name="Notify">- MCI_NOTIFY</param>
	/// <returns>If Succeeded returns 0, but If not returns MCIERROR Error Code</returns>
	static MCIERROR Play(HWND CallbackWindow, LPCWSTR Alias, BOOL Notify = FALSE) {

		MCI_PLAY_PARMS play = { 0 };
		play.dwCallback = (DWORD_PTR)CallbackWindow;
		play.dwFrom = 0;

		if (Notify) {
			return mciSendCommand(mciGetDeviceID(Alias), MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD_PTR)&play);
		} else {
			return mciSendCommand(mciGetDeviceID(Alias), MCI_PLAY, MCI_FROM, (DWORD_PTR)&play);
		}

	}

	/// <summary>
	/// > This Function Seek To Specified Point of Playback
	/// <para>> MCI_SEEK_TO_START - Seek To Beginning of Playback</para>
	/// <para>> MCI_SEEK_TO_END - Seek To End of Playback</para>
	/// </summary>
	/// <param name="Alias">- Alias for MCIDevice</param>
	/// <param name="SeekTo">- How Much Move Playback Current Position</param>
	/// <returns>If Succeeded returns 0, but If not returns MCIERROR Error Code</returns>
	static MCIERROR Seek(LPCWSTR Alias, DWORD SeekTo) {

		MCI_SEEK_PARMS seek = { 0 };
		seek.dwTo = SeekTo;

		if (SeekTo == MCI_SEEK_TO_START) {
			return mciSendCommand(mciGetDeviceID(Alias), MCI_SEEK, MCI_WAIT | MCI_SEEK_TO_START, NULL);
		} else if (SeekTo == MCI_SEEK_TO_END) {
			return mciSendCommand(mciGetDeviceID(Alias), MCI_SEEK, MCI_WAIT | MCI_SEEK_TO_END, NULL);
		} else {
			return mciSendCommand(mciGetDeviceID(Alias), MCI_SEEK, MCI_WAIT | MCI_TO, (DWORD_PTR)&seek);
		}

	}

	/// <summary>> This Function Pauses Playbeck</summary>
	/// <param name="Alias">- Alias for MCIDevice</param>
	/// <returns>If Succeeded returns 0, but If not returns MCIERROR Error Code</returns>
	static MCIERROR Pause(LPCWSTR Alias) {
		return mciSendCommand(mciGetDeviceID(Alias), MCI_PAUSE, MCI_WAIT, NULL);
	}

	/// <summary>> This Function Resumes Playbeck</summary>
	/// <param name="Alias">- Alias for MCIDevice</param>
	/// <returns>If Succeeded returns 0, but If not returns MCIERROR Error Code</returns>
	static MCIERROR Resume(LPCWSTR Alias) {
		return mciSendCommand(mciGetDeviceID(Alias), MCI_RESUME, MCI_WAIT, NULL);
	}

	/// <summary>> This Function Stops Playbeck</summary>
	/// <param name="Alias">- Alias for MCIDevice</param>
	/// <returns>If Succeeded returns 0, but If not returns MCIERROR Error Code</returns>
	static MCIERROR Stop(LPCWSTR Alias) {
		return mciSendCommand(mciGetDeviceID(Alias), MCI_STOP, MCI_WAIT, NULL);
	}

	/// <summary>> This Function Closes MCIDevice</summary>
	/// <param name="Alias">- Alias for MCIDevice</param>
	/// <returns>If Succeeded returns 0, but If not returns MCIERROR Error Code</returns>
	static MCIERROR Close(LPCWSTR Alias) {
		return mciSendCommand(mciGetDeviceID(Alias), MCI_CLOSE, MCI_WAIT, NULL);
	}

};

#endif