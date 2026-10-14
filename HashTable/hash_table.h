
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define HASH_SIZE 100

struct HashTable
{
    int caseId[HASH_SIZE];
    int occupied[HASH_SIZE];
};

void initializeHashTable(struct HashTable* ht);

int hashFunction(int caseId);

void insertHashCase(struct HashTable* ht, int caseId);

int searchHashCase(struct HashTable* ht, int caseId);

void deleteHashCase(struct HashTable* ht, int caseId);

void displayHashTable(struct HashTable* ht);

#endif
