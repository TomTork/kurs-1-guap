#ifndef ALGORITHMDIJKSTRAS_H
#define ALGORITHMDIJKSTRAS_H
#include <vector>
#include "SQLController.h"

using namespace std;

struct Processing {
    Courier* courier{};
    Client* client{};
    double time = 0;
    double localTime = 0;
};

class AlgorithmDijkstras {
private:
    vector<Client> _clients;
    vector<Courier> _couriers;

    /**
     * Компаратор для сортировки
     */
    static bool compareByTime(const Processing&, const Processing&);
public:
    /**
     * Основной алгоритм Дейкстры
     * @param clients
     * @param couriers
     */
    AlgorithmDijkstras(const vector<Client>& clients, const vector<Courier>& couriers) : _clients(clients), _couriers(couriers){}

    vector<string> process(SQLController&);
};

#endif //ALGORITHMDIJKSTRAS_H
