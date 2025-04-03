#pragma once
#include <string>
#include <map>
#include <iostream>
#include <thread>
#include <vector>
#include <windows.h>

class DebugConsole {
public:
    DebugConsole();
    ~DebugConsole();

    // show the console and start the command loop in a separate thread
    void Show();

    void Start() {
        std::string input;
        while (true) {
            std::cout << "> "; // Display a prompt
            std::getline(std::cin, input); // Get user input

            // Parse the command
            if (input == "exit") {
                break; // Exit the loop
            }

        }
    }

    void SetConsoleColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void HideWindow();
    void ShowWindowAgain();
    void RestoreWindow();
    std::vector<HWND> hiddenWindows;
    void ShowFromTray();
    void HideFromTray();
    void RunCommandLoop();
	std::string RunCommand(std::string command); // meant for command line arguments (being called thru CMD)

private:
    // function that runs the command loop
    bool consoleAlive;

    void ClearConsole();
    void HelpFunction(); 
    void ListWindows(const std::string& option);
    void Quit();

};