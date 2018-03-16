#include "TimerProc.h"

namespace EternalSnow
{
	HDC hdc, hdcmem/*, hdesktop, desktopmem, mem2*/;
	HBITMAP hbmmem/*, desktopbm*/;
	HANDLE oldhand/*, olddesktopbm*/;
	RECT snowrect, screen;
	HBRUSH snowbrush, black, oldbrush;
	HPEN snowpen, oldpen;
	DWORD last_time;

	//We will only have one timer in the window - the fps timer.
	VOID CALLBACK TimerProc (_In_ HWND hwnd, _In_ UINT umsg, _In_ UINT_PTR idevent, _In_ DWORD time)
	{
		static const int fps = 60;
		static const int mpf = 1000 / fps,
			width = GetSystemMetrics (SM_CXSCREEN),
			height = GetSystemMetrics (SM_CYSCREEN);
		static int max_snow = (int)width * height / 200/*20*/;
		static const double snowrad = 2;
		static int k;
		static double newx, newy, dir, timer, absdir;
		static DWORD finaltime;
		static POINT cursorpos;

		//ShowWindow (hwnd, SW_HIDE);

		//Get direction of snow based on cursor.
		GetCursorPos (&cursorpos);
		dir = (double)(cursorpos.x - width / 2) / (width / 2);
		absdir = abs (dir);

		//BitBlt (hdcmem, 0, 0, width, height, hdesktop, 0, 0, SRCCOPY);

		/*for (int a = 0; a < snowvec.size (); a++)
		{
			BitBlt (hdcmem, snowvec[a].x - snowrad, snowvec[a].y - snowrad, snowrad * 2, snowrad * 2, hdcmem, snowvec[a].x - snowrad, snowvec[a].y - snowrad, NOTSRCCOPY);
		}*/

		//BitBlt (hdesktop, 0, 0, width, height, hdcmem, 0, 0, SRCCOPY);
		//BitBlt (hdcmem, 0, 0, width, height, hdesktop, 0, 0, SRCCOPY);
		//BitBlt (desktopmem, 0, 0, width, height, hdesktop, 0, 0, SRCCOPY);

		//Clear screen.
		FillRect (hdcmem, &screen, black);

		//Draw snow circles all over the screen.
		for (int a = 0;a < snowvec.size ();a++)
		{
			//BitBlt (hdcmem, snowvec[a].x - snowrad, snowvec[a].y - snowrad, snowrad * 2, snowrad * 2, hdcmem, snowvec[a].x - snowrad, snowvec[a].y - snowrad, NOTSRCCOPY);
			//BitBlt (hdcmem, snowvec[a].x - snowrad, snowvec[a].y - snowrad, snowrad * 2, snowrad * 2, hdcmem, snowvec[a].x - snowrad, snowvec[a].y - snowrad, SRCCOPY);

			//Calculate new position, check if out of bounds.
			newx = snowvec[a].x + dir * (int)(time - last_time) * snowvec[a].hvel;
			newy = snowvec[a].y + (int)(time - last_time) * snowvec[a].vvel;

			//No snow stuck on sides.
			//if (newx == snowvec[a].x)
			//{
			//	if (dir >= 0)
			//		newx++;
			//	else
			//		newx--;
			//}

			if (newy > height || newx < 0 || newx > width)
			{
				snowvec[a].vvel = (double)(rand () % 100 + 50) / 1000;
				snowvec[a].hvel = (double)(rand () % 200 + 100) / 1000;
				k = rand () % (int)((snowvec[a].vvel * width + absdir * snowvec[a].hvel * height) * 10);
				if (k < absdir * snowvec[a].hvel * height * 10)
				{
					if (dir >= 0)
						newx = 0;
					else
						newx = width;
					newy = height * k / (absdir * snowvec[a].hvel * height * 10);
				}
				else
				{
					newx = width * (k - absdir * snowvec[a].hvel * height * 10) / (snowvec[a].vvel * width * 10);
					newy = 0;
				}
			}

			snowvec[a].x = newx;
			snowvec[a].y = newy;
			Ellipse (hdcmem, newx - snowrad, newy - snowrad, newx + snowrad, newy + snowrad);
			//BitBlt (hdcmem, newx - snowrad, newy - snowrad, snowrad * 2, snowrad * 2, desktopmem, newx - snowrad, newy - snowrad, NOTSRCCOPY);
		}

		if (snowvec.size () < max_snow)
		{
			snowvec.push_back (SnowParticle ());
			snowvec.back ().vvel = (double)(rand () % 100 + 50) / 1000;
			snowvec.back ().hvel = (double)(rand () % 200 + 100) / 1000;
			k = rand () % (int)((snowvec.back ().vvel * width + absdir * snowvec.back ().hvel * height) * 10);
			if (k < absdir * snowvec.back ().hvel * height * 10)
			{
				if (dir >= 0)
					snowvec.back ().x = 0;
				else
					snowvec.back ().x = width;
				snowvec.back ().y = height * k / (absdir * snowvec.back ().hvel * height * 10);
			}
			else
			{
				snowvec.back ().x = width * (k - absdir * snowvec.back ().hvel * height * 10) / (snowvec.back ().vvel * width * 10);
				snowvec.back ().y = 0;
			}
		}

		//Transfer the off-screen DC to the screen.
		BitBlt (hdc, 0, 0, width, height, hdcmem, 0, 0, SRCCOPY);
		//BitBlt (hdesktop, 0, 0, width, height, hdcmem, 0, 0, SRCCOPY);
		
		//BitBlt (mem2, 0, 0, width, height, hdcmem, 0, 0, SRCCOPY);

		//ShowWindow (hwnd, SW_SHOW);

		//Save the last update time.
		last_time = time;

		//Set timer to fulfill fps.
		finaltime = GetTickCount ();
		timer = mpf - ((int)finaltime - (int)time);
		while (timer < 0)
			timer += mpf;
		SetTimer (hwnd, idevent, timer, TimerProc);
	}

