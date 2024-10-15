#include "SQLController.h"
#include <stdio.h>
#include "sqlite3.h"
#include <string>
#include <format>
#include <iostream>
#include <algorithm>

using namespace std;

void SQLController::createTable() {
    const auto createCouriers = "CREATE TABLE IF NOT EXISTS Couriers(ID INTEGER PRIMARY KEY AUTOINCREMENT, fio TEXT, location TEXT, transport TEXT, speed INTEGER);";
    const auto createClients = "CREATE TABLE IF NOT EXISTS Clients(ID INTEGER PRIMARY KEY AUTOINCREMENT, fio TEXT, num TEXT, location TEXT);";
    const auto createOrders = "CREATE TABLE IF NOT EXISTS Orders(ID INTEGER PRIMARY KEY AUTOINCREMENT, client_id INTEGER, time TEXT, orders TEXT, status BOOLEAN, weight INTEGER, FOREIGN KEY (client_id) REFERENCES Clients (ID) ON DELETE CASCADE);";
    if (this->exit = sqlite3_exec(this->db, createCouriers, 0, 0, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::createTable " << this->err << endl;
        sqlite3_free(this->err);
    }
    if (this->exit = sqlite3_exec(this->db, createClients, 0, 0, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::createTable " << this->err << endl;
        sqlite3_free(this->err);
    }
    if (this->exit = sqlite3_exec(this->db, createOrders, 0, 0, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::createTable " << this->err << endl;
        sqlite3_free(this->err);
    }
}

SQLController::SQLController() {
    if (sqlite3_open("C:/Users/dima2/CLionProjects/opt/database.sqlite", &this->db)) {
        fprintf(stderr, "ERROR: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_exec(this->db, "PRAGMA foreign_keys = ON;", 0, 0, &this->err);
    createTable();
}

SQLController::~SQLController() {
    this->closeDB();
}


void SQLController::closeDB() const {
    sqlite3_close(this->db);
}

void SQLController::insertIntoCouriers(const string &fio, const string &location, const string &transport, const int &speed) {
    const string sql = format(R"(INSERT INTO Couriers(fio, location, transport, speed) VALUES("{}", "{}", "{}", {});)", fio, location, transport, speed);
    this->exit = sqlite3_exec(this->db, sql.c_str(), 0, 0, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::insertIntoCouriers " << this->err << endl;
        sqlite3_free(this->err);
    }
}

int SQLController::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << "-------------------" << endl;
    return 0;
}

void SQLController::getAllCouriers() {
    const string sql = "SELECT * FROM Couriers;";
    this->exit = sqlite3_exec(this->db, sql.c_str(), callback, 0, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::getAllCouriers " << this->err << endl;
        sqlite3_free(this->err);
    }
}

void SQLController::dropAllTables() {
    const auto deleteCouriers = "DROP TABLE Couriers;";
    if (this->exit = sqlite3_exec(this->db, deleteCouriers, 0, 0, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::dropAllTables " << this->err << endl;
        sqlite3_free(this->err);
    }
    const auto deleteOrders = "DROP TABLE Orders;";
    if (this->exit = sqlite3_exec(this->db, deleteOrders, 0, 0, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::dropAllTables " << this->err << endl;
        sqlite3_free(this->err);
    }
    const auto deleteClients = "DROP TABLE Clients;";
    if (this->exit = sqlite3_exec(this->db, deleteClients, 0, 0, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::dropAllTables " << this->err << endl;
        sqlite3_free(this->err);
    }
}

void SQLController::getAllClients() {
    const string sql = "SELECT * FROM Clients;";
    this->exit = sqlite3_exec(this->db, sql.c_str(), callback, 0, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::getAllClients " << this->err << endl;
        sqlite3_free(this->err);
    }
}

void SQLController::insertIntoClients(const string &fio, const string &num, const string &location) {
    const string sql = format(R"(INSERT INTO Clients(fio, num, location) VALUES("{}", "{}", "{}");)", fio, num, location);
    this->exit = sqlite3_exec(this->db, sql.c_str(), 0, 0, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::insertIntoClients " << this->err << endl;
        sqlite3_free(this->err);
    }
}

vector<Client> SQLController::getClientsWithOrders() {
    vector<Client> clients;
    const string sql = "SELECT Clients.ID, Clients.fio, Clients.num, Clients.location, Orders.ID, Orders.time, Orders.orders, Orders.status, Orders.weight FROM Clients LEFT JOIN Orders ON Clients.ID = Orders.client_id;";
    this->exit = sqlite3_exec(this->db, sql.c_str(), [](void* data, int argc, char** argv, char** azColName) {
        auto* inClients = static_cast<vector<Client>*>(data);

        // Получаем информацию о клиенте
        int clientId = stoi(argv[0]);
        const string fio = argv[1] ? argv[1] : "NULL";
        const string num = argv[2] ? argv[2] : "NULL";
        const string location = argv[3] ? argv[3] : "NULL";

        // Проверяем, существует ли клиент в векторе
        auto it = ranges::find_if(*inClients,
                                  [clientId](const Client& client) { return client.id == clientId; });

        // Если клиент не найден, добавляем его
        if (it == inClients->end()) {
            inClients->emplace_back(Client{ clientId, fio, num, location, {} });
            it = prev(inClients->end()); // Указатель на нового клиента
        }

        // Добавляем заказ, если он существует
        if (argv[4]) { // Если заказ существует
            const Order order{
                stoi(argv[4]),
                argv[5] ? argv[5] : "",
                argv[6] ? argv[6] : "",
                argv[7] ? string(argv[7]) == "1" : false,
                argv[8] ? stoi(argv[8]) : 0
            };
            it->orders.push_back(order);
        }

        return 0;
    }, &clients, &this->err);

    if (exit != SQLITE_OK) {
        std::cerr << "ERROR ::getClientsWithOrders " << this->err << std::endl;
        sqlite3_free(this->err);
    }
    return clients;
}
