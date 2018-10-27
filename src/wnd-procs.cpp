#include "wnd-procs.h"

namespace EternalSnow {
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
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
    {
        static POINT cursorPos;

        GetCursorPos(&cursorPos);
        direction = (double)(cursorPos.x - ud.SCREEN_WIDTH / 2) / (ud.SCREEN_WIDTH / 2);
        absDirection = abs(direction);

        vertVelMod = (double)cursorPos.y / ud.SCREEN_HEIGHT + 1;
    }

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
    BitBlt(ud.hDC, 0, 0, ud.SCREEN_WIDTH, ud.SCREEN_HEIGHT, ud.hDCMem, 0, 0, SRCCOPY);
}
}  // namespace EternalSnow