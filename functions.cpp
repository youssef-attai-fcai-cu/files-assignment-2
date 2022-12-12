#include "functions.h"
#include "constants.h"
#include "helpers.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

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
    int m = 5;
//  Record size
    const int recordSize = (CELL_SIZE * ((2 * m) + 1)); // TODO: Replace "5" with m

//  Open the B-tree file
    std::fstream btree;
    openBTree(filename, btree);

//  Seek to the beginning of the file
    btree.seekg(0, std::ios::beg);

//  TODO: Insertion logic

    int nextEmpty = getIndexOfNextEmptyRecord(btree);

//  If root is empty, insert in root
    if (nextEmpty == 1) {
//      Read new next empty from root
        char newNextEmpty[CELL_SIZE];
        btree.seekg(recordSize + CELL_SIZE, std::ios::beg);
        btree.read(newNextEmpty, CELL_SIZE);

//      Store new next empty in header
        btree.seekg(CELL_SIZE, std::ios::beg);
        btree << newNextEmpty;

//      Write pair in root node
        btree.seekg(recordSize + CELL_SIZE, std::ios::beg);
        btree << pad(std::to_string(RecordID), CELL_SIZE)
              << pad(std::to_string(Reference), CELL_SIZE);

//      Update root's leaf status to LEAF
        btree.seekg(recordSize, std::ios::beg);
        btree << pad("0", CELL_SIZE);
    } else {  //  Otherwise, if root is NOT empty

        /* Traverse B-tree */

//      Seek to root (skip header)
        btree.seekg(recordSize, std::ios::beg);

//      Keep track of visited cells
        std::stack<int> visited;

//      Traverse till a leaf is reached
        int currentRecordIndex = 1;
        while (!isLeaf(currentRecordIndex, recordSize, btree)) {
            for (int i = 0; i < m; ++i) {
                char key[CELL_SIZE], reference[CELL_SIZE];
                btree.read(key, CELL_SIZE);
                btree.read(reference, CELL_SIZE);
                int k = ctoi(key);
                int r = ctoi(reference);

                if (k == -1 && r == -1) break;

                currentRecordIndex = r;

//              Update visited cells
                visited.push((int) btree.tellg() - (2 * CELL_SIZE));

                if (RecordID < k) break;
            }
        }

        /* Leaf reached, insert pair in record at currentRecordIndex */

//      Read node into memory
        std::vector<std::pair<int, int>> node = readNode(currentRecordIndex, recordSize, m, btree);

        if (node.size() < m) {
//          Insert into node
            node.emplace_back(std::make_pair(RecordID, Reference));

//          Sort node on pair.first
            std::sort(node.begin(), node.end());

//          Write node
            writeNode(node, currentRecordIndex, recordSize, btree);
        } else {
//          
        }


//      Update visited parents
        while (!visited.empty()) {
//          Get the latest visited parent cell
            int parentCell = visited.top();
            visited.pop();

//          Get direct child node to this parent
            btree.seekg(parentCell + CELL_SIZE);
            char childNodeIndex[CELL_SIZE];
            btree.read(childNodeIndex, CELL_SIZE);
            node = readNode(ctoi(childNodeIndex), recordSize, m, btree);

//          Get maximum pair in the child node
            std::pair<int, int> maxPair = *std::max_element(node.begin(), node.end());

//          Override new maximum in parent
            btree.seekg(parentCell);
            btree << pad(std::to_string(maxPair.first), CELL_SIZE);
        }
    }

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
                Sort node
                Write node
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
        for (char i: record)
            std::cout << i;
        std::cout << '\n';
    }

//  Close B-tree file
    closeBTree(btree);
}

int SearchARecord(char *filename, int RecordID) {
    return 0;
}
