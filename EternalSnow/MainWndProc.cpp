#include "MainWndProc.h"

namespace EternalSnow
{
	LRESULT CALLBACK MainWndProc (_In_ HWND hwnd, _In_ UINT umsg, _In_ WPARAM wparam, _In_ LPARAM lparam)
	{
		switch (umsg)
		{
			//case WM_NCHITTEST:
			//	return HTNOWHERE;

			/*case WM_LBUTTONDOWN:
			{
				MessageBox (NULL, "Hi", "", MB_OK);
				break;
			}*/

			/*case WM_PAINT:
			{
				HDC hdc = GetDC (hwnd);
				RECT rect;
				HBRUSH hbrush;

				rect.top = rect.left = 0;
				rect.bottom = rect.right = 100;
				hbrush = CreateSolidBrush (RGB (100, 0, 100));

				FillRect (hdc, &rect, hbrush);

				DeleteObject (hbrush);
				ReleaseDC (hwnd, hdc);
				break;
			}*/

		/*case WM_PAINT:
		{
			MessageBox (NULL, "Hi", "", MB_OK);
			break;
		}

		case WM_ERASEBKGND:
		{
			MessageBox (NULL, "erase", "", MB_OK);
			break;
		}*/

			default:
				break;
		}

		return DefWindowProc (hwnd, umsg, wparam, lparam);
	}
}