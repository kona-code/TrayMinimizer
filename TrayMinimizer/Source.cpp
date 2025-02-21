#include <windows.h>
#include "Console.h"
#include "Tray.h"

// global instances
TrayIcon g_TrayIcon;
DebugConsole g_DebugConsole;

// forward declaration of window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // copyright
    std::cout << "@@@@@@@@*.                   =@@@@@@@@- \n@@-::::-#@*.               =@%=:::::%@=:\n@@-:     :#@*.           =@%=::.    #@=:\n@@=:       :#@*.       =@%=::.      %@=:\n.*@#:        :#@*.   =@%=::.      +@#-::\n  .*@#:        :#@- %%=::.      +@#-::. \n    .*@#:         .:  :.      +@#-::.   \n      .*@#:                 +@#-::.     \n        .*@*              -@#-::.       \n           ::.             .::.         \n         =@#              =@*.          \n       =@%=::.             :#@*.        \n     =@%=::.                 :#@*.      \n   =@%=::.      +%: #*:        :#@*.    \n =@%=::.      +@#-::.*@#:        :#@*.  \n@@=::.      +@#-::.   .*@#:        :%@- \n@@-:      +@#-::.       .*@#:       #@=:\n@@-:    +@#-::.           .*@#:     #@=:\n@@@@%%%@#-::.               .*@%%%%%@@=:\n.:------::.                    :-------:\n\n";
    std::cout << "      (C) NightVoid Entertainment\n";
    std::cout << "         https://nightvoid.com/\n";
    std::cout << "              By Kona Code\n\n\n";
    std::cout << "Copyright Policy: https://nightvoid.com/copyright\n";
    std::cout << "Terms of Service: https://nightvoid.com/terms-of-service\n\n";


    // register the window class.
    const wchar_t CLASS_NAME[] = L"TrayMinimizer";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClassW(&wc);

    // create the window.
    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, L"Tray Minimized App", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 512, 512,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    // show console
    g_DebugConsole.Show();
    // create the tray icon.
    g_TrayIcon.CreateTrayIcon(hwnd, hInstance);

    // main message loop.
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;
}

// window procedure for handling messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // handle tray icon callback messages.
    if (uMsg == g_TrayIcon.GetCallbackMessage()) {
        if (lParam == WM_RBUTTONUP) {
            g_TrayIcon.UpdateTrayMenu();
            g_TrayIcon.ShowContextMenu(hwnd);
        }
    }
    else if (uMsg == WM_COMMAND) {
        switch (LOWORD(wParam)) {
        case 1: // "Debug" selected from the tray menu.
            g_DebugConsole.ShowFromTray();
            break;
        case 3: // "Exit" selected from the tray menu.
            g_DebugConsole.HideFromTray();
            break;
        case 4:
            ShellExecute(NULL, L"open", L"https://github.com/kona-code/TrayMinimizer", NULL, NULL, SW_SHOWNORMAL);
            break;
        case 5:
            ShellExecute(NULL, L"open", L"https://software.nightvoid.com/stable/#tray-minimizer", NULL, NULL, SW_SHOWNORMAL);
            break;
        case 6:
            ShellExecute(NULL, L"open", L"https://nightvoid.com/github", NULL, NULL, SW_SHOWNORMAL);
            break;
        case 7: // "Exit" selected from the tray menu.
            g_TrayIcon.DestroyTrayIcon();
            PostQuitMessage(0);
            break;
        }
    }
    else if (uMsg == WM_DESTROY) {
        DebugConsole console;
        size_t i = 0;
        if (i < console.hiddenWindows.size()) {
            if (MessageBox(hwnd, L"Are you sure you want to quit the application?\nThis will stop all current processes.", L"Confirm Exit", MB_ICONQUESTION | MB_YESNO) == IDNO) {
                return 0; // Prevent closing
            }
            else {
                PostQuitMessage(0);
            }
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
