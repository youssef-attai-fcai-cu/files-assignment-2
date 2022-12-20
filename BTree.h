#ifndef ASSIGNMENT_2_BTREE_H
#define ASSIGNMENT_2_BTREE_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

class BTree {
private:
    std::fstream file{};
    int numberOfRecords{};
    int m{};
    int cellSize{};

public:
    BTree(const std::string &filename, int _numberOfRecords, int _m, int _cellSize);

    ~BTree();

    void createFile();

    void displayFile();

    int insertRecord(int recordID, int reference);

    void deleteRecord(int recordID);

    int findRecord(int recordID);

    int recordSize() const { return (cellSize * ((2 * m) + 1)); }

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
    int writeCharArray(int at, char *charArray, int n) {
        file.seekg(at, std::ios::beg);
        
//      Instead of printing the array of characters directly,
//      print each character one by one to avoid weird null-terminator issue
        for (int i = 0; i < n; ++i) file << charArray[i];
    }

//  Converts a character array to an integer
    static int ctoi(char *c) { return std::stoi(std::string(c)); }

//  Pads the given string with whitespaces till it's size becomes n
    static std::string pad(const std::string &s, int n) {
        std::stringstream ss;
        ss << s;
        for (int i = 0; i < n - s.size(); ++i) ss << ' ';
        return ss.str();
    }

//  Read the record at the given index and return it as vector of pairs of recordID and reference
    std::vector<std::pair<int, int>> readNode(int recordIndex);

//  Write node record at the given index
    void writeNode(const std::vector<std::pair<int, int>> &node, int recordIndex);
};


#endif //ASSIGNMENT_2_BTREE_H