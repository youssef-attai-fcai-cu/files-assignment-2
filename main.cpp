#include <iostream>
#include "helpers.h"

int main() {
    char BTREE_FILENAME[] = "../btree";

    std::fstream btree;
    openBTree(BTREE_FILENAME, btree);

    int r = 4 * ((2 * 5) + 1);
    for (int i = 0; i < 10; ++i)
        std::cout << isLeaf(i, r, btree) << '\n';


    closeBTree(btree);

    return 0;
}
