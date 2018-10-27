#pragma once

#include <string>
#include <vector>
#include <Windows.h>

namespace EternalSnow {
struct UserData {
    int SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SNOW_LIMIT_SCALING,
        SNOW_LIMIT,
        FRAMES_PER_SECOND,
        MS_PER_FRAME,
        TIMER_ID;
    double SNOW_RADIUS;
    std::string CLASS_NAME,
        CONFIG_LOC,
        SNOW_COLOR_HEX;
    COLORREF SNOW_COLOR,
        BLACK_COLOR;

    std::vector<SnowParticle> snowParts;

    HDC hDC, hDCMem;
    HBITMAP hBMMem, oldHBM;
    HPEN snowPen, oldPen;
    HBRUSH snowBrush, blackBrush, oldBrush;
    RECT screenRect;
};
}  // namespace EternalSnow