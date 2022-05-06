<h1>Windows Desktop Application -Template-</h1>

This is Windows Desktop Application Template

<h2>Drop Files Control : Class Name -- "DROP FILES"</h2>

	Pass This Structure To "CreateWindow()" -- [lpParam] Parameter
	
	//////////////////////////////////////////////////////////
	//// +----------------------------------------------+ ////
	//// |                                              | ////
	//// | [in] struct DropFilesStyle - BackgroundColor | ////
	//// | [in] struct DropFilesStyle - ForegroundColor | ////
	//// |                                              | ////
	//// +----------------------------------------------+ ////
	//////////////////////////////////////////////////////////
	
	When File/s Droped : Drop Files Control Post WM_COMMAND Message To Parent Window
	
	///////////////////////////////////////////////////////
	//// +-------------------------------------------+ ////
	//// |                                           | ////
	//// | [out] WPARAM - LOWORD(ID) | HIWORD(HWND)  | ////
	//// | [out] LPARAM - HDROP                      | ////
	//// |                                           | ////
	//// +-------------------------------------------+ ////
	///////////////////////////////////////////////////////

<h2>Animation Stars Control : Class Name -- "ANIMATION STARS"</h2>

	Pass This Structure To "CreateWindow()" -- [lpParam] Parameter
	
	//////////////////////////////////////////////////////
	//// +------------------------------------------+ ////
	//// |                                          | ////
	//// | [in] struct AnimationStyle - SymbolColor | ////
	//// | [in] struct AnimationStyle - Proportion  | ////
	//// | [in] struct AnimationStyle - Symbol      | ////
	//// |                                          | ////
	//// +------------------------------------------+ ////
	//////////////////////////////////////////////////////

<h2>Color Picker Control : Class Name -- "COLOR PICKER"</h2>

	When User Select Color : Color Picker Control Post WM_COMMAND Message To Parent Window
	
	//////////////////////////////////////////////////////
	//// +------------------------------------------+ ////
	//// |                                          | ////
	//// | [out] WPARAM - LOWORD(ID) | HIWORD(HWND) | ////
	//// | [out] LPARAM - Color                     | ////
	//// |                                          | ////
	//// +------------------------------------------+ ////
	//////////////////////////////////////////////////////
