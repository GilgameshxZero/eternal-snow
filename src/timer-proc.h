#pragma once

#include <Windows.h>
#include "snow-particle.h"

namespace EternalSnow {
extern HDC hdc, hdcmem;
extern HBITMAP hbmmem;
extern HANDLE oldhand;
extern RECT snowrect, screen;
extern HBRUSH snowbrush, black;
extern DWORD last_time;

VOID CALLBACK TimerProc(_In_ HWND hwnd, _In_ UINT umsg, _In_ UINT_PTR idevent, _In_ DWORD time);
void InitDraw(HWND hwnd);
void EndDraw(HWND hwnd);
}  // namespace EternalSnow