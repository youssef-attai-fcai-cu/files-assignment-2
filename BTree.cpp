#include <sstream>
#include <utility>
#include <iostream>
#include "BTree.h"
#include "exceptions/InvalidRecordNumber.h"
#include "exceptions/InvalidPairNumber.h"
#include "utils.h"

#define EXISTING_FILE (std::ios::in | std::ios::out | std::ios::app)
#define NEW_FILE (std::ios::trunc | EXISTING_FILE)

BTree::BTree(std::string _path, int _m, int _numberOfRecords, int _cellSize) {
    openFile();
    path = std::move(_path);
    m = _m;
    numberOfRecords = _numberOfRecords;
    cellSize = _cellSize;
    initialize();
}

void BTree::openFile() {
    file.open("../btree", EXISTING_FILE);
    if (!file) file.open("../btree", NEW_FILE);
}

void BTree::display() {
    char record[recordSize()];

    // Iterate over records printing every one in a new line.
    for (int i = 0; i < numberOfRecords + 1; ++i) {
        file.seekg(i * recordSize(), std::ios::beg);
        file.read(record, recordSize());
        std::cout << record << '\n';
    }
}

BTree::~BTree() {
    file.close();
}

std::pair<int, int> BTree::pair(int recordNumber, int pairNumber) {
    // Validate inputs
    validateRecordNumber(recordNumber);
    validatePairNumber(pairNumber);

    // Create and return the cell
    std::pair<int, int> thePair;
    thePair.first = cell(recordNumber, 2 * pairNumber - 1);
    thePair.second = cell(recordNumber - 1, 2 * pairNumber);
    return thePair;
}

std::vector<std::pair<int, int>> BTree::node(int recordNumber) {
    // Validate input
    validateRecordNumber(recordNumber);

    // Go to the specified record on the b-tree file
    file.seekg(recordNumber * recordSize() + cellSize, std::ios::beg);

    // Create and return the node
    std::vector<std::pair<int, int>> theNode;
    for (int i = 1; i <= m; ++i) theNode.push_back(pair(recordNumber, i));
    return theNode;
}

void BTree::validateRecordNumber(int recordNumber) const {
    if (recordNumber <= 0 || recordNumber > numberOfRecords)
        throw InvalidRecordNumber(recordNumber);
}

void BTree::validatePairNumber(int pairNumber) const {
    if (pairNumber <= 0 || pairNumber > m)
        throw InvalidPairNumber(pairNumber);
}

int BTree::nextEmpty() {
    return cell(0, 1);
}

int BTree::cell(int rowIndex, int columnIndex) {
    // Go to the specified cell
    file.seekg(rowIndex * recordSize() + columnIndex * cellSize, std::ios::beg);

    // Read and return the integer value in the cell
    char cell[cellSize];
    file.read(cell, cellSize);
    return ctoi(cell);
}

int BTree::insert(int recordId, int reference) {
    return 0;
}

void BTree::initialize() {
    for (int i = 0; i < numberOfRecords; ++i) {
        file << pad(-1);
        file << pad(i + 1);
        for (int j = 0; j < m * 2; ++j) file << pad(-1);
    }
}

std::string BTree::pad(int value) const {
    std::stringstream ss;

    // Convert the integer value into a string
    std::string v = std::to_string(value);

    // Write the string
    ss << v;

    // Write spaces until the final result's size becomes the cell size
    for (int i = 0; i < cellSize - v.size(); ++i) ss << ' ';
    return ss.str();
}
