#pragma once

#define _MBCS

#include "wnd-procs.h"
#include <ctime>
#include <Windows.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

int CALLBACK WinMain (HINSTANCE hinst, HINSTANCE hprevinst, LPSTR cmdLine, int cmdShow);