#ifndef HELPER_H
#define HELPER_H
#include <string>
#include <tuple>

std::tuple<bool, int, int> checkLocation(const std::string&);

std::pair<bool, string> checkStringState(const std::string&, bool allowSpace = false);

std::pair<bool, string> checkPhoneNumber(const string&);

#endif //HELPER_H
