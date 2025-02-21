#pragma once
#include <windows.h>
#include <vector>
#include <string>

struct WindowInfo {
    HWND hwnd;
    std::string title;
    bool isVisible;
};

class FUNC {
public:
    static std::vector<WindowInfo> GetOpenWindows();
    static std::vector<WindowInfo>& GetHiddenWindows();

    static bool is_digits(const std::string& str);
    static void HideWindow(HWND hwnd);
    static void ShowWindowAgain(HWND hwnd);
    static void RestoreWindow(HWND hwnd);
};