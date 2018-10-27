#pragma once

#define _MBCS

#include "main-wnd-proc.h"
#include "timer-proc.h"
#include <ctime>
#include <Windows.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

int CALLBACK WinMain (_In_ HINSTANCE hinst, _In_ HINSTANCE hprevinst, _In_ LPSTR cmdline, _In_ int cmdshow);