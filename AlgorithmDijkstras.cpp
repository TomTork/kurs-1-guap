#include "AlgorithmDijkstras.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include "helper.h"

using namespace std;

vector<string> AlgorithmDijkstras::process(SQLController& controller) {
    vector<string> commands;
    vector<Processing> timeArrival;
    Processing inTimeArrival{};
    for (auto& courier : this->_couriers) {
        for (auto& client : this->_clients) {
            int weightSum = 0;
            for (const auto& [_id, _orders, _status, _weight] : client.orders) {
                if (!_status) {
                    weightSum += _weight;
                }
            }
            if (weightSum != 0) {
                const double time = (static_cast<double>(weightSum) * 100) *
                              (distance(pair{courier.x, courier.y}, pair{client.x, client.y}) / courier.speed);
                inTimeArrival.courier = &courier;
                inTimeArrival.client = &client;
                inTimeArrival.time = time;

                timeArrival.push_back(inTimeArrival);
            }
        }
    }
    ranges::sort(timeArrival, [](const Processing& a, const Processing& b) {
        return a.time < b.time;
    });

    cout << "---LOG---" << endl;
    for (const auto& [courier, client, time, localTime] : timeArrival) {
        cout << courier->id << " " << courier->fio << " " << courier->x << " " << courier->y << "\t" << client->id << " " << client->fio << "\t" << time << endl;
    }

    double t = 0;
    while (true) {
        if (!timeArrival.empty()) {
            for (int i = 0; i < timeArrival.size(); i++) {
                if (timeArrival[i].localTime >= timeArrival[i].time) {
                    cout << "size: " << timeArrival.size() << endl;
                    cout << "Courier: " << timeArrival[i].courier->fio << " Client: " << timeArrival[i].client->fio << endl;
                    commands.push_back("Courier ID: " + to_string(timeArrival[i].courier->id) + " (" + timeArrival[i].courier->fio + ")" +
                    " must to go client ID: " + to_string(timeArrival[i].client->id) + " (" + timeArrival[i].client->fio + ")");
                    timeArrival[i].courier->x = timeArrival[i].client->x;
                    timeArrival[i].courier->y = timeArrival[i].client->y;
                    // controller.updateCoordsCourier(courier.id, client.x, client.y);
                    erase_if(timeArrival, [&](const Processing &p) { return p.client->id == timeArrival[i].client->id; });
                    break;
                }
                timeArrival[i].localTime = t;
            }
            // for (auto &[courier, client, time, localTime] : timeArrival) {
            //     cout << courier.id << " " << courier.x << " " << courier.y << endl;
            //     if (localTime >= time) {
            //         commands.push_back("Courier ID: " + to_string(courier.id) + " (" + courier.fio + ")" +
            //         " must to go client ID: " + to_string(client.id) + " (" + client.fio + ")");
            //         courier.x = client.x;
            //         courier.y = client.y;
            //         // controller.updateCoordsCourier(courier.id, client.x, client.y);
            //         erase_if(timeArrival, [&](const Processing &p) { return p.client.id == client.id; });
            //         break;
            //     }
            //     localTime = t;
            // }
            t++;
        } else break;
    }
    // for (int iter = 0; iter < timeArrival.size(); iter++) {
    //     if (!timeArrival.empty()) {
    //
    //
    //         for (auto &[courier, client, time, status] : timeArrival) {
    //             if (status) {
    //                 commands.push_back("Courier ID: " + to_string(courier.id) + " (" + courier.fio + ")" +
    //                 "  must to go client ID: " + to_string(client.id) + " (" + client.fio + ")");
    //                 controller.updateCoordsCourier(courier.id, client.x, client.y);
    //                 erase_if(timeArrival, [&](const Processing &p) { return p.client.id == client.id; });
    //                 status = false;
    //                 break;
    //             }
    //         }
    //     } else break;
    // }

    return commands;
}

bool AlgorithmDijkstras::compareByTime(const Processing &a, const Processing &b) {
    return a.time < b.time;
}

