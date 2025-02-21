#include "Functions.h"
#include <iostream>

std::vector<WindowInfo> FUNC::GetOpenWindows() {
    std::vector<WindowInfo> windows;

    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        char title[256];
        GetWindowTextA(hwnd, title, sizeof(title));

        if (IsWindowVisible(hwnd) && strlen(title) > 0) {
            auto* windows = reinterpret_cast<std::vector<WindowInfo>*>(lParam);
            windows->push_back({ hwnd, title });
        }
        return TRUE;
        }, reinterpret_cast<LPARAM>(&windows));

    return windows;
}

void FUNC::ListOpenWindows() {
    std::vector<WindowInfo> windows = GetOpenWindows();

    if (windows.empty()) {
        std::cout << "No open windows found.\n";
        return;
    }

    std::cout << "Open Windows:\n";
    for (size_t i = 0; i < windows.size(); ++i) {
        std::cout << "[" << i << "] " << windows[i].title << std::endl;
    }
}

// function to hide a window and remove it from the taskbar
void FUNC::HideWindow(HWND hwnd) {
    if (!IsWindow(hwnd)) return;

    ShowWindow(hwnd, SW_HIDE);  // hide window
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);  // remove from taskbar
}

// function to restore a hidden window
void FUNC::ShowWindowAgain(HWND hwnd) {
    if (!IsWindow(hwnd)) return;

    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~WS_EX_TOOLWINDOW);  // restore to taskbar
    ShowWindow(hwnd, SW_SHOW);  // restore window
}