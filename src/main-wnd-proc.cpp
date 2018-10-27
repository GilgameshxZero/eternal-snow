#include "main-wnd-proc.h"

namespace EternalSnow {
	LRESULT CALLBACK MainWndProc(_In_ HWND hwnd, _In_ UINT umsg, _In_ WPARAM wparam, _In_ LPARAM lparam) {
		switch(umsg) {
			default:
				break;
		}

		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}