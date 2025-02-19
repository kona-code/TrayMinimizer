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

void DebugConsole::RunCommandLoop() {
    std::string command;
    while (true) {
        std::getline(std::cin, command);
        if (command == "list") {
            std::cout << "[INF] Listing all open windows...\n";
        }
        else if (command == "exit") {
            std::cout << "[INF] Closing debug console...\n";
            Hide();
            break;
        }
        else {
            std::cout << "[ERR] Unknown command: " << command << "\n";
        }
    }
}
