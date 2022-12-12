#include "functions.h"


int main() {
    char BTREE_FILENAME[] = "../btree";

//    CreateIndexFileFile(BTREE_FILENAME, 10, 5);
//    
//    InsertNewRecordAtIndex(BTREE_FILENAME, 3, 12);
//    InsertNewRecordAtIndex(BTREE_FILENAME, 7, 24);
//    InsertNewRecordAtIndex(BTREE_FILENAME, 10, 48);
//    InsertNewRecordAtIndex(BTREE_FILENAME, 24, 60);
//    InsertNewRecordAtIndex(BTREE_FILENAME, 14, 72);
    
    InsertNewRecordAtIndex(BTREE_FILENAME, 33, 13);
    
    DisplayIndexFileContent(BTREE_FILENAME);

    return 0;
}
