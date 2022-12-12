#include "helpers.h"
#include "constants.h"
#include <sstream>

int ctoi(char *c) {
    return std::stoi(std::string(c));
}

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
    btree.seekg(recordIndex * recordSize, std::ios::beg);
    char leaf[CELL_SIZE];
    btree.read(leaf, CELL_SIZE);
    return ctoi(leaf) == 0;
}

bool isEmpty(int recordIndex, int recordSize, std::fstream &btree) {
    btree.seekg(recordIndex * recordSize);
    char leaf[CELL_SIZE];
    btree.read(leaf, CELL_SIZE);
    return ctoi(leaf) == -1;
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

std::vector<std::pair<int, int>> readNode(int nodeIndex, int recordSize, int m, std::fstream &btree) {
//  Returned node result
    std::vector<std::pair<int, int>> node;

//  Seek to the beginning of the node skipping the first cell (i.e. leaf status cell)
    btree.seekg(nodeIndex * recordSize + CELL_SIZE);

//  For each pair <PrimaryKey, DatafileReference>
    for (int i = 0; i < m; ++i) {
//      Read two cells
        char key[CELL_SIZE], reference[CELL_SIZE];
        btree.read(key, CELL_SIZE);
        btree.read(reference, CELL_SIZE);
        int k = ctoi(key);
        int r = ctoi(reference);

//      If end of node, stop
        if (k == -1 && r == -1) break;

//      Append pair to node vector
        node.emplace_back(std::make_pair(k, r));
    }

    return node;
}

void writeNode(const std::vector<std::pair<int, int>>& node, int nodeIndex, int recordSize, std::fstream &btree) {
    btree.seekg(nodeIndex * recordSize + CELL_SIZE);
    for (auto & i : node) {
        btree << pad(std::to_string(i.first), CELL_SIZE)
              << pad(std::to_string(i.second), CELL_SIZE);
    }
}
