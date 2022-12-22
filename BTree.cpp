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
    for (int i = 0; i < numberOfRecords; ++i) clearRecord(i);
}

void BTree::clearRecord(int recordIndex) {
//      Seek to the beginning of the record
    file.seekg(recordIndex * recordSize());

//      Add the leaf status cell
//      -1 means it is neither leaf nor non-leaf, it is an empty record
    file << pad("-1", cellSize);

//      Add index of the next empty cell
    file << pad(std::to_string(recordIndex + 1), cellSize);

//      Fill the rest of the record cells with -1
    for (int j = 0; j < (2 * m) - 1; ++j) file << pad("-1", cellSize);
}

int BTree::insertRecord(int recordID, int reference) {
//  Seek to the beginning of the file
    file.seekg(0, std::ios::beg);

//  If root is empty, insert in root
    if (rootIsEmpty()) {
        //  The header's next empty cell value = the root's next empty cell value
        //  Read new next empty from root
        char newNextEmpty[cellSize];
        file.seekg((1 * recordSize()) + cellSize, std::ios::beg);
        file.read(newNextEmpty, cellSize);

        //  Store new next empty in header (update 2nd cell in header)
        writeCharArray(cellSize, newNextEmpty, cellSize);

        writePair(1, 0, recordID, reference); //  Write pair in root node
        markAsLeaf(1); //  Update root's leaf status to LEAF
        return 1;
    } else {  //  Otherwise, if root is NOT empty

//      Traverse records till a leaf is reached record
//      starting from root (i.e. Record at index 1)
        int currentRecordIndex = 1;

        std::stack<int> visited;
        visited.push(currentRecordIndex);

        while (!isLeaf(currentRecordIndex)) {
            bool found = false;
            for (int i = 0; i < m; ++i) {
                char key[cellSize], val[cellSize];
                file.read(key, cellSize);
                file.read(val, cellSize);
                int k = ctoi(key);
                int v = ctoi(val);
                if (k == -1 && v == -1) {
                    found = true;
                    break;
                }
                currentRecordIndex = v;
                if (recordID < k) {
                    found = true;
                    break;
                }
            }
            visited.push(currentRecordIndex);
            if (found) break;
        }

        std::vector<std::pair<int, int>> lastVisited = readNode(currentRecordIndex);
        lastVisited.emplace_back(recordID, reference);
        std::sort(lastVisited.begin(), lastVisited.end());

        while (!visited.empty()) {
            int lastVisitedIndex = visited.top(); // first time = currentRecordIndex
            visited.pop();

//          Overflow
            if (lastVisited.size() > m) {
//              If root
                if (lastVisitedIndex == 1) {
//                  Split root, no update in parent nodes
                    if (splitRoot(lastVisited)) return 1; // Successfully inserted lastVisited
                    else return -1; // No space, so no insertion
                }
                int newNodeIndex = splitNode(lastVisitedIndex, lastVisited);
                
            } else {
//                no overflow, normal insertion
            }
        }
    }
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

int BTree::emptyCount() {
    int c = 0;
    char leafStatus[cellSize];
    for (int i = 1; i < numberOfRecords; ++i) {
        file.seekg(i * recordSize(), std::ios::beg);
        file.read(leafStatus, cellSize);
        if (ctoi(leafStatus) == -1) c++;
    }
    return c;
}

void BTree::markAsNonLeaf(int recordIndex) {
    file.seekg(recordIndex * recordSize(), std::ios::beg);
    file << pad("1", cellSize);
}

int BTree::splitNode(int recordIndex, std::vector<std::pair<int, int>> originalNode) {
    int nodeIndex = nextEmpty(); //  Get the index for the new node
    if (nodeIndex == -1) return -1;

//    TODO: Move padding into a separate function
    int newNextEmptyInHeader = nextEmpty(nodeIndex);
    std::string newNextEmptyInHeaderStr = std::to_string(newNextEmptyInHeader);
    const char *stringToBePadded = newNextEmptyInHeaderStr.c_str();
    char *paddedString = new char[cellSize];

    for (int i = 0; i < newNextEmptyInHeaderStr.size(); ++i)
        paddedString[i] = stringToBePadded[i];
    for (int i = (int) newNextEmptyInHeaderStr.size(); i < cellSize; ++i)
        paddedString[i] = ' ';

    writeCharArray(cellSize, paddedString, cellSize);

//  Distribute originalNode on two new nodes
    std::vector<std::pair<int, int>> firstNode, secondNode;

//  Fill first and second nodes from originalNode
    auto middle(originalNode.begin() + (int) (originalNode.size()) / 2);
    for (auto it = originalNode.begin(); it != originalNode.end(); ++it) {
        if (std::distance(it, middle) > 0) firstNode.push_back(*it);
        else secondNode.push_back(*it);
    }

//  Clear originalNodeIndex and newNodeIndex
    clearRecord(recordIndex);
    clearRecord(nodeIndex);

//  Write firstNode onto originalNode
    writeNode(firstNode, recordIndex);
    markAsLeaf(recordIndex); //  Update originalNode's leaf status to LEAF

//  Write second originalNode
    writeNode(secondNode, nodeIndex);
    markAsLeaf(nodeIndex); //  Update originalNode's leaf status to LEAF

    return nodeIndex;
}

bool BTree::splitRoot(std::vector<std::pair<int, int>> root) {
//  Find 2 empty records for the new nodes
    int firstNodeIndex = nextEmpty(); //  Get the first empty node
    if (firstNodeIndex == -1) return false;

    int secondNodeIndex = nextEmpty(firstNodeIndex);  //  Get empty node after it
    if (secondNodeIndex == -1) return false;

//  Write in header the new empty node index (based on the last empty node found)

//    TODO: Move padding into a separate function
    int newNextEmptyInHeader = nextEmpty(secondNodeIndex);
    std::string newNextEmptyInHeaderStr = std::to_string(newNextEmptyInHeader);
    const char *stringToBePadded = newNextEmptyInHeaderStr.c_str();
    char *paddedString = new char[cellSize];

    for (int i = 0; i < newNextEmptyInHeaderStr.size(); ++i)
        paddedString[i] = stringToBePadded[i];
    for (int i = (int) newNextEmptyInHeaderStr.size(); i < cellSize; ++i)
        paddedString[i] = ' ';

    writeCharArray(cellSize, paddedString, cellSize);

    std::vector<std::pair<int, int>> firstNode, secondNode;

//  Fill first and second nodes from root
    auto middle(root.begin() + (int) (root.size()) / 2);
    for (auto it = root.begin(); it != root.end(); ++it) {
        if (std::distance(it, middle) > 0) firstNode.push_back(*it);
        else secondNode.push_back(*it);
    }

//  Write first node
    writeNode(firstNode, firstNodeIndex);
    markAsLeaf(firstNodeIndex); //  Update node's leaf status to LEAF

//  Write second node
    writeNode(secondNode, secondNodeIndex);
    markAsLeaf(secondNodeIndex); //  Update node's leaf status to LEAF

//  Create new root with max values from the 2 new nodes
    clearRecord(1);

    std::vector<std::pair<int, int>> newRoot;

//  pair in non-leaf <Redundant value: index to node with this value>
    newRoot.emplace_back(firstNode.back().first, firstNodeIndex);
    newRoot.emplace_back(secondNode.back().first, secondNodeIndex);

    writeNode(newRoot, 1);
    markAsNonLeaf(1); //  Update root's leaf status to NON-LEAF

    return true;
}

bool BTree::isEmpty(int recordIndex) {
//  Seek to the beginning of the record at the given index
    file.seekg(recordIndex * recordSize(), std::ios::beg);

//  Read the first cell
    char leaf[cellSize];
    file.read(leaf, cellSize);

//  Return true if the read value is -1
    return ctoi(leaf) == -1;
}

std::vector<int> BTree::childrenIndexes(int recordIndex) {
    if (isLeaf(recordIndex)) return {};
    std::vector<int> references;
    for (auto i: readNode(recordIndex)) references.push_back(i.second);
    return references;
}

std::pair<int, int> BTree::getMaxPair(int recordIndex) {
    return readNode(recordIndex).back();
}