	void InitDraw (HWND hwnd)
	{
		static const int width = GetSystemMetrics (SM_CXSCREEN),
			height = GetSystemMetrics (SM_CYSCREEN);
		static const COLORREF snow_color = RGB (200, 200, 230);

		//hdesktop = GetDC (NULL);

		hdc = GetDC (hwnd);
		hdcmem = CreateCompatibleDC (hdc);
		//desktopmem = CreateCompatibleDC (hdc);
		//hdcmem = CreateCompatibleDC (hdesktop);
		//mem2 = CreateCompatibleDC (hdesktop);
		hbmmem = CreateCompatibleBitmap (hdc, width, height);
		//desktopbm = CreateCompatibleBitmap (hdc, width, height);

		oldhand = SelectObject (hdcmem, hbmmem);
		//olddesktopbm = SelectObject (desktopmem, desktopbm);

		screen.top = 0;
		screen.left = 0;
		screen.bottom = screen.top + height;
		screen.right = screen.left + width;

		snowbrush = CreateSolidBrush (snow_color);
		black = CreateSolidBrush (RGB (255, 255, 255));
		snowpen = CreatePen (PS_SOLID, 0, snow_color);

		oldbrush = (HBRUSH)SelectObject (hdcmem, snowbrush);
		oldpen = (HPEN)SelectObject (hdcmem, snowpen);
	}

	void EndDraw (HWND hwnd)
	{
		DeleteObject (snowbrush);
		DeleteObject (snowpen);
		DeleteObject (black);

		SelectObject (hdcmem, oldhand);
		SelectObject (hdcmem, oldbrush);
		SelectObject (hdcmem, oldpen);
		//SelectObject (desktopmem, olddesktopbm);
		DeleteObject (hbmmem);
		//DeleteObject (desktopbm);
		DeleteDC (hdcmem);
		//DeleteDC (desktopmem);

		ReleaseDC (hwnd, hdc);
		//ReleaseDC (0, hdesktop);
	}
}