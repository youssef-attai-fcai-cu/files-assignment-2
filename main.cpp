#include "functions.h"


int main() {
    char BTREE_FILENAME[] = "../btree";
    
    CreateIndexFileFile(BTREE_FILENAME, 10, 5);
    DisplayIndexFileContent(BTREE_FILENAME);
    
    return 0;
}
