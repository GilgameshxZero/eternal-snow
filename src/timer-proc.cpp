#include "timer-proc.h"

namespace EternalSnow {
HDC hdc, hdcmem /*, hdesktop, desktopmem, mem2*/;
HBITMAP hbmmem /*, desktopbm*/;
HANDLE oldhand /*, olddesktopbm*/;
RECT snowrect, screen;
HBRUSH snowbrush, black, oldbrush;
HPEN snowpen, oldpen;
DWORD last_time;

//We will only have one timer in the window - the fps timer.
VOID CALLBACK TimerProc(_In_ HWND hwnd, _In_ UINT umsg, _In_ UINT_PTR idevent, _In_ DWORD time) {
    static const int fps = 60;
    static const int mpf = 1000 / fps,
                     width = GetSystemMetrics(SM_CXSCREEN),
                     height = GetSystemMetrics(SM_CYSCREEN);
    static int max_snow = (int)width * height / 4000 /*20*/;
    static const double snowrad = 2;
    static int k;
    static double newx, newy, dir, timer, absdir;
    static DWORD finaltime;
    static POINT cursorpos;
    static double snowspeedmult, snowdownmult;

    //Get direction of snow based on cursor.
    GetCursorPos(&cursorpos);
    dir = (double)(cursorpos.x - width / 2) / (width / 2);
    absdir = abs(dir);

    snowdownmult = (double)cursorpos.y / height * 1 + 1;

    //Clear screen.
    FillRect(hdcmem, &screen, black);

    if (snowvec.size() < max_snow) {
        snowvec.push_back(SnowParticle());
        snowvec.back().x = -1;
        snowvec.back().hvel = 0;
    }

    //Draw snow circles all over the screen.
    for (int a = 0; a < snowvec.size(); a++) {
        //Calculate new position, check if out of bounds.
        newx = snowvec[a].x + dir * (int)(time - last_time) * snowvec[a].hvel;
        newy = snowvec[a].y + (int)(time - last_time) * snowvec[a].vvel * snowdownmult;

        if (newy > height || newx < 0 || newx > width) {
            snowspeedmult = (rand() % 2) * 3 + 1;
            snowvec[a].vvel = (double)(rand() % 100 + 50) / 1000 * snowspeedmult;
            snowvec[a].hvel = (double)(rand() % 200 + 100) / 1000 * snowspeedmult;
            k = rand() % (int)((snowvec[a].vvel * width + absdir * snowvec[a].hvel * height) * 10);
            if (k < absdir * snowvec[a].hvel * height * 10) {
                if (dir >= 0)
                    newx = 0;
                else
                    newx = width;
                newy = height * k / (absdir * snowvec[a].hvel * height * 10);
            } else {
                newx = width * (k - absdir * snowvec[a].hvel * height * 10) / (snowvec[a].vvel * width * 10);
                newy = 0;
            }
        }

        snowvec[a].x = newx;
        snowvec[a].y = newy;
        Ellipse(hdcmem, newx - snowrad, newy - snowrad, newx + snowrad, newy + snowrad);
    }

    //Transfer the off-screen DC to the screen.
    BitBlt(hdc, 0, 0, width, height, hdcmem, 0, 0, SRCCOPY);

    //Save the last update time.
    last_time = time;

    //Set timer to fulfill fps.
    finaltime = GetTickCount();
    timer = mpf - ((int)finaltime - (int)time);
    while (timer < 0)
        timer += mpf;
    SetTimer(hwnd, idevent, timer, TimerProc);
}

void InitDraw(HWND hwnd) {
    static const int width = GetSystemMetrics(SM_CXSCREEN),
                     height = GetSystemMetrics(SM_CYSCREEN);
    static const COLORREF snow_color = RGB(200, 200, 230);

    hdc = GetDC(hwnd);
    hdcmem = CreateCompatibleDC(hdc);
    hbmmem = CreateCompatibleBitmap(hdc, width, height);

    oldhand = SelectObject(hdcmem, hbmmem);

    screen.top = 0;
    screen.left = 0;
    screen.bottom = screen.top + height;
    screen.right = screen.left + width;

    snowbrush = CreateSolidBrush(snow_color);
    black = CreateSolidBrush(RGB(255, 255, 255));
    snowpen = CreatePen(PS_SOLID, 0, snow_color);

    oldbrush = (HBRUSH)SelectObject(hdcmem, snowbrush);
    oldpen = (HPEN)SelectObject(hdcmem, snowpen);
}

void EndDraw(HWND hwnd) {
    DeleteObject(snowbrush);
    DeleteObject(snowpen);
    DeleteObject(black);

    SelectObject(hdcmem, oldhand);
    SelectObject(hdcmem, oldbrush);
    SelectObject(hdcmem, oldpen);
    DeleteObject(hbmmem);
    DeleteDC(hdcmem);

    ReleaseDC(hwnd, hdc);
}
}  // namespace EternalSnow