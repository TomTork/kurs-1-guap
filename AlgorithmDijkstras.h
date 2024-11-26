#ifndef ALGORITHMDIJKSTRAS_H
#define ALGORITHMDIJKSTRAS_H
#include <vector>
#include "SQLController.h"

using namespace std;

struct Processing {
    Courier courier;
    Client client;
    double time = 0;
};

class AlgorithmDijkstras {
private:
    vector<Client> _clients;
    vector<Courier> _couriers;

    /**
     * Компаратор для сортировки
     */
    static bool compareByTime(const Processing&, const Processing&);

    static vector<Processing> calculate(const vector<Courier>&, const vector<Client>&);

    static string generateCommand(const Courier&, const Client&);
public:
    /**
     * Основной алгоритм Дейкстры
     * @param clients
     * @param couriers
     */
    AlgorithmDijkstras(const vector<Client>& clients, const vector<Courier>& couriers) : _clients(clients), _couriers(couriers){}

    vector<string> process();
};

#endif //ALGORITHMDIJKSTRAS_H
