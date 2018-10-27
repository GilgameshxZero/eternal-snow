#pragma once

#include "snow-particle.h"
#include "user-data.h"
#include <Windows.h>

namespace EternalSnow {
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR eventId, DWORD time);
}