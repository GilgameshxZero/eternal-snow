#include "WinMain.h"

int CALLBACK WinMain (_In_ HINSTANCE hinst, _In_ HINSTANCE hprevinst, _In_ LPSTR cmdline, _In_ int cmdshow)
{
	//const int width = GetSystemMetrics (SM_CXSCREEN),
	//	height = GetSystemMetrics (SM_CYSCREEN);

	//HDC hdesktop = GetDC (NULL);
	//	
	//BitBlt (hdesktop, 0, 0, width, height, hdesktop, 0, 0, NOTSRCCOPY);

	//ReleaseDC (0, hdesktop);

	//Sleep (5000);

	//Randomize.
	srand (time (NULL));

	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof (WNDCLASSEX);
	wndclass.style = NULL;
	wndclass.lpfnWndProc = EternalSnow::MainWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hinst;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "EternalSnowMainWnd";
	wndclass.hIconSm = NULL;

	if (!RegisterClassEx (&wndclass))
	{
		MessageBox (NULL, "RegisterClassEx failed!", "", MB_OK);
		return 0;
	}

	const int width = GetSystemMetrics (SM_CXSCREEN),
		height = GetSystemMetrics (SM_CYSCREEN);
	HWND mwnd = CreateWindowEx (WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, "EternalSnowMainWnd", "", WS_POPUP, 0, 0, width, height, NULL, NULL, hinst, NULL);

	if (!mwnd)
	{
		MessageBox (NULL, "CreateWindowEx failed!", "", MB_OK);
		return 0;
	}

	ShowWindow (mwnd, SW_SHOW);

	//Set black as the transparent color.
	SetLayeredWindowAttributes (mwnd, RGB (255, 255, 255), 0, LWA_COLORKEY);
	//SetLayeredWindowAttributes (mwnd, RGB (255, 255, 255), 0, LWA_ALPHA);

	//Paint whole window black.
	HDC hdc = GetDC (mwnd);
	RECT rect;
	HBRUSH hbrush;

	rect.top = rect.left = 0;
	rect.right = width;
	rect.bottom = height;
	hbrush = CreateSolidBrush (RGB (255, 255, 255));

	FillRect (hdc, &rect, hbrush);

	DeleteObject (hbrush);
	ReleaseDC (mwnd, hdc);

	//Set drawing apparatus.
	SetTimer (mwnd, 1, USER_TIMER_MINIMUM, EternalSnow::TimerProc);
	EternalSnow::InitDraw (mwnd);

	//SetLayeredWindowAttributes (mwnd, RGB (0, 0, 0), 0, LWA_ALPHA);

	//long style = GetWindowLong (mwnd, GWL_STYLE);
	//style &= ~(WS_VISIBLE);    // this works - window become invisible 

	//style |= WS_EX_TOOLWINDOW;   // flags don't work - windows remains in taskbar
	//style &= ~(WS_EX_APPWINDOW);

	//ShowWindow (mwnd, SW_HIDE); // hide the window
	//SetWindowLong (mwnd, GWL_STYLE, style); // set the style
	//ShowWindow (mwnd, SW_SHOW); // show the window for the new style to come into effect
	//ShowWindow (mwnd, SW_HIDE); // hide the window so we can't see it

	//ReleaseCapture ();
	//SetCapture ((HWND)GetDC (0));

	//BitBlt (GetDC (mwnd), 0, 0, width, height - 200, GetDC (mwnd), 0, 0, NOTSRCCOPY);

	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	EternalSnow::EndDraw (mwnd);
		
	return 0;
}