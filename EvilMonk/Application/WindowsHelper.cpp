#include "WindowsHelper.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

void centerWindow(sf::WindowHandle handle)
{
#ifdef _WIN32
	HWND hWnd = handle;

	RECT rect;
	GetClientRect(hWnd, &rect);

	LONG style = GetWindowLong(hWnd, GWL_STYLE);

	int left = (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2;
	int top = (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2;

	if (!(style & WS_POPUP))
	{

		left -= GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
		top -= GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXPADDEDBORDER);
	}

	SetWindowPos(hWnd, NULL, left, top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	SetWindowLong(hWnd, GWL_STYLE, style & ~WS_POPUP);
#endif
}
