#include <sstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include "BTree.h"
#include "exceptions/InvalidRecordNumber.h"
#include "exceptions/InvalidPairNumber.h"
#include "utils.h"


BTree::BTree(std::string _path, int _numberOfRecords, int _m, int _cellSize) :
        path{std::move(_path)},
        m{_m},
        numberOfRecords{_numberOfRecords},
        cellSize{_cellSize} {
    openFile();
    initialize();
}

void BTree::openFile() {
    file.open("../btree", std::ios::trunc | std::ios::in | std::ios::out);
}

void BTree::display() {
    // For each record
    for (int i = 0; i < numberOfRecords; ++i) {
        // Read the record
        char record[recordSize()];
        file.seekg(i * recordSize(), std::ios::beg);
        file.read(record, recordSize());

        // Instead of printing the array of characters, 
        // print each character one by one to avoid weird null-terminator issue
        for (char r: record) std::cout << r;

        std::cout << '\n';
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
    thePair.second = cell(recordNumber, 2 * pairNumber);
    return thePair;
}

std::vector<std::pair<int, int>> BTree::node(int recordNumber) {
    // Validate input
    validateRecordNumber(recordNumber);

    // Go to the specified record on the b-tree file
    file.seekg(recordNumber * recordSize() + cellSize, std::ios::beg);

    // Create and return the node
    std::vector<std::pair<int, int>> theNode;

    // Read every pair in the node
    for (int i = 1; i <= m; ++i) {
        std::pair<int, int> p = pair(recordNumber, i);

        // If it is empty then the rest is empty, so return
        if (p.second == -1) return theNode;

        // Otherwise, continue reading the node
        theNode.push_back(p);
    }
    return theNode;
}

void BTree::validateRecordNumber(int recordNumber) const {
    // If the record number is not between 1 and numberOfRecords
    if (recordNumber <= 0 || recordNumber > numberOfRecords)
        // then it is not a valid record number
        throw InvalidRecordNumber(recordNumber);
}

void BTree::validatePairNumber(int pairNumber) const {
    // If the pair number is not between 1 and m
    if (pairNumber <= 0 || pairNumber > m)
        // then it is not a valid pair number
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
    std::vector<std::pair<int, int>> current;

    // If the root is empty
    if (isEmpty(1)) {
        // Insert in root

        // The next empty for the record at next empty
        int nextEmptyNext = cell(nextEmpty(), 1);

        // Update the next empty
        writeCell(nextEmptyNext, 0, 1);

        // Create the node
        current = node(1);

        // Insert the new pair
        current.emplace_back(recordId, reference);

        // Sort the node
        std::sort(current.begin(), current.end());

        // Write the node in root
        writeNode(current, 1);

        // Mark the root as leaf
        markLeaf(1);

        // Return the index of the record in which the insertion happened
        // i.e. the root in this case
        return 1;
    }

    // Search for recordId in every node in the b-tree
    // starting with the root
    int currentIndex = 1;
    bool found;
    while (!isLeaf(currentIndex)) {
        current = node(currentIndex);
        found = false;
        for (auto p: current) {
            // If a greater value is found
            if (p.first >= recordId) {
                currentIndex = p.second;
                found = true;
                break;
            }
        }
        if (!found) currentIndex = current.back().second;
    }

    current = node(currentIndex);
    
    // Insert the new pair
    current.emplace_back(recordId, reference);

    // Sort the node
    std::sort(current.begin(), current.end());
    
    

    // Write the node in root
    writeNode(current, 1);

    // Return the index of the inserted record
    // or -1 if insertion failed
    return currentIndex;
}

void BTree::initialize() {
    // For each record
    for (int recordIndex = 0; recordIndex < numberOfRecords + 1; ++recordIndex) {
        // Write -1 in the first cell to indicate it is 
        // an empty record (available for allocation)
        writeCell(-1, recordIndex, 0);

        // Write the number of the next empty record
        // in the available list
        writeCell(recordIndex + 1, recordIndex, 1);

        // Fill the rest of the record with -1s
        for (int cellIndex = 2; cellIndex <= m * 2; ++cellIndex)
            writeCell(-1, recordIndex, cellIndex);
    }
}

std::string BTree::pad(int value) const {
    std::stringstream result;

    // Convert the integer value into a string
    std::string stringValue = std::to_string(value);

    // Write the string
    result << stringValue;

    // Write spaces until the final result's size becomes the cell size
    for (int i = 0; i < cellSize - stringValue.size(); ++i) result << ' ';
    return result.str();
}

void BTree::writeCell(int value, int rowIndex, int columnIndex) {
    // Go to the specified cell
    file.seekg(rowIndex * recordSize() + columnIndex * cellSize, std::ios::beg);

    // Write the given value in the cell
    file << pad(value);
}

bool BTree::isLeaf(int recordNumber) {
    return cell(recordNumber, 0) == 0;
}

bool BTree::isEmpty(int recordNumber) {
    return cell(recordNumber, 0) == -1;
}

void BTree::writeNode(const std::vector<std::pair<int, int>> &node, int recordNumber) {
    file.seekg(recordNumber * recordSize() + cellSize, std::ios::beg);
    for (auto p: node)
        file << pad(p.first) << pad(p.second);
}

void BTree::markLeaf(int recordNumber) {
    file.seekg(recordNumber * recordSize(), std::ios::beg);
    file << pad(0);
}
