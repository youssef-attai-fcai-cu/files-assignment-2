#ifndef ASSIGNMENT_2_HELPERS_H
#define ASSIGNMENT_2_HELPERS_H

#include <fstream>

//  Returns true if the record at index recordIndex represents a leaf node
bool isLeaf(int recordIndex, int recordSize, std::fstream &btree);

//  Returns true if the record at index recordIndex is an empty record
bool isEmpty(int recordIndex, std::fstream &btree);

//  Reads the value in the second cell of the B-tree's header
//  i.e. the index of the next empty record
int getIndexOfNextEmptyRecord();

//  Opens B-tree file named filename
void openBTree(char *filename, std::fstream &btree);

//  Closes B-tree file
void closeBTree(std::fstream &btree);

//  Pads a string s with spaces till it's size becomes n
std::string pad(const std::string &s, int n);

#endif //ASSIGNMENT_2_HELPERS_H
