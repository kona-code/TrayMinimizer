#include "Console.h"
#include <windows.h>
#include <cstdio>

DebugConsole::DebugConsole() {
    // constructor can set up initial state if needed
}

DebugConsole::~DebugConsole() {
    // clean up console resources if needed
    FreeConsole();
}

void DebugConsole::Show() {
    AllocConsole();
    FILE* fp;
    // redirect standard streams to the console:
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);
    std::cout << "[INF] Console initialized.\n";
    // start the command loop on a separate thread so it doesn't block the GUI
    std::thread t(&DebugConsole::RunCommandLoop, this);
    t.detach();
}

void DebugConsole::Hide() {
    FreeConsole();
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (IsWindowVisible(hwnd)) {
        wchar_t windowTitle[256];
        int length = GetWindowTextW(hwnd, windowTitle, sizeof(windowTitle) / sizeof(wchar_t));
        if (length > 0) {
            wprintf(L"[INF] Window: %s\n", windowTitle);  // log the title
        }
        else {
            wprintf(L"[WRN] Window with no title detected.\n");  // if no title is found
        }
    }
    return TRUE;  // continue enumerating windows
}


void ListOpenWindows() {
    EnumWindows(EnumWindowsProc, 0);
}

void DebugConsole::RunCommandLoop() {
    std::string command;
    while (true) {
        std::getline(std::cin, command);
        if (command == "help") {
            std::cout << "[INF] [1] 'list' - lists all current open windows (of applications)\n[INF] [2] 'exit' - exits the console\n";
        }
        else if (command == "list") {
            std::cout << "[INF] Listing all open windows...\n";
            ListOpenWindows();
        }
        else if (command == "exit") {
            FreeConsole();
            std::cout << "[INF] Terminated the console\n";
            break;
        }
        else {
            std::cout << "[ERR] Unknown command: " << command << "\n";
        }
    }
}
