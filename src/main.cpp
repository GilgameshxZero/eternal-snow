#include "main.h"

int CALLBACK WinMain(HINSTANCE hinst, HINSTANCE hprevinst, LPSTR cmdLine, int cmdShow) {
    static const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN),
                     SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN),
                     FRAMES_PER_SECOND = 60,
                     MS_PER_FRAME = 1000 / FRAMES_PER_SECOND,
                     TIMER_ID = 1;
    static const LPCSTR CLASS_NAME = "EternalSnow Main Window";

    //Randomize.
    srand(time(NULL));

    WNDCLASSEX wndClass;
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = NULL;
    wndClass.lpfnWndProc = EternalSnow::MainWndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hinst;
    wndClass.hIcon = NULL;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = NULL;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hIconSm = NULL;

    if (!RegisterClassEx(&wndClass)) {
        return -1;
    }

    HWND mainWnd = CreateWindowEx(WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, CLASS_NAME, "", WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hinst, NULL);

    if (!mainWnd) {
        return -1;
    }

    ShowWindow(mainWnd, SW_SHOW);

    //Set black as the transparent color.
    SetLayeredWindowAttributes(mainWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);

    //make sure variables declared here are accessible by procs

    //Set drawing apparatus.
    SetTimer(mainWnd, TIMER_ID, MS_PER_FRAME, EternalSnow::TimerProc);
    EternalSnow::InitDraw(mainWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    EternalSnow::EndDraw(mainWnd);
    return 0;
}