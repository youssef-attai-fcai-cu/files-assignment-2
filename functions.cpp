#include "functions.h"
#include "constants.h"
#include "helpers.h"
#include <iostream>

void CreateIndexFileFile(char *filename, int numberOfRecords, int m) {
//  Record size
    const int recordSize = (CELL_SIZE * ((2 * m) + 1));

//  Open the B-tree file
    std::fstream btree;
    openBTree(filename, btree);

//  For each record
    for (int i = 0; i < numberOfRecords; ++i) {
//      Seek to the beginning of the record
        btree.seekg(i * recordSize);

//      Add leaf status cell
        btree << pad("-1", CELL_SIZE);

//      Add index of the next empty cell
        btree << pad(std::to_string(i + 1), CELL_SIZE);

//      Fill rest of record with -1s
        for (int j = 0; j < (2 * m) - 1; ++j)
            btree << pad("-1", CELL_SIZE);
    }

//  Close B-tree file
    closeBTree(btree);
}

int InsertNewRecordAtIndex(char *filename, int RecordID, int Reference) {
//  Record size
    const int recordSize = (CELL_SIZE * ((2 * 5) + 1)); // TODO: Replace "5" with m

//  Open the B-tree file
    std::fstream btree;
    openBTree(filename, btree);
    
    
//  TODO: Insertion logic

/*
    Check root
    If (root is empty) Insert in root
    Otherwise {
        While (current node is non-leaf) Traverse the B-tree
        Read node in memory
        If (Node is not full) {
            Insert new pair <RecordID, Reference>
            Sort node
            Write node
        } Otherwise {
            // There is at least 1 empty record
            If (Next empty cell value != -1) {
                                       // At least 2 empty records
                If (Parent is full AND number of empty records > 1) Split parent
                Otherwise Abort
                Split node
                Insert new pair <RecordID, Reference>
                Adjust parent (i.e. insert new pair in parent)
            } Otherwise Abort
        }
    }
*/


//  Close B-tree file
    closeBTree(btree);
    return 0;
}

void DeleteRecordFromIndex(char *filename, int RecordID) {

}

void DisplayIndexFileContent(char *filename) {
//  Record size
    const int recordSize = (CELL_SIZE * ((2 * 5) + 1));

//  Open the B-tree file
    std::fstream btree;
    openBTree(filename, btree);

//  Calculate the number of records in the file
    int numberOfRecords = getFileSize(btree) / recordSize;

//  Seek to the beginning of the file
    btree.seekg(0, std::ios::beg);
    
//  For each record
    for (int j = 0; j < numberOfRecords; ++j) {
//      Read the record
        char record[recordSize];
        btree.read(record, recordSize);

//      Instead of printing the array of characters, print each character one by one
//      to avoid weird null-terminator issue
        for (char i : record)
            std::cout << i;
        std::cout << '\n';
    }
        
//  Close B-tree file
    closeBTree(btree);
}

int SearchARecord(char *filename, int RecordID) {
    return 0;
}
