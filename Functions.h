#ifndef _FUNCTIONS_
#define _FUNCTIONS_

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#include <ciso646>
#include <Windows.h>
#include <string>
#include <fstream>

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

		if (Value < 0) {
			Minus = TRUE;
			Value *= -1; // -Value *= -1
		}

		do {

			CHAR Character = static_cast<CHAR>(Value % 10 + ASCI_VALUE_ZERO); // Get Last Number - Char Value 0 / 48 - 9 / 57
			Value = Value / 10; // Remove Last Number
			StringValue.insert(0, 1, Character); // Add Character To Begining of StringValue

		} while (Value != 0);
		
		if (Minus) {
			StringValue.insert(0, "-");
		}

		if (BufferSize >= StringValue.length() + 1) {
			strcpy_s(Buffer, BufferSize, StringValue.c_str());
		} else {
			OutputDebugString(L"\'[Functions::_itoa] - Buffer To Small\'\r\n");
			return FALSE;
		}

		return TRUE;

	}

	/// <summary>
	/// This Function Encrypt Text To ASCII Value Code
	/// </summary>
	/// <param name="TextToBeEncrypted">- Text To Be Encrypted</param>
	/// <returns>Encrypted Text</returns>
	static std::string EncryptText(const std::string &TextToBeEncrypted) {

		std::string EncryptedText{};

		for (CHAR CHARACTER : TextToBeEncrypted) {
			SHORT ASCII_VALUE = static_cast<SHORT>(CHARACTER);
			EncryptedText += std::to_string(ASCII_VALUE) + ":";
		}

		return EncryptedText;

	}

	/// <summary>
	/// This Function Encrypt Unicode Text To ASCII Value Code
	/// </summary>
	/// <param name="UTextToBeEncrypted">- Unicode Text To Be Encrypted</param>
	/// <returns>Encrypted Unicode Text</returns>
	static std::wstring EncryptText(const std::wstring &UTextToBeEncrypted) {

		std::wstring EncryptedUText{};

		for (WCHAR UCHARACTER : UTextToBeEncrypted) {
			SHORT ASCII_VALUE = static_cast<SHORT>(UCHARACTER);
			EncryptedUText += std::to_wstring(ASCII_VALUE) + L":";
		}

		return EncryptedUText;

	}

	/// <summary>
	/// This Function Decrypt Text From ASCII Value Code
	/// </summary>
	/// <param name="EncryptedText">- Text To Be Decrypted</param>
	/// <returns>Decrypted Text</returns>
	static std::string DecryptText(std::string EncryptedText) {

		std::string DecryptedText{};

		while (EncryptedText.length() != 0) {
			if (EncryptedText.ends_with(':')) {
				CHAR CHARACTER = static_cast<CHAR>(std::stoi(EncryptedText));
				DecryptedText += CHARACTER;
				EncryptedText.replace(0, EncryptedText.find(':') + 1, "");
			} else {
				return "Cannot Decrypt Text because Text is Corrupted!";
			}
		}

		return DecryptedText;

	}

	/// <summary>
	/// This Function Decrypt Unicode Text From ASCII Value Code
	/// </summary>
	/// <param name="UEncryptedText">- Unicode Text To Be Decrypted</param>
	/// <returns>Decrypted Unicode Text</returns>
	static std::wstring DecryptText(std::wstring UEncryptedText) {

		std::wstring UDecryptedText{};

		while (UEncryptedText.length() != 0) {
			if (UEncryptedText.ends_with(L':')) {
				WCHAR UCHARACTER = static_cast<WCHAR>(std::stoi(UEncryptedText));
				UDecryptedText += UCHARACTER;
				UEncryptedText.replace(0, UEncryptedText.find(L':') + 1, L"");
			}
			else {
				return L"Cannot Decrypt Unicode Text because Unicode Text is Corrupted!";
			}
		}

		return UDecryptedText;

	}

	/// <summary>
	/// This Function Finds Character Position Into String
	/// </summary>
	/// <param name="Text">- Text</param>
	/// <param name="Char">- Character To Find</param>
	/// <param name="TextLength">- Text Length In Characters</param>
	/// <returns>
	/// If Character has been found Function Returns Character Position,
	/// <para>but If Character not found Function Returns MAXSIZE_T Value</para>
	/// </returns>
	static SIZE_T FindChar(LPCSTR Text, CONST CHAR Char, SIZE_T TextLength) {
		
		for (SIZE_T I = 0; I < TextLength; I++) {
			if (Text[I] == Char) {
				return I;
			}
		}

		return static_cast<SIZE_T>(-1);

	}

	/// <summary>
	/// This Function Finds Unicode Character Position Into Unicode String
	/// </summary>
	/// <param name="UText">- Unicode Text</param>
	/// <param name="UChar">- Unicode Character To Find</param>
	/// <param name="UTextLength">- Unicode Text Length In Characters</param>
	/// <returns>
	/// If Unicode Character has been found Function Returns Unicode Character Position,
	/// <para>but If Character not found Function Returns MAXSIZE_T Value</para>
	/// </returns>
	static SIZE_T FindChar(LPCWSTR UText, CONST WCHAR UChar, SIZE_T UTextLength) {

		for (SIZE_T I = 0; I < UTextLength; I++) {
			if (UText[I] == UChar) {
				return I;
			}
		}

		return static_cast<SIZE_T>(-1);

	}

	/// <summary>
	/// This Function Rounds Double String
	/// </summary>
	/// <param name="DoubleString">- Double String</param>
	/// <returns>Rounded Double String</returns>
	static std::string RoundDoubleString(std::string DoubleString) {

		if (DoubleString.find('.') != std::string::npos) {

			while (DoubleString.ends_with('0') or DoubleString.ends_with('.')) {

				if (DoubleString.ends_with('.')) {
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
	/// This Function Copy Text To Clipboard
	/// </summary>
	/// <param name="NewClipboardOwner">- New Clipboard Owner</param>
	/// <param name="Text">- Text To Clipboard</param>
	/// <param name="TextLength">- Text Length In Characters Without [null character] - [\0]</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL CopyTextToClipboard(HWND NewClipboardOwner, LPCSTR Text, SIZE_T TextLength) {

		if (TextLength != 0) {
			if (OpenClipboard(NewClipboardOwner)) {
				EmptyClipboard();
				HLOCAL CopyData = LocalAlloc(LPTR | LMEM_VALID_FLAGS, sizeof(CHAR) * (TextLength + 1));
				if (CopyData == NULL) {
					CloseClipboard();
					return FALSE;
				}
				void *CopyDataPtr = LocalLock(CopyData);
				if (CopyDataPtr == nullptr) {
					LocalFree(CopyData);
					CloseClipboard();
					return FALSE;
				}
				memcpy(CopyDataPtr, static_cast<const void*>(Text), sizeof(CHAR) * (TextLength + 1));
				SetClipboardData(CF_TEXT, CopyData);
				LocalUnlock(CopyData);
				LocalFree(CopyData);
				CloseClipboard();
				return TRUE;
			}
		}

		return FALSE;

	}

	/// <summary>
	/// This Function Copy Unicode Text To Clipboard
	/// </summary>
	/// <param name="NewClipboardOwner">- New Clipboard Owner</param>
	/// <param name="UText">- Unicode Text To Clipboard</param>
	/// <param name="UTextLength">- Unicode Text Length In Characters Without [null character] - [\0]</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL CopyTextToClipboard(HWND NewClipboardOwner, LPCWSTR UText, SIZE_T UTextLength) {

		if (UTextLength != 0) {
			if (OpenClipboard(NewClipboardOwner)) {
				EmptyClipboard();
				HLOCAL CopyData = LocalAlloc(LPTR | LMEM_VALID_FLAGS, sizeof(WCHAR) * (UTextLength + 1));
				if (CopyData == NULL) {
					CloseClipboard();
					return FALSE;
				}
				void *CopyDataPtr = LocalLock(CopyData);
				if (CopyDataPtr == nullptr) {
					LocalFree(CopyData);
					CloseClipboard();
					return FALSE;
				}
				memcpy(CopyDataPtr, static_cast<const void*>(UText), sizeof(WCHAR) * (UTextLength + 1));
				SetClipboardData(CF_UNICODETEXT, CopyData);
				LocalUnlock(CopyData);
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
	/// <param name="NewClipboardOwner">- New Clipboard Owner</param>
	/// <param name="Buffer">- Text From Clipboard</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL GetTextFromClipboard(HWND NewClipboardOwner, std::string &Buffer) {

		if (OpenClipboard(NewClipboardOwner)) {
			HLOCAL ClipboardData = GetClipboardData(CF_TEXT);
			if (ClipboardData == NULL) {
				CloseClipboard();
				return FALSE;
			}
			void *ClipboardDataPtr = LocalLock(ClipboardData);
			if (ClipboardDataPtr == nullptr) {
				LocalFree(ClipboardData);
				CloseClipboard();
				return FALSE;
			}
			Buffer += static_cast<char*>(ClipboardDataPtr);
			LocalUnlock(ClipboardData);
			LocalFree(ClipboardData);
			CloseClipboard();
			return TRUE;
		}

		return FALSE;

	}

	/// <summary>
	/// This Function Gets Unicode Text From Clipboard
	/// </summary>
	/// <param name="NewClipboardOwner">- New Clipboard Owner</param>
	/// <param name="UBuffer">- Unicode Text From Clipboard</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL GetTextFromClipboard(HWND NewClipboardOwner, std::wstring &UBuffer) {

		if (OpenClipboard(NewClipboardOwner)) {
			HLOCAL ClipboardData = GetClipboardData(CF_UNICODETEXT);
			if (ClipboardData == NULL) {
				CloseClipboard();
				return FALSE;
			}
			void *ClipboardDataPtr = LocalLock(ClipboardData);
			if (ClipboardDataPtr == nullptr) {
				LocalFree(ClipboardData);
				CloseClipboard();
				return FALSE;
			}
			UBuffer += static_cast<wchar_t*>(ClipboardDataPtr);
			LocalUnlock(ClipboardData);
			LocalFree(ClipboardData);
			CloseClipboard();
			return TRUE;
		}

		return FALSE;

	}

	/// <summary>
	/// Shows Error Message
	/// </summary>
	/// <param name="ParentWindow">- Parent Window</param>
	/// <param name="AdditionalErroMessage">- Additional Error Message</param>
	static VOID ShowLastError(HWND ParentWindow = HWND_DESKTOP, std::string AdditionalErrorMessage = " - Error") {
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
	static BOOL SaveBitmapToFile(HBITMAP Bitmap, LPCSTR FilePath, SIZE &BitmapSize) {

		std::ofstream image;

		CONST WORD BM = 0x4D42; // ASCII 'B' = 66 / 0x42 | 'M' = 77 / 0x4D
		CONST DWORD BitmapSizeCXxCY = BitmapSize.cx * BitmapSize.cy; // Bitmap Size [cx * cy]

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

		BITMAPINFO bminfo = { 0 };
		bminfo.bmiHeader = bmiheader;

		HDC ScreenDC = GetDC(HWND_DESKTOP);
		GetDIBits(ScreenDC, Bitmap, 0, BitmapSize.cy, BitmapBytes.get(), &bminfo, DIB_RGB_COLORS);
		ReleaseDC(HWND_DESKTOP, ScreenDC);
		
		image.open(FilePath, std::ios::binary); // Open File

		if (image.is_open()) {

			image.write(reinterpret_cast<char*>(&bmfheader), sizeof(BITMAPFILEHEADER)); // BITMAP FILE HEADER
			image.write(reinterpret_cast<char*>(&bmiheader), sizeof(BITMAPINFOHEADER)); // BITMAP INFO HEADER
			image.write(reinterpret_cast<char*>(BitmapBytes.get()), sizeof(COLORREF) * BitmapSizeCXxCY); // COLOR BYTE ARRAY

			image.close(); // Close File

			return TRUE;

		}

		return FALSE;

	}

};

class Sound {

public:

	typedef DWORD MCISTATUSCODE; // * MCISTATUSCODE [DWORD] is a 32-bit unsigned integer *
	typedef DWORD MCISTATUS; // * MCISTATUS [DWORD] is a 32-bit unsigned integer *

	/// <summary>
	/// MCI Error Codes
	/// </summary>
	enum MCI {
		MCI_OK = 0 // MCI Success Code
	};

	/// <summary>
	/// This Function Opens MCIDevice
	/// <para>Music File Path "*.wav" / "*.wma" / "*.mp3" || Video Formats not Supported Yet ||</para>
	/// </summary>
	static MCIERROR Open(LPCWSTR Alias, LPCWSTR FilePath) {

		MCI_OPEN_PARMS open = { 0 };
		open.lpstrAlias = Alias;
		open.lpstrElementName = FilePath;

		return mciSendCommand(NULL, MCI_OPEN, MCI_WAIT | MCI_OPEN_ELEMENT | MCI_OPEN_ALIAS, (DWORD_PTR)&open);

	}

	/// <summary>
	/// This Function Obtains Playback Status
	/// <para>MCI_STATUS_POSITION - Obtains Current Playback Position</para>
	/// <para>MCI_STATUS_LENGTH - Obtains Total Media Length</para>
	/// <para>MCI_STATUS_MODE - Obtains Current Mode of The Device</para>
	/// <para>MCI_STATUS_TIME_FORMAT - Obtains Current Time Format of The Device</para>
	/// </summary>
	static MCIERROR GetPlaybackStatus(LPCWSTR Alias, MCISTATUSCODE StatusCode, MCISTATUS *PlaybackStatus) {

		MCI_STATUS_PARMS status = { 0 };
		status.dwItem = StatusCode; // Status Code
		status.dwReturn = NULL; // Return

		MCIERROR Error = mciSendCommand(mciGetDeviceID(Alias), MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD_PTR)&status);
		*PlaybackStatus = static_cast<MCISTATUS>(status.dwReturn);
		return Error;

	}

	/// <summary>
	/// This Function Plays Music From The Beginning
	/// <para>If Notify is TRUE MCI Sends To Callback Window [MM_MCINOTIFY] Message 'LOWORD(lParam) - MCIDeviceID'</para>
	/// </summary>
	static MCIERROR Play(HWND CallbackWindow, LPCWSTR Alias, BOOL Notify = FALSE) {

		MCI_PLAY_PARMS play = { 0 };
		play.dwCallback = reinterpret_cast<DWORD_PTR>(CallbackWindow);
		play.dwFrom = 0;
		
		if (Notify) {
			return mciSendCommand(mciGetDeviceID(Alias), MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD_PTR)&play);
		} else {
			return mciSendCommand(mciGetDeviceID(Alias), MCI_PLAY, MCI_FROM, (DWORD_PTR)&play);
		}

	}

	/// <summary>
	/// This Function Seek To Specified Point of Playback
	/// <para>MCI_SEEK_TO_START - Seek To Beginning of Playback</para>
	/// <para>MCI_SEEK_TO_END - Seek To End of Playback</para>
	/// </summary>
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

	/// <summary>
	/// This Function Pauses Playbeck
	/// </summary>
	static MCIERROR Pause(LPCWSTR Alias) {
		return mciSendCommand(mciGetDeviceID(Alias), MCI_PAUSE, MCI_WAIT, NULL);
	}

	/// <summary>
	/// This Function Resumes Playbeck
	/// </summary>
	static MCIERROR Resume(LPCWSTR Alias) {
		return mciSendCommand(mciGetDeviceID(Alias), MCI_RESUME, MCI_WAIT, NULL);
	}

	/// <summary>
	/// This Function Stops Playbeck
	/// </summary>
	static MCIERROR Stop(LPCWSTR Alias) {
		return mciSendCommand(mciGetDeviceID(Alias), MCI_STOP, MCI_WAIT, NULL);
	}

	/// <summary>
	/// This Function Closes MCIDevice
	/// </summary>
	static MCIERROR Close(LPCWSTR Alias) {
		return mciSendCommand(mciGetDeviceID(Alias), MCI_CLOSE, MCI_WAIT, NULL);
	}

};

#endif // _FUNCTIONS_
