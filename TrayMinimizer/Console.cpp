#include "Console.h"
#include "Functions.h"
#include <iostream>
#include <thread>
#include <map>

FUNC f;
DebugConsole console;

DebugConsole::DebugConsole() : consoleAlive(true) {}

DebugConsole::~DebugConsole() {
    FreeConsole();
}

void DebugConsole::Show() {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);
    // copyright notice
    std::cout << "@@@@@@@@*.                   =@@@@@@@@- \n@@-::::-#@*.               =@%=:::::%@=:\n@@-:     :#@*.           =@%=::.    #@=:\n@@=:       :#@*.       =@%=::.      %@=:\n.*@#:        :#@*.   =@%=::.      +@#-::\n  .*@#:        :#@- %%=::.      +@#-::. \n    .*@#:         .:  :.      +@#-::.   \n      .*@#:                 +@#-::.     \n        .*@*              -@#-::.       \n           ::.             .::.         \n         =@#              =@*.          \n       =@%=::.             :#@*.        \n     =@%=::.                 :#@*.      \n   =@%=::.      +%: #*:        :#@*.    \n =@%=::.      +@#-::.*@#:        :#@*.  \n@@=::.      +@#-::.   .*@#:        :%@- \n@@-:      +@#-::.       .*@#:       #@=:\n@@-:    +@#-::.           .*@#:     #@=:\n@@@@%%%@#-::.               .*@%%%%%@@=:\n.:------::.                    :-------:\n\n";
    std::cout << "      (C) NightVoid Entertainment\n";
    std::cout << "         https://nightvoid.com/\n";
    std::cout << "              By Kona Code\n\n\n";
    std::cout << "Copyright Policy: https://nightvoid.com/copyright\n";
    std::cout << "Terms of Service: https://nightvoid.com/terms-of-service\n\n";
    std::cout << "[INF] Console initialized.\n";
    std::thread t(&DebugConsole::RunCommandLoop, this);
    t.detach();
}

void DebugConsole::RunCommandLoop() {
    std::string input;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "help") {
            HelpFunction();
        }
        else if (input == "list") {
            ListWindows(""); // default: show all
        }
        else if (input == "list -all" || input == "list -a") {
            ListWindows("-all");
        }
        else if (input == "list -hidden" || input == "list -h") {
            ListWindows("-hidden");
        }
        else if (input == "hide") {
            HideWindow();
        }
        else if (input == "show") {
            ShowWindowAgain();
        }
        else if (input == "restore") {
            RestoreWindow();
        }
        else if (input == "quit" || "terminate") {
            Quit();
        }
        else if (input == "exit") {
            Hide();
        }
        else {
            DebugConsole::SetConsoleColor(12);
            std::cout << "[ERR] Unknown command: " << input << ", use 'help' to show all available commands." << std::endl;
            DebugConsole::SetConsoleColor(7);
        }
    }
}



void DebugConsole::ListWindows(const std::string& option) {
    if (option == "-h" || option == "-hidden") {
        if (hiddenWindows.empty()) {
            DebugConsole::SetConsoleColor(12);
            std::cout << "[INF] No windows are currently hidden.\n";
            DebugConsole::SetConsoleColor(7);
            return;
        }
        std::cout << "[INF] List of currently hidden windows:\n";
        for (size_t i = 0; i < hiddenWindows.size(); ++i) {
            char title[256];
            GetWindowTextA(hiddenWindows[i], title, sizeof(title));

            DebugConsole::SetConsoleColor(3);
            std::cout << "[";
            DebugConsole::SetConsoleColor(10);
            std::cout << i;
            DebugConsole::SetConsoleColor(3);
            std::cout << "] ";
            DebugConsole::SetConsoleColor(7);
            std::cout << title << std::endl;
        }
    }
    else if (option == "-a" || option == "-all") {
        std::cout << "[INF] Listing all windows:\n";
        // list all windows.
        std::vector<WindowInfo> windows = FUNC::GetOpenWindows();
        size_t idx = 0;
        for (const auto& win : windows) {
            DebugConsole::SetConsoleColor(3);
            std::cout << "[";
            DebugConsole::SetConsoleColor(10);
            std::cout << idx++;
            DebugConsole::SetConsoleColor(3);
            std::cout << "] ";
            DebugConsole::SetConsoleColor(7);
            std::cout << win.title;
            // std::cout << " (" << (win.isVisible ? "Visible" : "Hidden") << ")"; stashed, might be useful
            if (win.isVisible) {
                DebugConsole::SetConsoleColor(2);
                std::cout << " (Visible)";
            }
            else {
                DebugConsole::SetConsoleColor(3);
                std::cout << " (Hidden)";
            }
            DebugConsole::SetConsoleColor(8);
            std::cout << " (HWND: " << win.hwnd << ")\n";
            DebugConsole::SetConsoleColor(7);
        }
    }
    
    else {
        std::cout << "[INF] List of visible windows:\n";
        std::vector<WindowInfo> windows = FUNC::GetOpenWindows();
        size_t idx = 0;
        for (const auto& win : windows) {
            if (win.isVisible) {
                DebugConsole::SetConsoleColor(3);
                std::cout << "[";
                DebugConsole::SetConsoleColor(10);
                std::cout << idx++;
                DebugConsole::SetConsoleColor(3);
                std::cout << "] ";
                DebugConsole::SetConsoleColor(7);
                std::cout << win.title;
                DebugConsole::SetConsoleColor(8);
                std::cout << " (HWND: " << win.hwnd << ")\n";
                DebugConsole::SetConsoleColor(7);
            }
        }
    }
}



void DebugConsole::HideWindow() {
    std::vector<WindowInfo> windows = FUNC::GetOpenWindows();
    std::vector<WindowInfo> visibleWindows;
    size_t idx = 0;

    // filter out only visible windows
    for (const auto& win : windows) {
        if (win.isVisible) {
            DebugConsole::SetConsoleColor(3);
            std::cout << "[";
            DebugConsole::SetConsoleColor(10);
            std::cout << idx++;
            DebugConsole::SetConsoleColor(3);
            std::cout << "] ";
            DebugConsole::SetConsoleColor(7);
            std::cout << win.title;
            DebugConsole::SetConsoleColor(8);
            std::cout << " (HWND: " << win.hwnd << ")\n";
            DebugConsole::SetConsoleColor(7);
            visibleWindows.push_back(win);  // save visible windows for selection
        }
    }

    if (visibleWindows.empty()) {
        DebugConsole::SetConsoleColor(12);
        std::cout << "[ERR] No visible windows found to hide.\n";
        DebugConsole::SetConsoleColor(7);
        return;
    }

    std::cout << "Enter window number to hide: ";
    std::string windowIndexStr;
    std::getline(std::cin, windowIndexStr);
    if (FUNC::is_digits(windowIndexStr)) {
        // ensure the selected index is valid
        int windowIndex = std::stoi(windowIndexStr);

        if (windowIndex >= 0 && windowIndex < visibleWindows.size()) {
            HWND hwnd = visibleWindows[windowIndex].hwnd;
            FUNC::HideWindow(hwnd);
            hiddenWindows.push_back(hwnd);  // store hidden window
            DebugConsole::SetConsoleColor(10);
            std::cout << "[INF] Window hidden.\n";
            DebugConsole::SetConsoleColor(7);
        }
        else {
            DebugConsole::SetConsoleColor(12);
            std::cout << "[ERR] Invalid selection.\n";
            DebugConsole::SetConsoleColor(7);
        }
    }
    else {
        DebugConsole::SetConsoleColor(12);
        std::cout << "[ERR] Invalid input. Input may only consist of numbers. Use the numbers located on the right of given window list.\n";
        DebugConsole::SetConsoleColor(7);
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

        DebugConsole::SetConsoleColor(3);
        std::cout << "[";
        DebugConsole::SetConsoleColor(10);
        std::cout << i;
        DebugConsole::SetConsoleColor(3);
        std::cout << "] ";
        DebugConsole::SetConsoleColor(7);
        std::cout << title << std::endl;
    }

    std::cout << "Enter window number to restore: ";
    std::string input;
    std::getline(std::cin, input);
    if (FUNC::is_digits(input)) {
        int index = std::stoi(input);

        if (index >= 0 && index < hiddenWindows.size()) {
            HWND hwnd = hiddenWindows[index];
            FUNC::ShowWindowAgain(hwnd);
            hiddenWindows.erase(hiddenWindows.begin() + index);
            DebugConsole::SetConsoleColor(10);
            std::cout << "[INF] Window restored.\n";
            DebugConsole::SetConsoleColor(7);
        }
        else {
            DebugConsole::SetConsoleColor(12);
            std::cout << "[ERR] Invalid selection.\n";
            DebugConsole::SetConsoleColor(7);
        }
    }
    else {
        DebugConsole::SetConsoleColor(12);
        std::cout << "[ERR] Invalid input. Input may only consist of numbers. Use the numbers located on the right of given window list.\n";
        DebugConsole::SetConsoleColor(7);
    }

}

void DebugConsole::RestoreWindow() {
    std::cout << "[INF] Restore a window from the global list:\n";
    std::vector<WindowInfo> windows = FUNC::GetOpenWindows();  // get all windows
    size_t idx = 0;

    // List the windows
    for (const auto& win : windows) {
        DebugConsole::SetConsoleColor(3);
        std::cout << "[";
        DebugConsole::SetConsoleColor(10);
        std::cout << idx++;
        DebugConsole::SetConsoleColor(3);
        std::cout << "] ";
        DebugConsole::SetConsoleColor(7);
        std::cout << win.title;
        if (win.isVisible) {
            DebugConsole::SetConsoleColor(2);
            std::cout << " (Visible)";
        }
        else {
            DebugConsole::SetConsoleColor(3);
            std::cout << " (Hidden)";
        }
        DebugConsole::SetConsoleColor(8);
        std::cout << " (HWND: " << win.hwnd << ")\n";
        DebugConsole::SetConsoleColor(7);
    }

    // ask the user to select a window to restore
    std::cout << "Enter the number of the window you want to restore: ";
    std::string windowIndexStr;
    std::getline(std::cin, windowIndexStr);
    if (FUNC::is_digits(windowIndexStr)) {
        int windowIndex = std::stoi(windowIndexStr);

        if (windowIndex >= 0 && windowIndex < windows.size()) {
            HWND hwnd = windows[windowIndex].hwnd;
            FUNC::RestoreWindow(hwnd);  // function to forcefully restore the window (unhide it) if the program was closed
            DebugConsole::SetConsoleColor(10);
            std::cout << "[INF] Window restored.\n";
            DebugConsole::SetConsoleColor(7);
        }
        else {
            DebugConsole::SetConsoleColor(10);
            std::cout << "[ERR] Invalid selection.\n";
            DebugConsole::SetConsoleColor(7);
        }
    }
    else {
        DebugConsole::SetConsoleColor(12);
        std::cout << "[ERR] Invalid input. Input may only consist of numbers. Use the numbers located on the right of given window list.\n";
        DebugConsole::SetConsoleColor(7);
    }
}


// scuffed but works
void DebugConsole::HelpFunction() {
    DebugConsole::SetConsoleColor(14);
    std::cout << "[ALL COMMANDS ARE CASE SENSETIVE]\n\n";
    DebugConsole::SetConsoleColor(3);
    std::cout << "[";
    DebugConsole::SetConsoleColor(10);
    std::cout << "1";
    DebugConsole::SetConsoleColor(3);
    std::cout << "]";
    DebugConsole::SetConsoleColor(11);
    std::cout << " help command:\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'help'";
    DebugConsole::SetConsoleColor(10);
    std::cout << " - shows all commands, and explains what they do.\n\n";
    DebugConsole::SetConsoleColor(7);
    DebugConsole::SetConsoleColor(3);
    std::cout << "[";
    DebugConsole::SetConsoleColor(10);
    std::cout << "2";
    DebugConsole::SetConsoleColor(3);
    std::cout << "]";
    DebugConsole::SetConsoleColor(11);
    std::cout << " listing commands:\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'list'";
    DebugConsole::SetConsoleColor(10);
    std::cout << " - shows all currently detectable windows, that have titles.\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'list -a'";
    DebugConsole::SetConsoleColor(6);
    std::cout << " or ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'list -all'";
    DebugConsole::SetConsoleColor(10);
    std::cout << " - shows all currently detectable windows, including overlays, system windows, etc.\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'list -h'";
    DebugConsole::SetConsoleColor(6);
    std::cout << " or ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'list -hidden'";
    DebugConsole::SetConsoleColor(10);
    std::cout << " - shows all windows, hidden by the user/TrayMinimizer. Windows hidden by the operating system will not be listed.\n\n";
    DebugConsole::SetConsoleColor(3);
    std::cout << "[";
    DebugConsole::SetConsoleColor(10);
    std::cout << "3";
    DebugConsole::SetConsoleColor(3);
    std::cout << "]";
    DebugConsole::SetConsoleColor(11);
    std::cout << " controls:\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'hide'";
    DebugConsole::SetConsoleColor(10);
    std::cout << " - hides a window and removes it from the taskbar.\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'show'";
    DebugConsole::SetConsoleColor(10);
    std::cout << " - restores a previously hidden window.\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'restore'";
    DebugConsole::SetConsoleColor(10);
    std::cout << " - forcefully shows the window, and puts it on in front of the rest.";
    DebugConsole::SetConsoleColor(14);
    std::cout << " Use if you want to recover a window from a previous session.\n";
    std::cout << "'exit'";
    DebugConsole::SetConsoleColor(10);
    std::cout << " - closes the console, but keeps the background process running.\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'quit'";
    DebugConsole::SetConsoleColor(10);
    std::cout << " - terminates all operations and exits the application.";
    DebugConsole::SetConsoleColor(12);
    std::cout << " Show all windows, before executing. Use recover function if you forgot to.\n";
    DebugConsole::SetConsoleColor(7);
}

void DebugConsole::Hide() {
    HWND hwnd = GetConsoleWindow();
    FUNC::HideWindow(hwnd);
    hiddenWindows.push_back(hwnd);  // store hidden window
}

void DebugConsole::Quit() {
    std::cout << "Terminating Program, goodbye!\n";
    consoleAlive = false;
    HWND hConsole = GetConsoleWindow();
    if (hConsole) {
        // Send a WM_CLOSE message to the console window.
        SendMessage(hConsole, WM_CLOSE, 0, 0);
    }
    FreeConsole();
    exit(0);
}
