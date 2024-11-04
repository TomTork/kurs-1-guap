#ifndef SQLCONTROLLER_H
#define SQLCONTROLLER_H
#include <cstdio>
#include "sqlite3.h"
#include <string>
#include <vector>

using namespace std;

struct Order {
    int id;
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

struct Courier {
    int id;
    string fio;
    int x;
    int y;
    string transport;
    int speed;
    double finalTime = 0;
    double localTime = 0;
};

struct SimpleId {
    int id;
    string fio;
};

struct Warehouse {
    int id;
    string name;
    int x;
    int y;
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
     * Обновление координат Курьера: id, x, y
     */
    void updateCoordsCourier(const int&, const int&, const int&);

    /**
    * Создание новой записи Клиента: fio, num, x, y
    */
    void insertIntoClients(const string&, const string&, const int&, const int&);

    /**
     * Обновить статус Заказа: id, status
     * @deprecated Метод не используется
     */
    void updateStatusOrders(const int&, const bool&);

    /**
     * Создание новой записи Заказа: client_id, orders, status, weight
     */
    void insertOrder(const int&, const string&, const bool&, const int&);

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
    vector<SimpleId> getDataClients();

    /**
     * Вернуть массив Курьеров
     * @return Courier[]
     */
    vector<Courier> getDataCouriers();

    /**
     * Вернуть массив Складов
     * @return Warehouse[]
     */
    vector<Warehouse> getDataWarehouses();

    /**
     * Удалить все данные из таблиц
     */
    void dropAllTables();

    vector<Client> getClientsWithOrders();

    void closeDB() const;
};

#endif //SQLCONTROLLER_H
