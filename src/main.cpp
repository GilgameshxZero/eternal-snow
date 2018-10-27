#include "main.h"

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
    EternalSnow::UserData userData;

    userData.SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    userData.SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
    userData.FRAMES_PER_SECOND = 60;
    userData.MS_PER_FRAME = 1000 / userData.FRAMES_PER_SECOND;
    userData.SNOW_LIMIT = userData.SCREEN_WIDTH * userData.SCREEN_HEIGHT / 4000;
    userData.SNOW_RADIUS = 2;
    userData.TIMER_ID = 1;
    userData.CLASS_NAME = "EternalSnow Main Window";

    //Randomize.
    srand(time(NULL));

    WNDCLASSEX wndClass;
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = NULL;
    wndClass.lpfnWndProc = EternalSnow::MainWndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInst;
    wndClass.hIcon = NULL;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = NULL;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = userData.CLASS_NAME.c_str();
    wndClass.hIconSm = NULL;

    if (!RegisterClassEx(&wndClass)) {
        return -1;
    }

    HWND mainWnd = CreateWindowEx(WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, userData.CLASS_NAME.c_str(), "", WS_POPUP, 0, 0, userData.SCREEN_WIDTH, userData.SCREEN_HEIGHT, NULL, NULL, hInst, NULL);

    if (!mainWnd) {
        return -1;
    }

    ShowWindow(mainWnd, SW_SHOW);

    //Set black as the transparent color.
    SetLayeredWindowAttributes(mainWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);

    //make sure variables declared here are accessible by procs
    SetWindowLongPtr(mainWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&userData));

    //Set drawing apparatus.
    SetTimer(mainWnd, userData.TIMER_ID, userData.MS_PER_FRAME, EternalSnow::TimerProc);
    EternalSnow::InitDraw(mainWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    EternalSnow::EndDraw(mainWnd);
    return 0;
}