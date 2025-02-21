#include "Console.h"
#include "Functions.h"
#include <iostream>
#include <thread>
#include <map>

FUNC f;
DebugConsole console;

DebugConsole::DebugConsole() {}

DebugConsole::~DebugConsole() {
    FreeConsole();
}

void DebugConsole::Show() {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);

    std::cout << "[INF] Console initialized.\n";
    std::thread t(&DebugConsole::RunCommandLoop, this);
    t.detach();
}

void DebugConsole::Hide() {
    FreeConsole();
}

void DebugConsole::RunCommandLoop() {
    std::string input;

    std::map<std::string, void(DebugConsole::*)()> commandMap = {
        {"list", &DebugConsole::ListWindows},
        {"hide", &DebugConsole::HideWindow},
        {"show", &DebugConsole::ShowWindowAgain},  // New show command
        {"help", &DebugConsole::HelpFunction},
        {"exit", &DebugConsole::Exit}
    };

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        auto cmd = commandMap.find(input);
        if (cmd != commandMap.end()) {
            (this->*(cmd->second))();
        }
        else {
            DebugConsole::SetConsoleColor(12);
            std::cout << "[ERR] Unknown command: " << input << ", use 'help' to show available commands." << std::endl;
            DebugConsole::SetConsoleColor(7);
        }
    }
}

void DebugConsole::ListWindows() {
    std::cout << "Listing windows..." << std::endl;
    std::vector<WindowInfo> windows = FUNC::GetOpenWindows();

    if (windows.empty()) {
        std::cout << "No open windows found.\n";
        return;
    }

    std::cout << "Open Windows:\n";
    for (size_t i = 0; i < windows.size(); ++i) {
        std::cout << "[" << i << "] " << windows[i].title << std::endl;
    }
}

void DebugConsole::HideWindow() {
    std::cout << "Enter window number to hide: ";
    std::string windowIndexStr;
    std::getline(std::cin, windowIndexStr);

    int windowIndex = std::stoi(windowIndexStr);
    std::vector<WindowInfo> windows = FUNC::GetOpenWindows();

    if (windowIndex >= 0 && windowIndex < windows.size()) {
        HWND hwnd = windows[windowIndex].hwnd;
        FUNC::HideWindow(hwnd);
        hiddenWindows.push_back(hwnd);  // Store hidden window
        std::cout << "[INF] Window hidden.\n";
    }
    else {
        std::cout << "[ERR] Invalid selection.\n";
    }
}

void DebugConsole::ShowWindowAgain() {
    if (hiddenWindows.empty()) {
        std::cout << "[INF] No windows to restore.\n";
        return;
    }

    std::cout << "Restorable Windows:\n";
    for (size_t i = 0; i < hiddenWindows.size(); ++i) {
        char title[256];
        GetWindowTextA(hiddenWindows[i], title, sizeof(title));
        std::cout << "[" << i << "] " << title << std::endl;
    }

    std::cout << "Enter window number to restore: ";
    std::string input;
    std::getline(std::cin, input);
    int index = std::stoi(input);

    if (index >= 0 && index < hiddenWindows.size()) {
        HWND hwnd = hiddenWindows[index];
        FUNC::ShowWindowAgain(hwnd);
        hiddenWindows.erase(hiddenWindows.begin() + index);
        std::cout << "[INF] Window restored.\n";
    }
    else {
        std::cout << "[ERR] Invalid selection.\n";
    }
}

void DebugConsole::HelpFunction() {
    std::cout << "[INF] [1] help commands:\n";
    std::cout << "[INF] [1] 'help' - shows all commands, and explains what they do.\n\n";
    std::cout << "[INF] [2] listing commands:\n";
    std::cout << "[INF] [2] 'list' - shows all currently detectable windows, that have titles.\n\n";
    std::cout << "[INF] [3] controlling commands:\n";
    std::cout << "[INF] [3] 'hide' - hides a window and removes it from the taskbar.\n";
    std::cout << "[INF] [3] 'show' - restores a previously hidden window.\n";
    std::cout << "[INF] [3] 'exit' - terminates all operations and exits the application.\n\n";
}

void DebugConsole::Exit() {
    std::cout << "Terminating Program...\n";
    exit(0);
}
