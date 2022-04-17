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
	
	/////////////////////////////////////////////////////
	//// +-----------------------------------------+ ////
	//// |                                         | ////
	//// | [in] struct AnimationStyle - StarColor  | ////
	//// | [in] struct AnimationStyle - Proportion | ////
	//// | [in] struct AnimationStyle - StarSymbol | ////
	//// |                                         | ////
	//// +-----------------------------------------+ ////
	/////////////////////////////////////////////////////

<h2>Calculator Control : Class Name -- "CALCULATOR"</h2>

	When Calculator Destroys : Calculator Control Post WM_COMMAND Message To Parent Window
	
	//////////////////////////////////////////////////////
	//// +------------------------------------------+ ////
	//// |                                          | ////
	//// | [out] WPARAM - LOWORD(ID) | HIWORD(HWND) | ////
	//// | [out] LPARAM - DestroyWindow             | ////
	//// |                                          | ////
	//// +------------------------------------------+ ////
	//////////////////////////////////////////////////////
