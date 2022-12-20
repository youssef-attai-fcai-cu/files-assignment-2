#include "BTree.h"
#include <algorithm>
#include <stack>
#include <iostream>


BTree::BTree(const std::string &filename, int _numberOfRecords, int _m, int _cellSize) {
    numberOfRecords = _numberOfRecords;
    m = _m;
    cellSize = _cellSize;

    file.open(filename, std::ios::in | std::ios::out);
    if (!file.is_open())
        file.open(filename, std::ios::trunc | std::ios::out | std::ios::in);

    createFile();
}

BTree::~BTree() {
    file.close();
}

void BTree::createFile() {
//  For each record
    for (int i = 0; i < numberOfRecords; ++i) {
//      Seek to the beginning of the record
        file.seekg(i * recordSize());

//      Add the leaf status cell
//      -1 means it is neither leaf nor non-leaf, it is an empty record
        file << pad("-1", cellSize);

//      Add index of the next empty cell
        file << pad(std::to_string(i + 1), cellSize);

//      Fill the rest of the record cells with -1
        for (int j = 0; j < (2 * m) - 1; ++j) file << pad("-1", cellSize);
    }
}

int BTree::insertRecord(int recordID, int reference) {
//  Seek to the beginning of the file
    file.seekg(0, std::ios::beg);

//  If root is empty, insert in root
    if (rootIsEmpty()) {
        getNextEmpty(1); //  The header's next empty cell value = the root's next empty cell value
        writePair(1, 0, recordID, reference); //  Write pair in root node
        markAsLeaf(1); //  Update root's leaf status to LEAF
    } else {  //  Otherwise, if root is NOT empty

        /* Traverse B-tree */
        
        file.seekg(recordSize(), std::ios::beg); //  Seek to root (skip header)
        std::stack<int> visited; //  Keep track of visited cells

//      Traverse records till a leaf is reached record
//      starting from root (i.e. Record at index 1)
        int currentRecordIndex = 1;
        while (!isLeaf(currentRecordIndex)) {
//          For each pair of cells in the current record
            for (int i = 0; i < m; ++i) {
                char key[cellSize], val[cellSize];
                file.read(key, cellSize);
                file.read(val, cellSize);
                int k = ctoi(key);
                int v = ctoi(val);

                if (k == -1 && v == -1) break;

                currentRecordIndex = v;

//              Update visited cells
                visited.push((int) file.tellg() - (2 * cellSize));

                if (recordID < k) break;
            }
        }

        /* Leaf reached, insert pair in record at currentRecordIndex */

//      Read node into memory
        std::vector<std::pair<int, int>> node = readNode(currentRecordIndex);

        if (node.size() < m) {
//          Insert into node
            node.emplace_back(recordID, reference);

//          Sort node on pair.first
            std::sort(node.begin(), node.end());

//          Write node
            writeNode(node, currentRecordIndex);
        } else {
//          
        }


//      Update visited parents
        while (!visited.empty()) {
//          Get the latest visited parent cell
            int parentCell = visited.top();
            visited.pop();

//          Get direct child node to this parent
            file.seekg(parentCell + cellSize);
            char childNodeIndex[cellSize];
            file.read(childNodeIndex, cellSize);
            node = readNode(ctoi(childNodeIndex));

//          Get maximum pair in the child node
            std::pair<int, int> maxPair = *std::max_element(node.begin(), node.end());

//          Override new maximum in parent
            file.seekg(parentCell);
            file << pad(std::to_string(maxPair.first), cellSize);
        }
    }

/*
     Check root
     If (root is empty) Insert in root
     Otherwise {
         While (current node is non-leaf) Traverse the B-tree
         Read node in memory
         If (Node is not full) {
             Insert new pair <RecordID, Reference>
             Sort node
             Write node
         } Otherwise {
             // There is at least 1 empty record
             If (Next empty cell value != -1) {
                                        // At least 2 empty records
                If (Parent is full AND number of empty records > 1) Split parent
                Otherwise Abort
                Split node
                Insert new pair <RecordID, Reference>
                Sort node
                Write node
                Adjust parent (i.e. insert new pair in parent)
             } Otherwise Abort
         }
     }
 */

    return 0;
}

void BTree::getNextEmpty(int recordIndex) {
//  Read new next empty from root
    char newNextEmpty[cellSize];
    file.seekg((recordIndex * recordSize()) + cellSize, std::ios::beg);
    file.read(newNextEmpty, cellSize);

//  Store new next empty in header
    writeCharArray(cellSize, newNextEmpty, cellSize);
}

void BTree::markAsLeaf(int recordIndex) {
    file.seekg(recordIndex * recordSize(), std::ios::beg);
    file << pad("0", cellSize);
}

bool BTree::rootIsEmpty() { return nextEmpty() == 1; }

void BTree::displayFile() {
//  Seek to the beginning of the file
    file.seekg(0, std::ios::beg);

//  For each record
    for (int j = 0; j < numberOfRecords; ++j) {
//      Read the record
        char record[recordSize()];
        file.read(record, recordSize());

//      Instead of printing the array of characters, 
//      print each character one by one to avoid weird null-terminator issue
        for (char i: record) std::cout << i;

        std::cout << '\n';
    }
}

std::vector<std::pair<int, int>> BTree::readNode(int recordIndex) {
//  Returned node result
    std::vector<std::pair<int, int>> node;

//  Seek to the beginning of the node skipping the first cell (i.e. leaf status cell)
    file.seekg(recordIndex * recordSize() + cellSize);

//  For each pair <PrimaryKey, DatafileReference>
    for (int i = 0; i < m; ++i) {
//      Read two cells
        char key[cellSize], val[cellSize];
        file.read(key, cellSize);
        file.read(val, cellSize);
        int k = ctoi(key);
        int v = ctoi(val);

//      If end of node, stop
        if (k == -1 && v == -1) break;

//      Append pair to node vector
        node.emplace_back(k, v);
    }

    return node;
}

void BTree::writeNode(const std::vector<std::pair<int, int>> &node, int recordIndex) {
//  Seek to the beginning to the record
//  skipping the first cell (i.e. The leaf status cell)
    file.seekg(recordIndex * recordSize() + cellSize);

//  Write the pairs of recordID and reference one by one
    for (auto &i: node) {
        file << pad(std::to_string(i.first), cellSize)
             << pad(std::to_string(i.second), cellSize);
    }
}

void BTree::deleteRecord(int recordID) {

}

int BTree::findRecord(int recordID) const {
    return m;
}

void BTree::writePair(int recordIndex, int pairIndex, int recordID, int reference) {
    file.seekg((recordIndex * recordSize()) + cellSize + (pairIndex * cellSize), std::ios::beg);
    file << pad(std::to_string(recordID), cellSize)
         << pad(std::to_string(reference), cellSize);
}
