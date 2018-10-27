#pragma once

#include <Windows.h>
#include "snow-particle.h"

namespace EternalSnow {
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR eventId, DWORD time);
void InitDraw(HWND hWnd);
void EndDraw(HWND hWnd);
}