#include <iostream>
#include "functions.h"
#include "helpers.h"

int main() {
    char BTREE_FILENAME[] = "../btree";

//    CreateIndexFileFile(BTREE_FILENAME, 10, 5);

    std::fstream btree;
    openBTree(BTREE_FILENAME, btree);
    
    int r = 4 * ((2 * 5) + 1);
    for (int i = 0; i < 10; ++i)
        std::cout << isEmpty(i, r, btree) << '\n';

    closeBTree(btree);

    return 0;
}
