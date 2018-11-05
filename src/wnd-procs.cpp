#include "wnd-procs.h"

namespace EternalSnow {
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    UserData &ud = *reinterpret_cast<UserData *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (uMsg) {
        case WM_APP:
            //tray message, should not be called until ud is valid
            switch (lParam) {
                case WM_LBUTTONUP:
                    if (!ud.paused) {
                        //pause the timer
                        KillTimer(hWnd, ud.TIMER_ID);

                        FillRect(ud.hDCMem, &ud.screenRect, ud.blackBrush);
                        static POINT zeroPt = {0, 0};
                        static SIZE fullSz = {ud.SCREEN_WIDTH, ud.SCREEN_HEIGHT};
                        UpdateLayeredWindow(hWnd, ud.hDC, &zeroPt, &fullSz, ud.hDCMem, &zeroPt, ud.BLACK_COLOR, NULL, ULW_COLORKEY);

                        UnhookWindowsHookEx(ud.hMouseLLHook);

                        ud.paused = true;
                    } else {
                        //resume
                        SetTimer(hWnd, ud.TIMER_ID, ud.MS_PER_FRAME, EternalSnow::TimerProc);

                        GetCursorPos(&ud.mousePos);
                        ud.hMouseLLHook = SetWindowsHookEx(WH_MOUSE_LL, EternalSnow::MouseLLHookProc, NULL, NULL);
    
                        ud.paused = false;
                    }
                    break;
                case WM_RBUTTONUP:
                    //exit the program
                    PostQuitMessage(0);
                    break;
            }
            break;

        default:
            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR eventId, DWORD time) {
    static UserData &ud = *reinterpret_cast<UserData *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    static double direction, absDirection, vertVelMod;

    //Get direction of snow based on cursor.
    //todo: update mouse position only when mouse moved
    direction = (double)(ud.mousePos.x - ud.SCREEN_WIDTH / 2) / (ud.SCREEN_WIDTH / 2);
    absDirection = abs(direction);

    vertVelMod = (double)ud.mousePos.y / ud.SCREEN_HEIGHT + 1;

    //Clear screen.
    FillRect(ud.hDCMem, &ud.screenRect, ud.blackBrush);

    //add more snow if there aren't enough
    if (ud.snowParts.size() < ud.SNOW_LIMIT) {
        ud.snowParts.push_back(SnowParticle());
        ud.snowParts.back().x = -1;
        ud.snowParts.back().horiVel = 0;
    }

    //Draw snow circles all over the screen.
    for (int a = 0; a < ud.snowParts.size(); a++) {
        //Calculate new position, check if out of bounds.
        ud.snowParts[a].x += ud.MS_PER_FRAME * ud.snowParts[a].horiVel * direction;
        ud.snowParts[a].y += ud.MS_PER_FRAME * ud.snowParts[a].vertVel * vertVelMod;

        //snow is off screen or new, put it somewhere again
        if (ud.snowParts[a].y < 0 || ud.snowParts[a].y > ud.SCREEN_HEIGHT || ud.snowParts[a].x < 0 || ud.snowParts[a].x > ud.SCREEN_WIDTH) {
            //used to initialize snow position
            static int k;
            static double snowSpeedMod;

            snowSpeedMod = (rand() % 2) * ud.SNOW_SPEED_FAST + 1;
            ud.snowParts[a].vertVel = (double)(rand() % 100 + 50) / 1000 * snowSpeedMod;
            ud.snowParts[a].horiVel = (double)(rand() % 200 + 100) / 1000 * snowSpeedMod;

            k = rand() % (int)((ud.snowParts[a].vertVel * ud.SCREEN_WIDTH + absDirection * ud.snowParts[a].horiVel * ud.SCREEN_HEIGHT) * 10);
            if (k < absDirection * ud.snowParts[a].horiVel * ud.SCREEN_HEIGHT * 10) {
                if (direction >= 0)
                    ud.snowParts[a].x = 0;
                else
                    ud.snowParts[a].x = ud.SCREEN_WIDTH;
                ud.snowParts[a].y = ud.SCREEN_HEIGHT * k / (absDirection * ud.snowParts[a].horiVel * ud.SCREEN_HEIGHT * 10);
            } else {
                ud.snowParts[a].x = ud.SCREEN_WIDTH * (k - absDirection * ud.snowParts[a].horiVel * ud.SCREEN_HEIGHT * 10) / (ud.snowParts[a].vertVel * ud.SCREEN_WIDTH * 10);
                ud.snowParts[a].y = 0;
            }
        }

        Ellipse(ud.hDCMem, ud.snowParts[a].x - ud.SNOW_RADIUS, ud.snowParts[a].y - ud.SNOW_RADIUS, ud.snowParts[a].x + ud.SNOW_RADIUS, ud.snowParts[a].y + ud.SNOW_RADIUS);
    }

    //Transfer the off-screen DC to the screen.
    static POINT zeroPt = {0, 0};
    static SIZE fullSz = {ud.SCREEN_WIDTH, ud.SCREEN_HEIGHT};
    UpdateLayeredWindow(hWnd, ud.hDC, &zeroPt, &fullSz, ud.hDCMem, &zeroPt, ud.BLACK_COLOR, NULL, ULW_COLORKEY);
}
LRESULT CALLBACK MouseLLHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0)
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    if (wParam != WM_MOUSEMOVE)
        return CallNextHookEx(NULL, nCode, wParam, lParam);

    pud->mousePos = reinterpret_cast<MSLLHOOKSTRUCT *>(lParam)->pt;

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
}  // namespace EternalSnow