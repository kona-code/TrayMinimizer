#pragma once

#include <string>
#include <iostream>
#include <thread>

class DebugConsole {
public:
    DebugConsole();
    ~DebugConsole();

    // show the console and start the command loop in a separate thread
    void Show();

    // hide (or close) the console
    void Hide();

private:
    // function that runs the command loop
    void RunCommandLoop();
};
