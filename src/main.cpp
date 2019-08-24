#include "main.h"

int main(int argc, const char* argv[]) {
	EternalSnow::UserData ud;
	EternalSnow::pud = &ud;

	//set ctrl+c handler (doesn't work right now...)
	if (!SetConsoleCtrlHandler([](DWORD fdwCtrlType) -> BOOL {
		switch (fdwCtrlType) {
		case CTRL_C_EVENT:
			PostQuitMessage(0);
			return TRUE;
		default:
			return FALSE;
		}
		}, TRUE)) {
		Rain::reportError(GetLastError(), "SetConsoleCtrlHandler failed while capturing ctrl+c signal; continuing...");
	}

	//configuration
	std::map<std::string, std::string> config;

	//provide default values
	config["hide"] = "yes";
	config["paused"] = "no";
	config["fps"] = "90";
	config["limit"] = "2";
	config["radius"] = "2.5";
	config["color"] = "c8c8e6";
	config["fast"] = "3";

	//read command line
	for (int a = 0; a < argc; a++) {
		std::string argument = argv[a];
		
		//split the argument by =
		std::size_t splitPosition = argument.find('=');
		if (splitPosition == argument.length()) continue;
		std::string option = argument.substr(0, splitPosition),
			value = argument.substr(splitPosition + 1);
		config[option] = value;
	}
	ud.HIDE_CONSOLE = config["hide"];
	ud.FRAMES_PER_SECOND = Rain::strToT<int>(config["fps"]);
	ud.SNOW_LIMIT_SCALING = Rain::strToT<double>(config["limit"]);
	ud.SNOW_RADIUS = Rain::strToT<double>(config["radius"]);
	ud.SNOW_COLOR_HEX = config["color"];
	ud.SNOW_SPEED_FAST = Rain::strToT<double>(config["fast"]);

	ud.SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
	ud.SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
	ud.MS_PER_FRAME = 1000 / ud.FRAMES_PER_SECOND;
	ud.SNOW_LIMIT = static_cast<int>(ud.SCREEN_WIDTH * ud.SCREEN_HEIGHT / 20000 * ud.SNOW_LIMIT_SCALING);
	ud.TIMER_ID = 1;
	ud.TRAY_ICON_ID = 1;
	ud.CLASS_NAME = "EternalSnow Main Window";
	ud.SNOW_COLOR = Rain::colorFromHex(ud.SNOW_COLOR_HEX);
	ud.BLACK_COLOR = RGB(0, 0, 1);

	ud.hInst = GetModuleHandle(NULL);
	GetCursorPos(&ud.mousePos);

	ud.paused = false;

	//hide console
	if (ud.HIDE_CONSOLE == "yes") FreeConsole();

	//Randomize.
	srand(static_cast<unsigned int>(time(NULL)));

	{
		WNDCLASSEX wndClass;
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = NULL;
		wndClass.lpfnWndProc = EternalSnow::MainWndProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = ud.hInst;
		wndClass.hIcon = NULL;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = NULL;
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = ud.CLASS_NAME.c_str();
		wndClass.hIconSm = NULL;

		if (!RegisterClassEx(&wndClass)) {
			return -1;
		}
	}

	HWND mainWnd = CreateWindowEx(WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, ud.CLASS_NAME.c_str(), "", WS_POPUP, 0, 0, ud.SCREEN_WIDTH, ud.SCREEN_HEIGHT, NULL, NULL, ud.hInst, NULL);

	if (!mainWnd) {
		return -1;
	}

	//make sure variables declared here are accessible by procs
	SetWindowLongPtr(mainWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&ud));

	ShowWindow(mainWnd, SW_SHOW);

	//hook the mouse so that we can update mouse positions
	ud.hMouseLLHook = SetWindowsHookEx(WH_MOUSE_LL, EternalSnow::MouseLLHookProc, NULL, NULL);

	//system tray icon
	NOTIFYICONDATA nid;

	ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uID = ud.TRAY_ICON_ID;
	nid.uFlags = NIF_ICON | NIF_MESSAGE;
	nid.hIcon = static_cast<HICON>(
		LoadImage(ud.hInst,
			MAKEINTRESOURCE(1),  //program icon
			IMAGE_ICON,
			GetSystemMetrics(SM_CXSMICON),
			GetSystemMetrics(SM_CYSMICON),
			LR_DEFAULTCOLOR));
	nid.hWnd = mainWnd;
	nid.uCallbackMessage = WM_APP;

	if (!Shell_NotifyIcon(NIM_ADD, &nid)) {
		return -1;
	}

	//free the icon
	DestroyIcon(nid.hIcon);

	//setup timer
	SetTimer(mainWnd, ud.TIMER_ID, ud.MS_PER_FRAME, EternalSnow::TimerProc);

	//setup drawing-related handles
	ud.hDC = GetDC(mainWnd);
	ud.hDCMem = CreateCompatibleDC(ud.hDC);
	ud.hBMMem = CreateCompatibleBitmap(ud.hDC, ud.SCREEN_WIDTH, ud.SCREEN_HEIGHT);

	ud.oldHBM = static_cast<HBITMAP>(SelectObject(ud.hDCMem, ud.hBMMem));

	ud.screenRect = { 0, 0, ud.SCREEN_WIDTH, ud.SCREEN_HEIGHT };
	ud.snowBrush = CreateSolidBrush(ud.SNOW_COLOR);
	ud.blackBrush = CreateSolidBrush(ud.BLACK_COLOR);
	ud.snowPen = CreatePen(PS_SOLID, 1, ud.SNOW_COLOR);

	ud.oldBrush = static_cast<HBRUSH>(SelectObject(ud.hDCMem, ud.snowBrush));
	ud.oldPen = static_cast<HPEN>(SelectObject(ud.hDCMem, ud.snowPen));

	//send a synthetic message to pause the snow if paused was chosen
	if (config["paused"] == "yes") SendMessage(mainWnd, WM_APP, NULL, WM_LBUTTONUP);

	{
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//free all objects
	UnhookWindowsHookEx(ud.hMouseLLHook);

	Shell_NotifyIcon(NIM_DELETE, &nid);

	DeleteObject(ud.snowBrush);
	DeleteObject(ud.blackBrush);
	DeleteObject(ud.snowPen);

	SelectObject(ud.hDCMem, ud.oldPen);
	SelectObject(ud.hDCMem, ud.oldHBM);
	SelectObject(ud.hDCMem, ud.oldBrush);
	DeleteObject(ud.hBMMem);
	DeleteDC(ud.hDCMem);

	ReleaseDC(mainWnd, ud.hDC);
	return 0;
}