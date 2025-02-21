#pragma once
#include <windows.h>
#include <vector>
#include <string>

struct WindowInfo {
    HWND hwnd;
    std::string title;
};

class FUNC {
public:
    static void ListOpenWindows();
    static std::vector<WindowInfo> GetOpenWindows();
    static void HideWindow(HWND hwnd);
    static void ShowWindowAgain(HWND hwnd);
};
