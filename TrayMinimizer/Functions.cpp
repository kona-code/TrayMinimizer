#include "Functions.h"
#include "Console.h" // for hiding console
#include <iostream>

static std::vector<WindowInfo> hiddenWindows;

std::vector<WindowInfo> FUNC::GetOpenWindows() {
    std::vector<WindowInfo> windows;

    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        char title[256] = { 0 };
        GetWindowTextA(hwnd, title, sizeof(title));
        // Only add windows that have a title.
        if (strlen(title) > 0) {
            auto* windows = reinterpret_cast<std::vector<WindowInfo>*>(lParam);
            WindowInfo info;
            info.hwnd = hwnd;
            info.title = title;
            info.isVisible = IsWindowVisible(hwnd) != 0;
            windows->push_back(info);
        }
        return TRUE;
        }, reinterpret_cast<LPARAM>(&windows));

    return windows;
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

void FUNC::RestoreWindow(HWND hwnd) {
    // Check if the window is minimized or hidden and restore it
    ShowWindow(hwnd, SW_SHOWNORMAL);  // Restores the window to its normal state
    SetForegroundWindow(hwnd);        // Optionally bring the window to the foreground
}


std::vector<WindowInfo>& FUNC::GetHiddenWindows() {
    return hiddenWindows;
}

bool FUNC::is_digits(const std::string& str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}