#ifndef ASSIGNMENT_2_BTREE_H
#define ASSIGNMENT_2_BTREE_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

class BTree {
private:
    std::fstream file{};
    int cellSize{};

    int numberOfRecords{};
    int m{};

public:
    int recordSize() const { return (cellSize * ((2 * m) + 1)); }

public:

    BTree(const std::string &filename, int _numberOfRecords, int _m, int _cellSize);

    ~BTree();

    void createFile();

    void displayFile();

    int insertRecord(int recordID, int reference);

    void deleteRecord(int recordID);

    int findRecord(int recordID) const;

//  Returns the index of the next empty record in the B-tree file,
//  or -1 if there are no empty records.
    int nextEmpty() {
//      Seek to the second cell of the header
        file.seekg(cellSize, std::ios::beg);

//      Read the content of the cell
        char cell[cellSize];
        file.read(cell, cellSize);

//      Return the value as an integer
        return ctoi(cell);
    }

    bool isEmpty(int recordIndex);

//  Returns the index of the next empty record after the record at recordIndex.
    int nextEmpty(int recordIndex) {
//      Seek to the second cell of the header
        file.seekg(recordIndex * recordSize() + cellSize, std::ios::beg);

//      Read the content of the cell
        char cell[cellSize];
        file.read(cell, cellSize);

        int i = ctoi(cell);
        if (!isEmpty(i)) throw "Node at this index is not even empty";
        
//      Return the value as an integer
        return i;
    }

//  Returns true if the record at the given index corresponds to a leaf node
//  i.e. The value in the record's first cell is 0
    bool isLeaf(int recordIndex) {
//      Seek to the beginning of the record at the given index
        file.seekg(recordIndex * recordSize(), std::ios::beg);

//      Read the first cell
        char leaf[cellSize];
        file.read(leaf, cellSize);

//      Return true if the read value is 0
        return ctoi(leaf) == 0;
    }

private:
//  Writes a character array to the B-tree file
    void writeCharArray(int at, char *charArray, int n) {
        file.seekg(at, std::ios::beg);

//      Instead of printing the array of characters directly,
//      print each character one by one to avoid weird null-terminator issue
        for (int i = 0; i < n; ++i) file << charArray[i];
    }

//  Converts a character array to an integer
    static int ctoi(char *c) { return std::stoi(std::string(c)); }

//  Pads the given string with whitespaces till it's size becomes n
    static std::string pad(const std::string &stringToBePadded, int n) {
        std::stringstream ss;
        ss << stringToBePadded;
        for (int i = 0; i < n - stringToBePadded.size(); ++i) ss << ' ';
        return ss.str();
    }

//  Pads the given stringToBePadded with whitespaces till it's size becomes finalSize
//  WARNING: delete the returned char array after usage
    static const char *pad(const char *stringToBePadded, int sSize, int finalSize) {
        char *paddedString = new char[finalSize];

        
        return paddedString;
    }

//  Read the record at the given index and return it as vector of pairs of recordID and reference
    std::vector<std::pair<int, int>> readNode(int recordIndex);

//  Write node record at the given index
    void writeNode(const std::vector<std::pair<int, int>> &node, int recordIndex);

    bool rootIsEmpty();

    void markAsLeaf(int recordIndex);

    void writePair(int recordIndex, int pairIndex, int recordID, int reference);

    int emptyCount();

    void markAsNonLeaf(int recordIndex);

    void splitNode(int recordIndex, std::pair<int, int> newPair);

    bool splitRoot(std::pair<int, int> newPair);

    void clearRecord(int recordIndex);
};


#endif //ASSIGNMENT_2_BTREE_H
