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
    int x;
    int y;
    vector<Order> orders;
};

class SQLController final {
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
    SQLController();
    ~SQLController();

    /**
     * Создание новой записи Курьера: fio, x, y, transport, speed
     */
    void insertIntoCouriers(const string&, const int&, const int&, const string&, const int &);

    /**
    * Создание новой записи Клиента: fio, num, x, y
    */
    void insertIntoClients(const string&, const string&, const int&, const int&);

    /**
     * Вывод в консоль таблицы Курьеров
     */
    void getAllCouriers();

    /**
    * Вывод в консоль таблицы Клиентов
    */
    void getAllClients();

    /**
     * Вернуть массив клиентов
     * @return Client[]
     */
    vector<Client> getDataClients();

    /**
     * Удалить все данные из таблиц
     */
    void dropAllTables();

    vector<Client> getClientsWithOrders();

    void closeDB() const;
};

#endif //SQLCONTROLLER_H
