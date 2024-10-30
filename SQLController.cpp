#include "SQLController.h"
#include <cstdio>
#include "sqlite3.h"
#include <string>
#include <format>
#include <iostream>
#include <algorithm>

using namespace std;

void SQLController::createTable() {
    const auto createCouriers = "CREATE TABLE IF NOT EXISTS Couriers(ID INTEGER PRIMARY KEY AUTOINCREMENT, fio TEXT, x INTEGER, y INTEGER, transport TEXT, speed INTEGER);";
    const auto createClients = "CREATE TABLE IF NOT EXISTS Clients(ID INTEGER PRIMARY KEY AUTOINCREMENT, fio TEXT, num TEXT, x INTEGER, y INTEGER);";
    const auto createOrders = "CREATE TABLE IF NOT EXISTS Orders(ID INTEGER PRIMARY KEY AUTOINCREMENT, client_id INTEGER, orders TEXT, status BOOLEAN, weight INTEGER, FOREIGN KEY (client_id) REFERENCES Clients (ID) ON DELETE CASCADE);";
    if (this->exit = sqlite3_exec(this->db, createCouriers, nullptr, nullptr, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::createTable " << this->err << endl;
        sqlite3_free(this->err);
    }
    if (this->exit = sqlite3_exec(this->db, createClients, nullptr, nullptr, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::createTable " << this->err << endl;
        sqlite3_free(this->err);
    }
    if (this->exit = sqlite3_exec(this->db, createOrders, nullptr, nullptr, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::createTable " << this->err << endl;
        sqlite3_free(this->err);
    }
}

SQLController::SQLController() {
    if (sqlite3_open("C:/Users/dima2/CLionProjects/opt/database.sqlite", &this->db)) {
        fprintf(stderr, "ERROR: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_exec(this->db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, &this->err);
    createTable();
}

SQLController::~SQLController() {
    this->closeDB();
}


void SQLController::closeDB() const {
    sqlite3_close(this->db);
}

void SQLController::insertIntoCouriers(const string &fio, const int& x, const int& y, const string &transport, const int &speed) {
    const string sql = format(R"(INSERT INTO Couriers(fio, x, y, transport, speed) VALUES("{}", {}, {}, "{}", {});)", fio, x, y, transport, speed);
    this->exit = sqlite3_exec(this->db, sql.c_str(), nullptr, nullptr, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::insertIntoCouriers " << this->err << endl;
        sqlite3_free(this->err);
    }
}

int SQLController::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    cout << "-------------------" << endl;
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << "-------------------" << endl;
    cout << endl;
    return 0;
}

void SQLController::getAllCouriers() {
    const string sql = "SELECT * FROM Couriers;";
    this->exit = sqlite3_exec(this->db, sql.c_str(), callback, nullptr, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::getAllCouriers " << this->err << endl;
        sqlite3_free(this->err);
    }
}

void SQLController::dropAllTables() {
    const auto deleteCouriers = "DROP TABLE Couriers;";
    if (this->exit = sqlite3_exec(this->db, deleteCouriers, nullptr, nullptr, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::dropAllTables " << this->err << endl;
        sqlite3_free(this->err);
    }
    const auto deleteOrders = "DROP TABLE Orders;";
    if (this->exit = sqlite3_exec(this->db, deleteOrders, nullptr, nullptr, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::dropAllTables " << this->err << endl;
        sqlite3_free(this->err);
    }
    const auto deleteClients = "DROP TABLE Clients;";
    if (this->exit = sqlite3_exec(this->db, deleteClients, nullptr, nullptr, &this->err); this->exit != SQLITE_OK) {
        cerr << "ERROR ::dropAllTables " << this->err << endl;
        sqlite3_free(this->err);
    }
}

void SQLController::getAllClients() {
    const string sql = "SELECT * FROM Clients;";
    this->exit = sqlite3_exec(this->db, sql.c_str(), callback, nullptr, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::getAllClients " << this->err << endl;
        sqlite3_free(this->err);
    }
}

void SQLController::insertIntoClients(const string &fio, const string &num, const int& x, const int& y) {
    const string sql = format(R"(INSERT INTO Clients(fio, num, x, y) VALUES("{}", "{}", {}, {});)", fio, num, x, y);
    this->exit = sqlite3_exec(this->db, sql.c_str(), nullptr, nullptr, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::insertIntoClients " << this->err << endl;
        sqlite3_free(this->err);
    }
}

vector<Client> SQLController::getClientsWithOrders() {
    vector<Client> clients;
    const string sql = "SELECT Clients.ID, Clients.fio, Clients.num, Clients.x, Clients.y, Orders.ID, Orders.orders, Orders.status, Orders.weight FROM Clients LEFT JOIN Orders ON Clients.ID = Orders.client_id;";
    this->exit = sqlite3_exec(this->db, sql.c_str(), [](void* data, int argc, char** argv, char** azColName) {
        auto* inClients = static_cast<vector<Client>*>(data);

        // Получаем информацию о клиенте
        const int clientId = stoi(argv[0]);
        const string fio = argv[1] ? argv[1] : "NULL";
        const string num = argv[2] ? argv[2] : "NULL";
        const int x = argv[3] ? stoi(argv[3]) : -1;
        const int y = argv[4] ? stoi(argv[4]) : -1;

        // Проверяем, существует ли клиент в векторе
        auto it = ranges::find_if(*inClients,
                                  [clientId](const Client& client) { return client.id == clientId; });

        // Если клиент не найден, добавляем его
        if (it == inClients->end()) {
            inClients->emplace_back(Client{ clientId, fio, num, x, y, {} });
            it = prev(inClients->end()); // Указатель на нового клиента
        }

        // Добавляем заказ, если он существует
        if (argv[4]) { // Если заказ существует
            const Order order{
                stoi(argv[5]),
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


int callback2(void* data, int argc, char** argv, char** azColName) {
    auto* clients = static_cast<vector<SimpleId>*>(data);

    SimpleId client;
    client.id = argv[0] ? stoi(argv[0]) : 0;
    client.fio = argv[1] ? argv[1] : "NULL";

    clients->push_back(client);
    return 0;
}

vector<SimpleId> SQLController::getDataClients() {
    vector<SimpleId> clients;
    const string sql = "SELECT ID, fio FROM Clients;";

    this->exit = sqlite3_exec(this->db, sql.c_str(), callback2, &clients, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::getDataClients " << this->err << endl;
        sqlite3_free(this->err);
    }
    return clients;
}

void SQLController::insertOrder(const int& clientId, const string& orders, const bool& status, const int& weight) {
    const string sql = "INSERT INTO Orders (client_id, orders, status, weight) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    this->exit = sqlite3_prepare_v2(this->db, sql.c_str(), -1, &stmt, nullptr);
    if (this->exit != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(this->db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, clientId);
    sqlite3_bind_text(stmt, 2, orders.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, status ? 1 : 0);
    sqlite3_bind_int(stmt, 4, weight);

    this->exit = sqlite3_step(stmt);
    if (this->exit != SQLITE_DONE) {
        cerr << "Error executing statement: " << sqlite3_errmsg(this->db) << endl;
    }
    sqlite3_finalize(stmt);
}

void SQLController::updateCoordsCourier(const int& id, const int& x, const int& y) {
    const string sql = "UPDATE Couriers SET x=" + to_string(x) + ", y=" + to_string(y) + " WHERE id=" + to_string(id) + ";";
    this->exit = sqlite3_exec(this->db, sql.c_str(), nullptr, nullptr, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::updateCoordsCourier: " << this->err << endl;
        sqlite3_free(this->err);
    }
}

void SQLController::updateStatusOrders(const int& id, const bool& status) {
    const string sql = "UPDATE Orders SET status=" + to_string(status) + " WHERE id=" + to_string(id)  + ";";
    this->exit = sqlite3_exec(this->db, sql.c_str(), nullptr, nullptr, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::updateStatusOrders: " << this->err << endl;
        sqlite3_free(this->err);
    }
}

int callback3(void* data, int argc, char** argv, char** azColName) {
    auto* couriers = static_cast<vector<Courier>*>(data);

    Courier courier;
    courier.id = argv[0] ? stoi(argv[0]) : 0;
    courier.fio = argv[1] ? argv[1] : "NULL";
    courier.x = argv[2] ? stoi(argv[2]) : 0;
    courier.y = argv[3] ? stoi(argv[3]) : 0;
    courier.transport = argv[4] ? argv[4] : "NULL";
    courier.speed = argv[5] ? stoi(argv[5]) : 0;

    couriers->push_back(courier);
    return 0;
}

vector<Courier> SQLController::getDataCouriers() {
    vector<Courier> couriers;
    const string sql = "SELECT id, fio, x, y, transport, speed FROM Couriers;";

    this->exit = sqlite3_exec(this->db, sql.c_str(), callback3, &couriers, &this->err);
    if (this->exit != SQLITE_OK) {
        cerr << "ERROR ::getDataCouriers: " << this->err << endl;
        sqlite3_free(this->err);
    }
    return couriers;
}