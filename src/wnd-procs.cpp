#include "wnd-procs.h"

namespace EternalSnow {
HDC hDC, hDCMem;
HBITMAP hBMMem;
HANDLE oldHDC;
HPEN oldPen, snowPen;
RECT screen;
HBRUSH snowBrush, blackBrush, oldBrush;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        default:
            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR eventId, DWORD time) {
    static const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN),
                     SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN),
                     SNOW_LIMIT = SCREEN_WIDTH * SCREEN_HEIGHT / 4000,
                     FRAMES_PER_SECOND = 60,
                     MS_PER_FRAME = 1000 / FRAMES_PER_SECOND;
    static const double SNOW_RADIUS = 2;

    static double direction, absDirection, downSpeedMod;

    //Get direction of snow based on cursor.
    {
        static POINT cursorPos;

        GetCursorPos(&cursorPos);
        direction = (double)(cursorPos.x - SCREEN_WIDTH / 2) / (SCREEN_WIDTH / 2);
        absDirection = abs(direction);

        downSpeedMod = (double)cursorPos.y / SCREEN_HEIGHT + 1;
    }

    //Clear screen.
    FillRect(hDCMem, &screen, blackBrush);

    //add more snow if there aren't enough
    if (snowParts.size() < SNOW_LIMIT) {
        snowParts.push_back(SnowParticle());
        snowParts.back().x = -1;
        snowParts.back().horiVel = 0;
    }

    //Draw snow circles all over the screen.
    for (int a = 0; a < snowParts.size(); a++) {
        //Calculate new position, check if out of bounds.
        snowParts[a].x += direction * MS_PER_FRAME * snowParts[a].horiVel;
        snowParts[a].y += MS_PER_FRAME * snowParts[a].vertVel * downSpeedMod;

        //snow is off screen, put it somewhere again
        if (snowParts[a].y > SCREEN_HEIGHT || snowParts[a].x < 0 || snowParts[a].x > SCREEN_WIDTH) {
            //used to initialize snow position
            static int k;
            static double snowSpeedMod;

            snowSpeedMod = (rand() % 2) * 3 + 1;
            snowParts[a].vertVel = (double)(rand() % 100 + 50) / 1000 * snowSpeedMod;
            snowParts[a].horiVel = (double)(rand() % 200 + 100) / 1000 * snowSpeedMod;

            k = rand() % (int)((snowParts[a].vertVel * SCREEN_WIDTH + absDirection * snowParts[a].horiVel * SCREEN_HEIGHT) * 10);
            if (k < absDirection * snowParts[a].horiVel * SCREEN_HEIGHT * 10) {
                if (direction >= 0)
                    snowParts[a].x = 0;
                else
                    snowParts[a].x = SCREEN_WIDTH;
                snowParts[a].y = SCREEN_HEIGHT * k / (absDirection * snowParts[a].horiVel * SCREEN_HEIGHT * 10);
            } else {
                snowParts[a].x = SCREEN_WIDTH * (k - absDirection * snowParts[a].horiVel * SCREEN_HEIGHT * 10) / (snowParts[a].vertVel * SCREEN_WIDTH * 10);
                snowParts[a].y = 0;
            }
        }

        Ellipse(hDCMem, snowParts[a].x - SNOW_RADIUS, snowParts[a].y - SNOW_RADIUS, snowParts[a].x + SNOW_RADIUS, snowParts[a].y + SNOW_RADIUS);
    }

    //Transfer the off-screen DC to the screen.
    BitBlt(hDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hDCMem, 0, 0, SRCCOPY);
}

void InitDraw(HWND hWnd) {
    static const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN),
                     SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
    static const COLORREF SNOW_COLOR = RGB(200, 200, 230);

    hDC = GetDC(hWnd);
    hDCMem = CreateCompatibleDC(hDC);
    hBMMem = CreateCompatibleBitmap(hDC, SCREEN_WIDTH, SCREEN_HEIGHT);

    oldHDC = SelectObject(hDCMem, hBMMem);

    screen.top = 0;
    screen.left = 0;
    screen.bottom = screen.top + SCREEN_HEIGHT;
    screen.right = screen.left + SCREEN_WIDTH;

    snowBrush = CreateSolidBrush(SNOW_COLOR);
    blackBrush = CreateSolidBrush(RGB(255, 255, 255));
    snowPen = CreatePen(PS_SOLID, 1, SNOW_COLOR);

    oldBrush = static_cast<HBRUSH>(SelectObject(hDCMem, snowBrush));
    oldPen = static_cast<HPEN>(SelectObject(hDCMem, snowPen));
}

void EndDraw(HWND hWnd) {
    DeleteObject(snowBrush);
    DeleteObject(blackBrush);
    DeleteObject(snowPen);

    SelectObject(hDCMem, oldPen);
    SelectObject(hDCMem, oldHDC);
    SelectObject(hDCMem, oldBrush);
    DeleteObject(hBMMem);
    DeleteDC(hDCMem);

    ReleaseDC(hWnd, hDC);
}
}  // namespace EternalSnow