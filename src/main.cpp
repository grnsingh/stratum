#include "Window.h"

int main() {
    Window appWindow(1280, 720, "Stratum Debug");

    //Initialize
    if (!appWindow.init()) {
        return -1;
    }

    //The Render Loop
    while (appWindow.isOpen()) {
        appWindow.clear();
        
        appWindow.update();
    }

    return 0;
}