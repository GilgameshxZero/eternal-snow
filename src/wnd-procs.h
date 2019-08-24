#pragma once

#include <Windows.h>

#include "snow-particle.h"
#include "user-data.h"

namespace EternalSnow {
	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR eventId, DWORD time);
	LRESULT CALLBACK MouseLLHookProc(int nCode, WPARAM wParam, LPARAM lParam);
}