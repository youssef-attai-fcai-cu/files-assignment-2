#include "helpers.h"
#include "constants.h"
#include <sstream>

void openBTree(char *filename, std::fstream &btree) {
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

bool isLeaf(int recordIndex, int recordSize, std::fstream &btree) {
    btree.seekg(recordIndex * recordSize);
    char leaf[CELL_SIZE];
    btree.read(leaf, CELL_SIZE);
    return std::stoi(std::string(leaf)) == 0;
}

bool isEmpty(int recordIndex, std::fstream &btree) {
    return false;
}

void closeBTree(std::fstream &btree) {
    btree.close();
}
