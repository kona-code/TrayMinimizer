#include "Console.h"

DebugConsole console;

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
    // copyright
    std::cout << "@@@@@@@@*.                   =@@@@@@@@- \n@@-::::-#@*.               =@%=:::::%@=:\n@@-:     :#@*.           =@%=::.    #@=:\n@@=:       :#@*.       =@%=::.      %@=:\n.*@#:        :#@*.   =@%=::.      +@#-::\n  .*@#:        :#@- %%=::.      +@#-::. \n    .*@#:         .:  :.      +@#-::.   \n      .*@#:                 +@#-::.     \n        .*@*              -@#-::.       \n           ::.             .::.         \n         =@#              =@*.          \n       =@%=::.             :#@*.        \n     =@%=::.                 :#@*.      \n   =@%=::.      +%: #*:        :#@*.    \n =@%=::.      +@#-::.*@#:        :#@*.  \n@@=::.      +@#-::.   .*@#:        :%@- \n@@-:      +@#-::.       .*@#:       #@=:\n@@-:    +@#-::.           .*@#:     #@=:\n@@@@%%%@#-::.               .*@%%%%%@@=:\n.:------::.                    :-------:\n\n";
    std::cout << "      (C) NightVoid Entertainment\n";
    std::cout << "         https://nightvoid.com/\n";
    std::cout << "              By Kona Code\n\n\n";
    std::cout << "Copyright Policy: https://nightvoid.com/copyright\n";
    std::cout << "Terms of Service: https://nightvoid.com/terms-of-service\n\n";
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
            console.SetConsoleColor(7);
            wprintf(L"[INF] Window: %s\n", windowTitle);  // log the title
        }
        else {
            console.SetConsoleColor(14);
            wprintf(L"[WRN] Window with no title detected.\n");  // if no title is found
            console.SetConsoleColor(7);
        }
    }
    return TRUE;  // continue enumerating windows
}
BOOL CALLBACK EnumWindowsProcWExs(HWND hwnd, LPARAM lParam) {
    if (IsWindowVisible(hwnd)) {
        wchar_t windowTitle[256];
        int length = GetWindowTextW(hwnd, windowTitle, sizeof(windowTitle) / sizeof(wchar_t));
        if (length > 0) {
            wprintf(L"[INF] Window: %s\n", windowTitle);  // log the title
        }
    }
    return TRUE;  // continue enumerating windows
}

void ListOpenWindows() {
    std::cout << EnumWindows(EnumWindowsProc, 0);
}



void DebugConsole::ParseCommand(const std::string& command) {
    size_t spacePos = command.find(' ');
    std::string cmd = command.substr(0, spacePos);
    std::string args = (spacePos != std::string::npos) ? command.substr(spacePos + 1) : "";


    if (cmd == "help") {
        std::cout << "[INF] [1] 'list' - lists all current open windows (of applications)\n[INF] [2] 'exit' - exits the console\n";
    }
    else if (cmd == "list") {
            std::cout << EnumWindows(EnumWindowsProc, 0);
            ListOpenWindows();
        
    }
    else {
        std::cout << "[ERR] Unknown command: " << cmd << std::endl;
    }
}

void DebugConsole::RunCommandLoop() {
    std::string input;

    // Map to handle different commands
    std::map<std::string, void(DebugConsole::*)()> commandMap = {
        {"list", &DebugConsole::ListWindows},
        {"list -all", &DebugConsole::ListWindows},
        {"help", &DebugConsole::DebugMode},
        {"exit", &DebugConsole::Exit}
    };

    // Command loop
    while (true) {
        std::cout << "> ";  // Display a prompt
        std::getline(std::cin, input);  // Read user input

        // Find the command in the map and execute the corresponding function
        auto cmd = commandMap.find(input);
        if (cmd != commandMap.end()) {
            (this->*(cmd->second))();  // Call the corresponding function
        }
        else {
            std::cout << "[ERR] Unknown command: " << input << std::endl;
        }
    }
}

void DebugConsole::ListWindows() {
    std::cout << "Listing windows..." << std::endl;
    EnumWindows(EnumWindowsProc, 0);

}

void DebugConsole::ListAllWindows() {
    std::cout << "Listing all windows detected..." << std::endl;
    EnumWindows(EnumWindowsProcWExs, 0);

}
void DebugConsole::Exit() {
    std::cout << "Terminating Program...\n";
    exit(0);  // exit the program
}

