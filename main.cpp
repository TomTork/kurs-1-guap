#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <conio.h>
#include "SQLController.h"
#include "helper.h"

using namespace std;

int main() {
    SQLController controller;
    vector<Client> clients;
    string command, _num, _fio, _location, _transport, _speed;
    int _x, _y, finalSpeed;
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
                cout << options[i] << " <-" << endl;
            } else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << options[i] << endl;
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
            switch (selected) {
                case 0: // Output all couriers
                    controller.getAllCouriers();
                    break;
                case 1: // Output all clients
                    controller.getAllClients();
                    break;
                case 2: // Output all orders
                    controller.getClientsWithOrders();
                    break;
                case 3: // Add a new courier
                    cout << "---ADD A NEW COURIER---" << endl;
                    cout << "Input FIO:\t";
                    while (true) {
                        getline(cin, _fio);
                        if (const auto [fst, snd] = checkStringState(_fio, true); fst) {
                            _fio = snd;
                            break;
                        }
                        cerr << "Error in input, please try again!" << endl;
                    }
                    cout << "\nInput location in format (x, y):\t";
                    while (true) {
                        getline(cin, _location);
                        const tuple<bool, int, int> cLocation = checkLocation(_location);
                        if (_location.contains(" ") && get<0>(cLocation)) {
                            _x = get<1>(cLocation);
                            _y = get<2>(cLocation);
                            break;
                        }
                        if (!get<0>(cLocation)) cerr << "Error in input, please try again!" << endl;
                    }
                    cout << "\nInput transport:\t";
                    while (true) {
                        getline(cin, _transport);
                        if (const auto [fst, snd] = checkStringState(_transport, true); fst) {
                            _transport = snd;
                            break;
                        }
                        cerr << "Error in input, please try again!" << endl;
                    }
                    cout << "\nInput speed:\t";
                    while (true) {
                        try {
                            getline(cin, _speed);
                            finalSpeed = stoi(_speed);
                            break;
                        } catch (exception &e) {
                            cerr << e.what() << endl;
                        }
                    }
                    controller.insertIntoCouriers(_fio, _x, _y, _transport, finalSpeed);
                    break;
                case 4: // Add a new client
                    cout << "---ADD A NEW CLIENT---" << endl;
                    cout << "Input FIO:\t";
                    while (true) {
                        getline(cin, _fio);
                        if (const auto [fst, snd] = checkStringState(_fio, true); fst) {
                            _fio = snd;
                            break;
                        }
                        cerr << "Error in input, please try again!" << endl;
                    }
                    cout << "Input phone number:\t";
                    while (true) {
                        getline(cin, _num);
                        if (const auto [fst, snd] = checkPhoneNumber(_num); fst) {
                            _num = snd;
                            break;
                        }
                        cerr << "Error in input, please try again!" << endl;
                    }
                    cout << "\nInput location in format (x, y):\t";
                    while (true) {
                        getline(cin, _location);
                        const tuple<bool, int, int> cLocation = checkLocation(_location);
                        if (_location.contains(" ") && get<0>(cLocation)) {
                            _x = get<1>(cLocation);
                            _y = get<2>(cLocation);
                            break;
                        }
                        if (!get<0>(cLocation)) cerr << "Error in input, please try again!" << endl;
                    }
                    controller.insertIntoClients(_fio, _num, _x, _y);
                    break;
                case 5: // Add a new order
                    cout << "Input ";

                    clients = controller.getClientsWithOrders();
                    cout << "Input id in outputs clients in format: { id - fio }" << endl;
                    for (const auto&[id, fio, num, x, y, orders] : clients) {
                        cout << "ID: " << id << " FIO: " << fio << endl;
                    }
                    break;
                default:
                    break;
            }
            system("pause");
        }
    }
    return 0;
}
