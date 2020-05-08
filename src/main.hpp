#pragma once

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Dwmapi.lib")

#include "../rain/src/rain.hpp"

#include "user-data.hpp"
#include "wnd-procs.hpp"

#include <ctime>
#include <dwmapi.h>
#include <Shellapi.h>

int main(int argc, const char *argv[]);