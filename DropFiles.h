#ifndef _DROP_FILES_
#define _DROP_FILES_

#include <ciso646>
#include <Windows.h>

#define MAX_DROP_FILES_CHAR_STRING 256

class DropFiles {

private:

	#pragma region CalculatorStaticMembers
	static HFONT DropFilesFont;
	static BOOL FileDroped;
	#pragma endregion

	#pragma region Functions
	static VOID CreateDropFilesFont();
	static VOID DrawBorder(HDC hdc, RECT &Rectangle, USHORT Width);
	static VOID FillRectOpacity50(HDC hdc, RECT &Rectangle,COLORREF Color);
	static VOID FillArrow(HDC hdc, INT COORD_X, INT COORD_Y, INT WIDTH, INT HEIGHT, COLORREF Color);
	#pragma endregion

	#pragma region Events
	static VOID onCreate(HWND hDropFiles, LPARAM lParam);
	static VOID onPaint(HWND hDropFiles);
	static VOID onDropFiles(HWND hDropFiles, WPARAM wParam);
	#pragma endregion

	#pragma region DropFilesProcedure
	static LRESULT CALLBACK DropFilesProcedure(HWND hDropFiles, UINT Msg, WPARAM wParam, LPARAM lParam);
	#pragma endregion

public:

	typedef struct DropFilesStyle {
		COLORREF BackgroundColor;
		COLORREF ForegroundColor;
	}DFStyle, *LPDFStyle, *LPDropFilesStyle;

	#pragma region InitDropFiles
	static BOOL InitDropFiles();
	#pragma endregion
};

#endif // _DROP_FILES_
