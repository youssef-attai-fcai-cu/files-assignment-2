#include <iostream>
#include "BTree.h"

void printSeparator();

void assignmentTestCase(BTree &btree);

int main() {
    BTree btree("../btree", 10, 5, 5);
    btree.display();
    
    return 0;
}

void printSeparator() {
    std::cout << '\n' << "---------------------------------------------" << '\n';
}

void assignmentTestCase(BTree &btree) {
    btree.insert(3, 12);
    btree.insert(7, 24);
    btree.insert(10, 48);
    btree.insert(24, 60);
    btree.insert(14, 72);
    btree.display();
    printSeparator();

    btree.insert(19, 84);
    btree.display();
    printSeparator();

    btree.insert(30, 196);
    btree.insert(15, 108);
    btree.insert(1, 120);
    btree.insert(5, 132);
    btree.display();
    printSeparator();

    btree.insert(2, 144);
    btree.display();
    printSeparator();

    btree.insert(8, 156);
    btree.insert(9, 168);
    btree.insert(6, 180);
    btree.insert(11, 192);
    btree.insert(12, 204);
    btree.insert(17, 216);
    btree.insert(18, 228);
    btree.display();
    printSeparator();

    btree.insert(32, 240);
    btree.display();
    printSeparator();
}
