#include <stdio.h>
#include "hash_table.h"

void initializeHashTable(struct HashTable* ht)
{
    int i;

    for (i = 0; i < HASH_SIZE; i++)
    {
        ht->caseId[i] = -1;
        ht->occupied[i] = 0;
    }
}

int hashFunction(int caseId)
{
    return caseId % HASH_SIZE;
}

void insertHashCase(struct HashTable* ht, int caseId)
{
    int index;
    int startIndex;

    index = hashFunction(caseId);
    startIndex = index;

    while (ht->occupied[index] == 1)
    {
        if (ht->caseId[index] == caseId)
        {
            printf("\nCase ID already exists in Hash Table!\n");
            return;
        }

        index = (index + 1) % HASH_SIZE;

        if (index == startIndex)
        {
            printf("\nHash Table is full!\n");
            return;
        }
    }

    ht->caseId[index] = caseId;
    ht->occupied[index] = 1;

    printf("\nCase %d added to Hash Table at index %d.\n",
           caseId, index);
}

int searchHashCase(struct HashTable* ht, int caseId)
{
    int index;
    int startIndex;

    index = hashFunction(caseId);
    startIndex = index;

    while (ht->occupied[index] == 1 ||
           ht->caseId[index] == -2)
    {
        if (ht->occupied[index] == 1 &&
            ht->caseId[index] == caseId)
        {
            return index;
        }

        index = (index + 1) % HASH_SIZE;

        if (index == startIndex)
        {
            break;
        }
    }

    return -1;
}

void deleteHashCase(struct HashTable* ht, int caseId)
{
    int index;

    index = searchHashCase(ht, caseId);

    if (index == -1)
    {
        printf("\nCase not found in Hash Table!\n");
        return;
    }

    /*
       -2 means deleted slot (tombstone).
       It keeps the collision chain working.
    */
    ht->caseId[index] = -2;
    ht->occupied[index] = 0;

    printf("\nCase %d deleted from Hash Table.\n", caseId);
}

void displayHashTable(struct HashTable* ht)
{
    int i;

    printf("\n===== HASH TABLE =====\n");

    for (i = 0; i < HASH_SIZE; i++)
    {
        if (ht->occupied[i] == 1)
        {
            printf("Index %d -> Case ID: %d\n",
                   i, ht->caseId[i]);
        }
        else if (ht->caseId[i] == -2)
        {
            printf("Index %d -> Deleted\n", i);
        }
        else
        {
            printf("Index %d -> Empty\n", i);
        }
    }
}