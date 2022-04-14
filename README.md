# Windows Desktop Application -Template-
-Template-

Thi is Windows Desktop Application Template

# Drop Files Control : Class Name -- -"DROP FILES"-

	////////////////////////////////////////////////////////////
	//// +------------------------------------------------+ ////
	//// |                                                | ////
	//// | [in] struct LpDropFilesStyle - BackgroundColor | ////
	//// | [in] struct LpDropFilesStyle - ForegroundColor | ////
	//// |                                                | ////
	//// +------------------------------------------------+ ////
	////////////////////////////////////////////////////////////
	
	When File/s Droped : Drop Files Control Post WM_COMMAND Message
	
	///////////////////////////////////////////////////////
	//// +-------------------------------------------+ ////
	//// |                                           | ////
	//// | [out] WPARAM - LOWORD(ID) | HIWORD(HWND)  | ////
	//// | [out] LPARAM - HDROP                      | ////
	//// |                                           | ////
	//// +-------------------------------------------+ ////
	///////////////////////////////////////////////////////
