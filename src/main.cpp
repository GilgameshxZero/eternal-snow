#include "main.h"

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
    EternalSnow::UserData ud;

    ud.CONFIG_LOC = "../config/eternal-snow.cfg";

    std::map<std::string, std::string> config = Rain::readParameterFile(ud.CONFIG_LOC);
    ud.FRAMES_PER_SECOND = Rain::strToT<int>(config["frames-per-second"]);
    ud.SNOW_LIMIT_SCALING = Rain::strToT<int>(config["snow-limit-scaling"]);
    ud.SNOW_RADIUS = Rain::strToT<int>(config["snow-radius"]);
    ud.SNOW_COLOR_HEX = config["snow-color-hex"];

    ud.SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    ud.SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
    ud.MS_PER_FRAME = 1000 / ud.FRAMES_PER_SECOND;
    ud.SNOW_LIMIT = ud.SCREEN_WIDTH * ud.SCREEN_HEIGHT / 4000 * ud.SNOW_LIMIT_SCALING;
    ud.TIMER_ID = 1;
    ud.CLASS_NAME = "EternalSnow Main Window";
    ud.SNOW_COLOR = Rain::colorFromHex(ud.SNOW_COLOR_HEX);
    ud.BLACK_COLOR = RGB(0, 0, 1);

    //Randomize.
    srand(time(NULL));

    {
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
        wndClass.lpszClassName = ud.CLASS_NAME.c_str();
        wndClass.hIconSm = NULL;

        if (!RegisterClassEx(&wndClass)) {
            return -1;
        }
    }

    HWND mainWnd = CreateWindowEx(WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, ud.CLASS_NAME.c_str(), "", WS_POPUP, 0, 0, ud.SCREEN_WIDTH, ud.SCREEN_HEIGHT, NULL, NULL, hInst, NULL);

    if (!mainWnd) {
        return -1;
    }

    ShowWindow(mainWnd, SW_SHOW);

    //Set black as the transparent color.
    SetLayeredWindowAttributes(mainWnd, ud.BLACK_COLOR, 0, LWA_COLORKEY);

    //make sure variables declared here are accessible by procs
    SetWindowLongPtr(mainWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&ud));

    //setup timer
    SetTimer(mainWnd, ud.TIMER_ID, ud.MS_PER_FRAME, EternalSnow::TimerProc);
    
    //setup drawing-related handles
    ud.hDC = GetDC(mainWnd);
    ud.hDCMem = CreateCompatibleDC(ud.hDC);
    ud.hBMMem = CreateCompatibleBitmap(ud.hDC, ud.SCREEN_WIDTH, ud.SCREEN_HEIGHT);

    ud.oldHBM = static_cast<HBITMAP>(SelectObject(ud.hDCMem, ud.hBMMem));

    ud.screenRect.top = ud.screenRect.left = 0;
    ud.screenRect.bottom = ud.SCREEN_HEIGHT;
    ud.screenRect.right = ud.SCREEN_WIDTH;

    ud.snowBrush = CreateSolidBrush(ud.SNOW_COLOR);
    ud.blackBrush = CreateSolidBrush(ud.BLACK_COLOR);
    ud.snowPen = CreatePen(PS_SOLID, 1, ud.SNOW_COLOR);

    ud.oldBrush = static_cast<HBRUSH>(SelectObject(ud.hDCMem, ud.snowBrush));
    ud.oldPen = static_cast<HPEN>(SelectObject(ud.hDCMem, ud.snowPen));

    {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //free all objects
    DeleteObject(ud.snowBrush);
    DeleteObject(ud.blackBrush);
    DeleteObject(ud.snowPen);

    SelectObject(ud.hDCMem, ud.oldPen);
    SelectObject(ud.hDCMem, ud.oldHBM);
    SelectObject(ud.hDCMem, ud.oldBrush);
    DeleteObject(ud.hBMMem);
    DeleteDC(ud.hDCMem);

    ReleaseDC(mainWnd, ud.hDC);
    return 0;
}