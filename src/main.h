#pragma once

#define _MBCS

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "Shell32.lib")

#include "rain-aeternum/rain-libraries.h"

#include <ctime>
#include <Shellapi.h>
#include <Windows.h>

#include "user-data.h"
#include "wnd-procs.h"

int main(int argc, const char *argv[]);