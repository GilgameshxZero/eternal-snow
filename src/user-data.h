#pragma once

#include <string>
#include <Windows.h>

namespace EternalSnow {
struct UserData {
    int SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SNOW_LIMIT,
        FRAMES_PER_SECOND,
        MS_PER_FRAME,
        TIMER_ID;
    double SNOW_RADIUS;
    std::string CLASS_NAME;

    HDC hDC, hDCMem;
    HBITMAP hBMMem;
    HANDLE oldHDC;
    HPEN oldPen, snowPen;
    RECT screen;
    HBRUSH snowBrush, blackBrush, oldBrush;
};
}  // namespace EternalSnow