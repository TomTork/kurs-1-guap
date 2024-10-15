#ifndef SQLCONTROLLER_H
#define SQLCONTROLLER_H
#include <stdio.h>
#include "sqlite3.h"
#include <string>
#include <vector>

using namespace std;

struct Order {
    int id;
    string time;
    string orders;
    bool status;
    int weight;
};

struct Client {
    int id;
    string fio;
    string num;
    string location;
    vector<Order> orders;
};

class SQLController {
private:
    sqlite3 *db = nullptr;
    char *err = nullptr;
    int exit = 0;

    /**
     * Возвращает значения, полученные из `SELECT * FROM`
     * @return 0
     */
    static int callback(void*, int, char**, char**);

    /**
    * Создание всех таблиц, если их не существует
    */
    void createTable();
public:
    /**
     * Создание новой записи Курьера: fio, location, transport, speed
     */
    void insertIntoCouriers(const string &, const string &, const string &, const int &);

    /**
    * Создание новой записи Клиента: fio, num, location
    */
    void insertIntoClients(const string &, const string &, const string &);

    /**
     * Вывод в консоль таблицы Курьеров
     */
    void getAllCouriers();

    /**
    * Вывод в консоль таблицы Клиентов
    */
    void getAllClients();

    /**
     * Удалить все данные из таблиц
     */
    void dropAllTables();

    vector<Client> getClientsWithOrders();

    SQLController();
    ~SQLController();
    void closeDB() const;
};

#endif //SQLCONTROLLER_H
