#pragma once
#include "../rain/src/rain.hpp"

#include "snow-particle.hpp"
#include "user-data.hpp"

namespace EternalSnow {
	LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR eventId, DWORD time);
	LRESULT CALLBACK MouseLLHookProc(int nCode, WPARAM wParam, LPARAM lParam);
}