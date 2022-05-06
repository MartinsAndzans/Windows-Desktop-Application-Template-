<h1>Windows Desktop Application -Template-</h1>

This is Windows Desktop Application Template

<h2>Drop Files Control : Class Name -- "DROP FILES"</h2>

<table>
	<tr><th>Pass This Structure To "CreateWindow()" -- [lpParam] Parameter</th></tr>
	<tr><th></th></tr>
	<tr><th>[in] struct DropFilesStyle - BackgroundColor</th></tr>
	<tr><th>[in] struct DropFilesStyle - ForegroundColor</th></tr>
	<tr><th></th></tr>
	<tr><th>When File/s Droped : Drop Files Control Post WM_COMMAND Message To Parent Window</th></tr>
	<tr><th></th></tr>
	<tr><th>[out] WPARAM - LOWORD(ID) | HIWORD(HWND)</th></tr>
	<tr><th>[out] LPARAM - HDROP</th></tr>
</table>

<h2>Animation Stars Control : Class Name -- "ANIMATION STARS"</h2>

<table>
	<tr><th>Pass This Structure To "CreateWindow()" -- [lpParam] Parameter</th></tr>
	<tr><th></th></tr>
	<tr><th>[in] struct AnimationStyle - SymbolColor</th></tr>
	<tr><th>[in] struct AnimationStyle - Proportion</th></tr>
	<tr><th>[in] struct AnimationStyle - Symbol</th></tr>
</table>

<h2>Color Picker Control : Class Name -- "COLOR PICKER"</h2>

<table>
	<tr><th>When User Select Color : Color Picker Control Post WM_COMMAND Message To Parent Window</th></tr>
	<tr><th></th></tr>
	<tr><th>[out] WPARAM - LOWORD(ID) | HIWORD(HWND)</th></tr>
	<tr><th>out] LPARAM - Color</th></tr>
</table>
