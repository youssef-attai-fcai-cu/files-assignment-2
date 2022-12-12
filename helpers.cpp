#include "helpers.h"
#include <sstream>

void openBTree(std::fstream &btree, char *filename) {
    btree.open(filename, std::ios::in | std::ios::out);
    if (!btree.is_open())
        btree.open(filename, std::ios::trunc | std::ios::out | std::ios::in);
}

std::string pad(const std::string &s, int n) {
    std::stringstream ss;
    ss << s;
    for (int i = 0; i < n - s.size(); ++i)
        ss << ' ';
    return ss.str();
}
