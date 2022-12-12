#include "functions.h"


int main() {
    char BTREE_FILENAME[] = "../btree";

    InsertNewRecordAtIndex(BTREE_FILENAME, 3, 12);
    InsertNewRecordAtIndex(BTREE_FILENAME, 7, 24);
    InsertNewRecordAtIndex(BTREE_FILENAME, 10, 48);
    InsertNewRecordAtIndex(BTREE_FILENAME, 24, 60);
    InsertNewRecordAtIndex(BTREE_FILENAME, 14, 72);
    
    DisplayIndexFileContent(BTREE_FILENAME);

    return 0;
}
