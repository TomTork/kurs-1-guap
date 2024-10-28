#include <iostream>
#include <string>

using namespace std;

tuple<bool, int, int> checkLocation(const string &location) {
    try {
        const string validateNumbers = "0123456789";
        const string part1 = location.substr(0, location.find(' '));
        const string part2 = location.substr(location.find(' ') + 1, location.length());
        for (const auto& el : part1) {
            if (!validateNumbers.contains(el)) {
                return tuple { false, -1, -1 };
            }
        }
        for (const auto& el : part2) {
            if (!validateNumbers.contains(el)) {
                return tuple { false, -1, -1 };
            }
        }
        return tuple { true, stoi(part1), stoi(part2) };
    } catch (exception& e) {
        cerr << e.what() << endl;
        return tuple { false, -1, -1 };
    }
}

pair<bool, string> checkStringState(const string &state, const bool allowSpace = false) {
    const string validateSymbols = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
    for (const auto& el : state) {
        if (!((allowSpace && el == ' ') || validateSymbols.contains(el))) {
            return pair { false, "" };
        }
    }
    return pair { true, state };
}

pair<bool, string> checkPhoneNumber(const string &num) {
    // 89177732814
    if ((num.length() == 12 && num[0] == '+') || (num.length() == 11)) {
        for (const auto& el : num) {
            if (!isdigit(el)) return pair { false, "" };
        }
        return pair { true, num };
    }
    return pair { false, "" };
}