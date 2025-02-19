#include <windows.h>
#include <iostream>

// message handler
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
	switch (uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// winmain entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { 
    const WCHAR TRAY_MINIMIZER[] = L"TrayMinimizer"; // window class definition

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TRAY_MINIMIZER;

    RegisterClass(&wc); // register and create a new window

    HWND hwnd = CreateWindowEx(
        0, TRAY_MINIMIZER, L"Window to Tray", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,
        NULL, NULL, hInstance, NULL);

    if (!hwnd) return 0; // exits if creation fails

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {}; // message loop - event loop that keeps the code running
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// callback function for EnumWindows
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    wchar_t windowTitle[256]; // buffer for the window title

    // get the window title
    if (GetWindowTextW(hwnd, windowTitle, sizeof(windowTitle) / sizeof(wchar_t)) > 0) {
        std::wcout << L"Window Found: " << windowTitle << std::endl;
    }

    return TRUE; // continue enumerating windows
}

// function to start listing windows
void ListOpenWindows() {
    std::wcout << L"Enumerating open windows...\n";
    EnumWindows(EnumWindowsProc, 0);
}