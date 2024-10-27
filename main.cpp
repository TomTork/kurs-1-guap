#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <conio.h>
#include "SQLController.h"

using namespace std;

int main() {
    const SQLController controller;
    string command;
    const vector<string> options = {
        "Output all couriers", "Output all clients", "Output all orders",
        "Add a new courier", "Add a new client", "Add a new order",
        "Optimize", "Exit"
    };
    unsigned long long selected = 0;

    while (true) {
        system("cls");

        for (int i = 0; i < options.size(); ++i) {
            if (i == selected) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                std::cout << "\t> " << options[i] << " <" << std::endl;
            } else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                std::cout << "\t" << options[i] << std::endl;
            }
        }

        if (const int key = _getch(); key == 72) {
            selected = (selected > 0) ? selected - 1 : options.size() - 1;
        } else if (key == 80) {
            selected = (selected + 1) % options.size();
        } else if (key == 13) {
            if (selected == options.size() - 1) { // Exit
                break;
            }
            cout << "..." << options[selected] << std::endl;
            switch (selected) {
                case 0:
                    cout << selected << " ... " << options[selected] << endl;
                    break;
                case 1:
                    cout << "..." << options[selected] << std::endl;

            }
            system("pause");
        }
    }
    return 0;
}
