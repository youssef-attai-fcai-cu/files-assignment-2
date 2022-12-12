#ifndef ASSIGNMENT_2_FUNCTIONS_H
#define ASSIGNMENT_2_FUNCTIONS_H

void CreateIndexFileFile(char *filename, int numberOfRecords, int m);

// insert function should return -1 if there is no place to insert the record or 
// the index of the node where the new record is inserted if the record was inserted successfully.
int InsertNewRecordAtIndex(char *filename, int RecordID, int Reference);

void DeleteRecordFromIndex(char *filename, int RecordID);

// this method should display content of the file, each node in a line.
void DisplayIndexFileContent(char *filename);

// this method should return -1 if the record does not exist in the index or 
// the reference value to the data file if the record exist on the index 
int SearchARecord(char *filename, int RecordID);

#endif //ASSIGNMENT_2_FUNCTIONS_H
