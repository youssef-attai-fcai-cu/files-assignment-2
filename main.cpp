#include <iostream>
#include "BTree.h"

void printSeparator();

int main() {
    BTree btree("../btree", 10, 5, 4);
    btree.insertRecord(3, 12);
    btree.insertRecord(7, 24);
    btree.insertRecord(10, 48);
    btree.insertRecord(24, 60);
    btree.insertRecord(14, 72);
    btree.displayFile();
    printSeparator();

    btree.insertRecord(19, 84);
    btree.displayFile();
    printSeparator();

    btree.insertRecord(30, 196);
    btree.insertRecord(15, 108);
    btree.insertRecord(1, 120);
    btree.insertRecord(5, 132);
    btree.displayFile();
    printSeparator();

    btree.insertRecord(2, 144);
    btree.displayFile();
    printSeparator();

    btree.insertRecord(8, 156);
    btree.insertRecord(9, 168);
    btree.insertRecord(6, 180);
    btree.insertRecord(11, 192);
    btree.insertRecord(12, 204);
    btree.insertRecord(17, 216);
    btree.insertRecord(18, 228);
    btree.displayFile();
    printSeparator();

    btree.insertRecord(32, 240);
    btree.displayFile();
    printSeparator();
    
    return 0;
}

void printSeparator() {
    std::cout << '\n' << "---------------------------------------------" << '\n';
}
