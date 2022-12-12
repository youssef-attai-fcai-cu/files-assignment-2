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

bool isEmpty(int recordIndex, int recordSize, std::fstream &btree) {
    btree.seekg(recordIndex * recordSize);
    char leaf[CELL_SIZE];
    btree.read(leaf, CELL_SIZE);
    return std::stoi(std::string(leaf)) == -1;
}

std::streamoff getFileSize(std::fstream &btree) {
    btree.seekg(0, std::ios::end);
    return btree.tellg();
}

void closeBTree(std::fstream &btree) {
    btree.close();
}

int getIndexOfNextEmptyRecord(std::fstream &btree) {
    btree.seekg(CELL_SIZE, std::ios::beg);
    char nextEmpty[CELL_SIZE];
    btree.read(nextEmpty, CELL_SIZE);
    return std::stoi(std::string(nextEmpty));
}

std::vector<std::pair<int, int>> readNode(int nodeIndex, int recordSize, std::fstream &btree) {
    std::vector<std::pair<int, int>> node;
    btree.seekg(nodeIndex * recordSize);
    
//  TODO: Node read logic
    
    return node;
}
