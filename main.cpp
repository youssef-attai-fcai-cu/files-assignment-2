#include <iostream>
#include "BTree.h"

#define SEPARATOR std::cout << "-----------------------------------------------------\n";

void insertions(BTree &btree);

int main() {
    BTree btree("../btree", 10, 5, 5);
    insertions(btree);
    return 0;
}

void insertions(BTree &btree) {

    std::cout << "Found " << 3 << " at: " <<  btree.search(3) << '\n';
    std::cout << "Found " << 19 << " at: " <<  btree.search(19) << '\n';
    std::cout << "Found " << 30 << " at: " <<  btree.search(30) << '\n';
    std::cout << "Found " << 2 << " at: " <<  btree.search(2) << '\n';
    std::cout << "Found " << 8 << " at: " <<  btree.search(8) << '\n';
    std::cout << "Found " << 32 << " at: " <<  btree.search(32) << '\n';
    
    SEPARATOR
    
    btree.insert(3, 12);
    btree.insert(7, 24);
    btree.insert(10, 48);
    btree.insert(24, 60);
    btree.insert(14, 72);

    btree.display();

    std::cout << "Found " << 3 << " at: " <<  btree.search(3) << '\n';
    std::cout << "Found " << 19 << " at: " <<  btree.search(19) << '\n';
    std::cout << "Found " << 30 << " at: " <<  btree.search(30) << '\n';
    std::cout << "Found " << 2 << " at: " <<  btree.search(2) << '\n';
    std::cout << "Found " << 8 << " at: " <<  btree.search(8) << '\n';
    std::cout << "Found " << 32 << " at: " <<  btree.search(32) << '\n';

    SEPARATOR
    
    btree.insert(19, 84);

    btree.display();

    std::cout << "Found " << 3 << " at: " <<  btree.search(3) << '\n';
    std::cout << "Found " << 19 << " at: " <<  btree.search(19) << '\n';
    std::cout << "Found " << 30 << " at: " <<  btree.search(30) << '\n';
    std::cout << "Found " << 2 << " at: " <<  btree.search(2) << '\n';
    std::cout << "Found " << 8 << " at: " <<  btree.search(8) << '\n';
    std::cout << "Found " << 32 << " at: " <<  btree.search(32) << '\n';
    
    SEPARATOR
    
    btree.insert(30, 96);
    btree.insert(15, 108);
    btree.insert(1, 120);
    btree.insert(5, 132);

    btree.display();

    std::cout << "Found " << 3 << " at: " <<  btree.search(3) << '\n';
    std::cout << "Found " << 19 << " at: " <<  btree.search(19) << '\n';
    std::cout << "Found " << 30 << " at: " <<  btree.search(30) << '\n';
    std::cout << "Found " << 2 << " at: " <<  btree.search(2) << '\n';
    std::cout << "Found " << 8 << " at: " <<  btree.search(8) << '\n';
    std::cout << "Found " << 32 << " at: " <<  btree.search(32) << '\n';
    
    SEPARATOR
    
    btree.insert(2, 144);

    btree.display();
    
    std::cout << "Found " << 3 << " at: " <<  btree.search(3) << '\n';
    std::cout << "Found " << 19 << " at: " <<  btree.search(19) << '\n';
    std::cout << "Found " << 30 << " at: " <<  btree.search(30) << '\n';
    std::cout << "Found " << 2 << " at: " <<  btree.search(2) << '\n';
    std::cout << "Found " << 8 << " at: " <<  btree.search(8) << '\n';
    std::cout << "Found " << 32 << " at: " <<  btree.search(32) << '\n';
    
    SEPARATOR

    btree.insert(8, 156);
    btree.insert(9, 168);
    btree.insert(6, 180);
    btree.insert(11, 192);
    btree.insert(12, 204);
    btree.insert(17, 216);
    btree.insert(18, 228);
    
    btree.display();

    std::cout << "Found " << 3 << " at: " <<  btree.search(3) << '\n';
    std::cout << "Found " << 19 << " at: " <<  btree.search(19) << '\n';
    std::cout << "Found " << 30 << " at: " <<  btree.search(30) << '\n';
    std::cout << "Found " << 2 << " at: " <<  btree.search(2) << '\n';
    std::cout << "Found " << 8 << " at: " <<  btree.search(8) << '\n';
    std::cout << "Found " << 32 << " at: " <<  btree.search(32) << '\n';
    
    SEPARATOR

    btree.insert(32, 240);
    
    btree.display();

    std::cout << "Found " << 3 << " at: " <<  btree.search(3) << '\n';
    std::cout << "Found " << 19 << " at: " <<  btree.search(19) << '\n';
    std::cout << "Found " << 30 << " at: " <<  btree.search(30) << '\n';
    std::cout << "Found " << 2 << " at: " <<  btree.search(2) << '\n';
    std::cout << "Found " << 8 << " at: " <<  btree.search(8) << '\n';
    std::cout << "Found " << 32 << " at: " <<  btree.search(32) << '\n';
}
