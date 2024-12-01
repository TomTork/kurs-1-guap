#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <conio.h>
#include "SQLController.h"
#include "helper.h"
#include "AlgorithmDijkstras.h"

using namespace std;

int main() {
    SQLController controller;
    vector<Client> clients;
    vector<string> commands;
    vector<SimpleId> clientsIds;
    vector<Warehouse> warehouses;
    SimpleId clientId;
    Warehouse warehouse;
    string command, _num, _fio, _location, _transport, _speed, orders;
    int _x, _y, finalSpeed, weight;
    string inputWeight;
    bool choiceOrder, choiceWarehouse;
    const vector<string> options = {
        "Output all couriers", "Output all clients", "Output all orders",
        "Add a new courier", "Add a new client", "Add a new order",
        "Optimize", "Exit"
    };
    unsigned long long selected = 0, clientSelected = 0, warehouseSelected = 0;
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
                    try {
                        cout << endl;
                        controller.getAllCouriers();
                        cout << endl;
                    } catch (exception& e) {
                        cerr << "Table is empty! " << e.what() << endl;
                    }
                    break;
                case 1: // Output all clients
                    try {
                        cout << endl;
                        controller.getAllClients();
                        cout << endl;
                    } catch (exception& e) {
                        cerr << "Table is empty! " << e.what() << endl;
                    }
                    break;
                case 2: // Output all orders
                    try {
                        for (const auto& [id, fio, num, x, y, orders] : controller.getClientsWithOrders()) {
                            cout << endl;
                            cout << "For Client " << id << " -> FIO: " << fio << " | Phone number: " << num << " | X: " << x << " | Y: " << y << endl;
                            cout << "Orders:" << endl;
                            for (const auto& [id, orders, status, weight] : orders) {
                                cout << "ID: " << id << " | Order(s): " << orders << " | Status: " << status << " | Weight: " << weight << endl;
                            }
                        }
                        cout << endl;
                    } catch (exception& e) {
                        cerr << "Table is empty! " << e.what() << endl;
                    }
                    break;
                case 3: // Add a new courier
                    cout << "---ADD A NEW COURIER---" << endl;
                    cout << "Input FIO: ";
                    while (true) {
                        getline(cin, _fio);
                        if (const auto [fst, snd] = checkStringState(_fio, true); fst) {
                            _fio = snd;
                            break;
                        }
                        cerr << "Error in input, please try again!" << endl;
                    }
                    choiceWarehouse = true;
                    while (true) {
                        warehouses = controller.getDataWarehouses();
                        system("cls");
                        cout << "---LINK Courier and Warehouse---" << endl;
                        cout << "Use arrow keys to select a warehouse, press Enter to confirm, or select 'Exit' to go back:\n";
                        for (size_t i = 0; i < warehouses.size() + 1; ++i) {
                            if (i == warehouseSelected) {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                                if (i == warehouses.size()) {
                                    cout << "Exit <-\n";
                                } else {
                                    cout << "|ID:\t" << warehouses[i].id << " | NAME: " << warehouses[i].name
                                    << "\t | X:\t" << warehouses[i].x << "\t | Y:\t" << warehouses[i].y << " <-\n";
                                }
                            } else {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                                if (i == warehouses.size()) {
                                    cout << "Exit\n";
                                } else {
                                    cout << "|ID:\t" << warehouses[i].id << " | NAME: " << warehouses[i].name
                                    << "\t | X:\t" << warehouses[i].x << "\t | Y:\t" << warehouses[i].y << "\n";
                                }
                            }
                        }
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

                        if (const int _key = _getch(); _key == 72) {
                            warehouseSelected = warehouseSelected > 0 ? warehouseSelected - 1 : warehouses.size();
                        } else if (_key == 80) {
                            warehouseSelected = (warehouseSelected + 1) % (warehouses.size() + 1);
                        } else if (_key == 13) {
                            if (warehouseSelected == warehouses.size()) {
                                choiceWarehouse = false;
                                break;
                            }
                            warehouse = warehouses[warehouseSelected];
                            cout << "Selected warehouse ID: " << warehouses[warehouseSelected].id << endl;
                            break;
                        }
                    }
                    if (!choiceWarehouse) break;
                    cout << "Input transport: ";
                    while (true) {
                        getline(cin, _transport);
                        if (const auto [fst, snd] = checkStringState(_transport, true); fst) {
                            _transport = snd;
                            break;
                        }
                        cerr << "Error in input, please try again!" << endl;
                    }
                    cout << "Input speed: ";
                    while (true) {
                        try {
                            getline(cin, _speed);
                            finalSpeed = stoi(_speed);
                            break;
                        } catch (exception &e) {
                            cerr << "Error: " << e.what() << endl;
                        }
                    }
                    controller.insertIntoCouriers(_fio, warehouse.x, warehouse.y, _transport, finalSpeed);
                    break;
                case 4: // Add a new client
                    cout << "---ADD A NEW CLIENT---" << endl;
                    cout << "Input FIO: ";
                    while (true) {
                        getline(cin, _fio);
                        if (const auto [fst, snd] = checkStringState(_fio, true); fst) {
                            _fio = snd;
                            break;
                        }
                        cerr << "Error in input, please try again!" << endl;
                    }
                    cout << "Input phone number: ";
                    while (true) {
                        getline(cin, _num);
                        if (const auto [fst, snd] = checkPhoneNumber(_num); fst) {
                            _num = snd;
                            break;
                        }
                        cerr << "Error in input, please try again!" << endl;
                    }
                    cout << "Input location in format (x, y): ";
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
                    choiceOrder = true;
                    while (true) {
                        clientsIds = controller.getDataClients();
                        system("cls");
                        cout << "---ADD A NEW ORDER---" << endl;
                        cout << "Use arrow keys to select a client, press Enter to confirm, or select 'Exit' to go back:\n";
                        for (size_t i = 0; i < clientsIds.size() + 1; ++i) {
                            if (i == clientSelected) {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                                if (i == clientsIds.size()) {
                                    cout << "Exit <-\n";
                                } else {
                                    cout << "|ID:\t" << clientsIds[i].id << " | FIO:\t" << clientsIds[i].fio << " <-\n";
                                }
                            } else {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                                if (i == clientsIds.size()) {
                                    cout << "Exit\n";
                                } else {
                                    cout << "|ID:\t" << clientsIds[i].id << " | FIO:\t" << clientsIds[i].fio << "\n";
                                }
                            }
                        }
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

                        const int key = _getch();
                        if (key == 72) {
                            clientSelected = clientSelected > 0 ? clientSelected - 1 : clientsIds.size();
                        } else if (key == 80) {
                            clientSelected = (clientSelected + 1) % (clientsIds.size() + 1);
                        } else if (key == 13) {
                            if (clientSelected == clientsIds.size()) {
                                choiceOrder = false;
                                break;
                            }
                            clientId = clientsIds[clientSelected];
                            cout << "Selected client ID: " << clientsIds[clientSelected].id << endl;
                            break;
                        }
                    }
                    if (choiceOrder) {
                        cout << "Input your order(s): ";
                        cin >> orders;
                        cout << "Input weight of order(s): ";
                        while (true) {
                            try {
                                cin >> inputWeight;
                                weight = stoi(inputWeight);
                                break;
                            } catch (exception& e) {
                                cerr << "A non-integer number was entered. Error: " << e.what() << endl;
                            }
                        }
                        controller.insertOrder(clientId.id, orders, false, weight);
                    }
                    break;
                case 6: // Optimize
                    cout << endl;
                    commands = AlgorithmDijkstras(controller.getClientsWithOrders(), controller.getDataCouriers()).process();
                    for (const auto& com : commands) {
                        cout << com << endl;
                    }
                    cout << endl;
                    break;
                default:
                    break;
            }
            system("pause");
        }
    }
    return 0;
}
