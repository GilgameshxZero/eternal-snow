#pragma once

#define _MBCS

#include "rain-aeternum/rain-libraries.h"

#include "user-data.h"
#include "wnd-procs.h"
#include <ctime>
#include <Windows.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

int CALLBACK WinMain (HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow);