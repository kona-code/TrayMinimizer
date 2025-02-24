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
    DebugConsole::SetConsoleColor(15);
    std::cout << "@@@@@@@@*.                   =@@@@@@@@- \n@@-::::-#@*.               =@%=:::::%@=:\n@@-:     :#@*.           =@%=::.    #@=:\n@@=:       :#@*.       =@%=::.      %@=:\n.*@#:        :#@*.   =@%=::.      +@#-::\n  .*@#:        :#@- %%=::.      +@#-::. \n    .*@#:         .:  :.      +@#-::.   \n      .*@#:                 +@#-::.     \n        .*@*              -@#-::.       \n           ::.             .::.         \n         =@#              =@*.          \n       =@%=::.             :#@*.        \n     =@%=::.                 :#@*.      \n   =@%=::.      +%: #*:        :#@*.    \n =@%=::.      +@#-::.*@#:        :#@*.  \n@@=::.      +@#-::.   .*@#:        :%@- \n@@-:      +@#-::.       .*@#:       #@=:\n@@-:    +@#-::.           .*@#:     #@=:\n@@@@%%%@#-::.               .*@%%%%%@@=:\n.:------::.                    :-------:\n\n";
    std::cout << "      (C) NightVoid Entertainment\n";
    DebugConsole::SetConsoleColor(3);
    std::cout << "         https://nightvoid.com/\n";
    DebugConsole::SetConsoleColor(7);
    std::cout << "              By Kona Code\n\n";
    DebugConsole::SetConsoleColor(7);
    std::cout << "Copyright Policy: ";
    DebugConsole::SetConsoleColor(3);
    std::cout << "https://nightvoid.com/copyright\n";
    DebugConsole::SetConsoleColor(7);
    std::cout << "Terms of Service: ";
    DebugConsole::SetConsoleColor(3);
    std::cout << "https://nightvoid.com/terms-of-service\n\n";
    DebugConsole::SetConsoleColor(7);
    std::cout << "[INF] Console initialized.\n";
    std::thread t(&DebugConsole::RunCommandLoop, this);
    t.detach();
}

void DebugConsole::RunCommandLoop() {
    std::string input;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "help" || input == "man" || input == "manual") {
            HelpFunction();
        }
        else if (input == "list" || input == "l") {
            ListWindows("");
        }
        else if (input == "list -all" || input == "list -a" || input == "la") {
            ListWindows("-all");
        }
        else if (input == "list -hidden" || input == "list -h" || input == "lh") {
            ListWindows("-hidden");
        }
        else if (input == "hide" || input == "h") {
            HideWindow();
        }
        else if (input == "show" || input == "s") {
            ShowWindowAgain();
        }
        else if (input == "restore" || input == "r") {
            RestoreWindow();
        }
        else if (input == "about" || input == "github" || input == "code") {
            ShellExecute(NULL, L"open", L"https://github.com/kona-code/TrayMinimizer", NULL, NULL, SW_SHOWNORMAL);
        }
        else if (input == "author" || input == "publisher" || input == "kona") {
            ShellExecute(NULL, L"open", L"https://github.com/kona-code/", NULL, NULL, SW_SHOWNORMAL);
        }
        else if (input == "update") {
            ShellExecute(NULL, L"open", L"https://software.nightvoid.com/stable/#tray-minimizer", NULL, NULL, SW_SHOWNORMAL);
        }
        else if (input == "ver" || input == "version" || input == "v") {
            std::cout << "Currently running version 1.0\n";
        }
        else if (input == "hide -console" || input == "exit" || input == "hc") {
            HideFromTray();
        }
        else if (input == "terminate" || input == "quit") {
            DebugConsole::Quit();
        }
        else if (input == "clear" || input == "clean" || input == "c") {
            ClearConsole();
        }
        else {
            DebugConsole::SetConsoleColor(12);
            std::cout << "[ERR] Unknown command: '" << input << "', use 'help' to show all available commands." << std::endl;
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

    std::cout << "\nEnter window number to hide: ";
    std::string windowIndexStr;
    std::getline(std::cin, windowIndexStr);
    if (FUNC::is_digits(windowIndexStr) && windowIndexStr.length() != 0) {
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

    std::cout << "\nEnter window number to restore: ";
    std::string input;
    std::getline(std::cin, input);
    if (FUNC::is_digits(input) && input.length() != 0) {
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
    std::cout << "\nEnter the number of the window you want to restore: ";
    std::string windowIndexStr;
    std::getline(std::cin, windowIndexStr);
    if (FUNC::is_digits(windowIndexStr) && windowIndexStr.length() != 0) {
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
    DebugConsole::SetConsoleColor(3);
    std::cout << "[";
    DebugConsole::SetConsoleColor(10);
    std::cout << "1";
    DebugConsole::SetConsoleColor(3);
    std::cout << "]";
    std::cout << ">--------------------------------------------";
    DebugConsole::SetConsoleColor(11);
    std::cout << "< help command >";
    DebugConsole::SetConsoleColor(3);
    std::cout << "--------------------------------------------<\n\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "  'help'";
    DebugConsole::SetConsoleColor(10);
    std::cout << "\n - shows all commands, and explains what they do.\n";
    DebugConsole::SetConsoleColor(6);
    std::cout << "   Note that all commands are case sensetive.\n\n";
    DebugConsole::SetConsoleColor(3);
    std::cout << "[";
    DebugConsole::SetConsoleColor(10);
    std::cout << "2";
    DebugConsole::SetConsoleColor(3);
    std::cout << "]";
    std::cout << ">------------------------------------------";
    DebugConsole::SetConsoleColor(11);
    std::cout << "< listing commands >";
    DebugConsole::SetConsoleColor(3);
    std::cout << "------------------------------------------<\n\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "  'list'";
    DebugConsole::SetConsoleColor(6);
    std::cout << " or ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'l'";
    DebugConsole::SetConsoleColor(10);
    std::cout << "\n - shows all detected windows visible on the desktop (including minimized windows).\n\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "  'list -all'";
    DebugConsole::SetConsoleColor(6);
    std::cout << ", ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'list -a'";
    DebugConsole::SetConsoleColor(6);
    std::cout << " or ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'la'";
    DebugConsole::SetConsoleColor(10);
    std::cout << "\n - shows all detected windows, including hidden overlays, system windows, etc.\n\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "  'list -hidden'";
    DebugConsole::SetConsoleColor(6);
    std::cout << ", ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'list -h'";
    DebugConsole::SetConsoleColor(6);
    std::cout << " or ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'lh'";
    DebugConsole::SetConsoleColor(10);
    std::cout << "\n - shows all windows, hidden by the user/TrayMinimizer in the current session.\n   Windows hidden by the operating system will not be listed.\n\n";
    DebugConsole::SetConsoleColor(3);
    std::cout << "[";
    DebugConsole::SetConsoleColor(10);
    std::cout << "3";
    DebugConsole::SetConsoleColor(3);
    std::cout << "]";
    std::cout << ">----------------------------------------------";
    DebugConsole::SetConsoleColor(11);
    std::cout << "< controls >";
    DebugConsole::SetConsoleColor(3);
    std::cout << "----------------------------------------------<\n\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "  'hide'";
    DebugConsole::SetConsoleColor(6);
    std::cout << " or ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'h'";
    DebugConsole::SetConsoleColor(10);
    std::cout << "\n - hides a window and keeps it running in the background.\n\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "  'show'";
    DebugConsole::SetConsoleColor(6);
    std::cout << " or ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'s'";
    DebugConsole::SetConsoleColor(10);
    std::cout << "\n - restores a previously hidden window.\n   You are able to recover windows hidden only in the current session.\n   Use 'restore' if your window is not listed or was hidden in a previous session.\n\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "  'restore'";
    DebugConsole::SetConsoleColor(6);
    std::cout << " or ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'r'";
    DebugConsole::SetConsoleColor(10);
    std::cout << "\n - forcefully shows any window, and puts it on in front of the rest.\n";
    DebugConsole::SetConsoleColor(6);
    std::cout << "   Use if you want to recover a window from a previous session.\n";
    std::cout << "   You can narrow down you search by using shortcut 'CTRL + F' and entering you window title.\n\n";
    std::cout << "  'clear'";
    DebugConsole::SetConsoleColor(6);
    std::cout << ", ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'clean'";
    DebugConsole::SetConsoleColor(6);
    std::cout << " or ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'c'";
    DebugConsole::SetConsoleColor(10);
    std::cout << "\n - clears all current text and issued commands from the console.\n\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "  'terminate'";
    DebugConsole::SetConsoleColor(6);
    std::cout << " or ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'quit'";
    DebugConsole::SetConsoleColor(10);
    std::cout << "\n - terminates all operations and exits the application.\n";
    DebugConsole::SetConsoleColor(12);
    std::cout << "   Show all windows, before executing. Use recover function if you forgot to.\n\n\n";
    DebugConsole::SetConsoleColor(14);
    std::cout << "  'exit'";
    DebugConsole::SetConsoleColor(6);
    std::cout << ", ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'hide -console'";
    DebugConsole::SetConsoleColor(6);
    std::cout << " or ";
    DebugConsole::SetConsoleColor(14);
    std::cout << "'hc'";
    DebugConsole::SetConsoleColor(10);
    std::cout << "\n - hides the console. You can enable/show it again using the tray icon.\n\n";
    DebugConsole::SetConsoleColor(7);
}

void DebugConsole::ClearConsole() {
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get the console screen buffer info
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    // Set the console cursor position to the top left
    COORD topLeft = { 0, 0 };
    SetConsoleCursorPosition(hConsole, topLeft);

    // Fill the screen with spaces (clear screen)
    DWORD dwWritten;
    FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X * csbi.dwSize.Y, topLeft, &dwWritten);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, topLeft, &dwWritten);

    // Reset the cursor position
    SetConsoleCursorPosition(hConsole, topLeft);
}

void DebugConsole::HideFromTray() {
    HWND hwnd = GetConsoleWindow();
    FUNC::HideWindow(hwnd);
}

void DebugConsole::ShowFromTray() {
    HWND hwnd = GetConsoleWindow();
    FUNC::ShowWindowAgain(hwnd);
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
