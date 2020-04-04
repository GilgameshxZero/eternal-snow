#pragma once
#include "../rain/cpp/rain.hpp"

#include "snow-particle.hpp"

namespace EternalSnow {
	struct UserData {
		int SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SNOW_LIMIT,
			FRAMES_PER_SECOND,
			MS_PER_FRAME,
			TIMER_ID,
			TRAY_ICON_ID;
		double SNOW_RADIUS,
			SNOW_LIMIT_SCALING,
			SNOW_SPEED_FAST;
		std::string CLASS_NAME,
			CONFIG_LOC,
			SNOW_COLOR_HEX,
			HIDE_CONSOLE;
		COLORREF SNOW_COLOR,
			BLACK_COLOR;

		HINSTANCE hInst;

		std::vector<SnowParticle> snowParts;
		bool paused;
		POINT mousePos;

		HDC hDC, hDCMem;
		HBITMAP hBMMem, oldHBM;
		HPEN snowPen, oldPen;
		HBRUSH snowBrush, blackBrush, oldBrush;
		HHOOK hMouseLLHook;
		RECT screenRect;
	};

	//hookproc useful
	extern UserData* pud;
}