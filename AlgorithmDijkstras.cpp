#include "AlgorithmDijkstras.h"
#include <algorithm>
#include <iostream>
#include "helper.h"

using namespace std;

vector<Processing> AlgorithmDijkstras::calculate(const vector<Courier>& couriers, const vector<Client>& clients) {
    vector<Processing> _timeArrival;
    for (auto& courier : couriers) {
        for (auto& client : clients) {
            int weightSum = 0;
            for (const auto &[id, orders, status, weight] : client.orders) {
                weightSum += weight;
            }
            if (weightSum != 0) {
                Processing inTimeArrival;
                const double time = (static_cast<double>(weightSum) * 100) *
                              (distance(pair{courier.x, courier.y}, pair{client.x, client.y}) / courier.speed);
                inTimeArrival.courier = courier;
                inTimeArrival.client = client;
                inTimeArrival.time = time;
                inTimeArrival.courier.localTime = time;

                _timeArrival.push_back(inTimeArrival);
            } else throw WeightIsNull();
        }
    }
    return _timeArrival;
}

string AlgorithmDijkstras::generateCommand(const Courier& courier, const Client& client) {
    return "Courier by ID: " + to_string(courier.id) + " (" + courier.fio + ") must to go to Client by ID: "
    + to_string(client.id) + " (" + client.fio + ") with Time: " + to_string(courier.finalTime);
}

vector<string> AlgorithmDijkstras::process(SQLController& controller) {
    vector<string> commands;
    while (!_clients.empty()) {
        auto timeArrival = calculate(_couriers, _clients);

        ranges::sort(timeArrival, [](const Processing& a, const Processing& b) {
            return a.time + a.courier.finalTime > b.time + b.courier.finalTime;
        });
        for (int i = 0; i < _couriers.size(); i++) {
            if (_clients.empty() || timeArrival.empty())
                break;
            auto [courier, client, time] = timeArrival.back();
            courier.finalTime += time;
            courier.x = client.x;
            courier.y = client.y;
            commands.emplace_back(generateCommand(courier, client));
            erase_if(_clients, [&](const Client &c) { return c.id == client.id; });
            erase_if(timeArrival, [&](const Processing &p) { return p.client.id == client.id; });
            erase_if(timeArrival, [&](const Processing &p) { return p.courier.id == courier.id; });
        }
    }

    return commands;
}

bool AlgorithmDijkstras::compareByTime(const Processing &a, const Processing &b) {
    return a.time < b.time;
}

