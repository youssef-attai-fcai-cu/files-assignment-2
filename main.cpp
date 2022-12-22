#include "BTree.h"

int main() {
    BTree btree("../btree", 10, 5, 4);
    btree.insertRecord(3, 12);
    btree.insertRecord(7, 24);
    btree.insertRecord(10, 48);
    btree.insertRecord(24, 60);
    btree.insertRecord(14, 72);
    btree.insertRecord(9, 123);
    btree.insertRecord(11, 456);
    btree.displayFile();

    return 0;
}
