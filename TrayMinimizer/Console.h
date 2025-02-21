#pragma once
#include <string>
#include <map>
#include <iostream>
#include <thread>
#include <windows.h>
#include <cstdio>

class DebugConsole {
public:
    DebugConsole();
    ~DebugConsole();

    // show the console and start the command loop in a separate thread
    void Show();

    // hide (or close) the console
    void Hide();

    void Start() {
        std::string input;
        while (true) {
            std::cout << "> "; // Display a prompt
            std::getline(std::cin, input); // Get user input

            // Parse the command
            if (input == "exit") {
                break; // Exit the loop
            }

            ParseCommand(input); // Call method to parse and handle subcommands
        }
    }

    void SetConsoleColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }


private:
    // function that runs the command loop
    void RunCommandLoop();

    void ParseCommand(const std::string& command);


    void ListWindows();
    void ListAllWindows();

    void Exit();

    void DebugMode() {
        std::cout << "Entering debug mode..." << std::endl;
        // Add the debug mode logic here
    }
};
