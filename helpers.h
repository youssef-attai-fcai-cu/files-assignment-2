#ifndef ASSIGNMENT_2_HELPERS_H
#define ASSIGNMENT_2_HELPERS_H

#include <fstream>
#include <vector>

//  Get the size of a file
std::streamoff getFileSize(std::fstream &btree);

//  Returns true if the record at index recordIndex represents a leaf node
bool isLeaf(int recordIndex, int recordSize, std::fstream &btree);

//  Returns true if the record at index recordIndex is an empty record
bool isEmpty(int recordIndex, int recordSize, std::fstream &btree);

//  Reads the value in the second cell of the B-tree's header
//  i.e. the index of the next empty record
int getIndexOfNextEmptyRecord(std::fstream &btree);

//  Opens B-tree file named filename
void openBTree(char *filename, std::fstream &btree);

//  Closes B-tree file
void closeBTree(std::fstream &btree);

//  Pads a string s with spaces till it's size becomes n
std::string pad(const std::string &s, int n);

//  Read the record at nodeIndex and return it as vector of pairs
std::vector<std::pair<int, int>> readNode(int nodeIndex, int recordSize, int m, std::fstream &btree);

//  Write node at index nodeIndex
void writeNode(const std::vector<std::pair<int, int>>& node, int nodeIndex, int recordSize, std::fstream &btree);

//  Converts a character array c into an integer
int ctoi(char c[]);

#endif //ASSIGNMENT_2_HELPERS_H
