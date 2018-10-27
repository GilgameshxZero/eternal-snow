#include "main.h"

int CALLBACK WinMain (_In_ HINSTANCE hinst, _In_ HINSTANCE hprevinst, _In_ LPSTR cmdline, _In_ int cmdshow)
{
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

	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	EternalSnow::EndDraw (mwnd);
		
	return 0;
}