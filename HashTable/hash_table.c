
#include <stdio.h>
#include "hash_table.h"

/*
    Slot meanings:
    occupied = 1 : slot contains a case
    caseId = -1  : slot is empty
    caseId = -2  : slot was deleted (tombstone)
*/

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
    int index = caseId % HASH_SIZE;

    if (index < 0)
    {
        index += HASH_SIZE;
    }

    return index;
}

void insertHashCase(struct HashTable* ht, int caseId)
{
    int index;
    int startIndex;
    int firstDeleted = -1;

    if (caseId <= 0)
    {
        printf("\nInvalid Case ID. Case ID must be positive.\n");
        return;
    }

    /*
        Search the entire relevant probe chain first.
        This prevents duplicates when a deleted slot exists.
    */
    if (searchHashCase(ht, caseId) != -1)
    {
        printf("\nCase ID already exists in Hash Table!\n");
        return;
    }

    index = hashFunction(caseId);
    startIndex = index;

    do
    {
        if (ht->occupied[index] == 1)
        {
            /* This slot contains another case. */
        }
        else if (ht->caseId[index] == -2)
        {
            /* Remember the first deleted slot for reuse. */
            if (firstDeleted == -1)
            {
                firstDeleted = index;
            }
        }
        else
        {
            /* Found a genuinely empty slot. */
            if (firstDeleted != -1)
            {
                index = firstDeleted;
            }

            ht->caseId[index] = caseId;
            ht->occupied[index] = 1;

            printf("\nCase %d added to Hash Table at index %d.\n",
                   caseId, index);
            return;
        }

        index = (index + 1) % HASH_SIZE;

    } while (index != startIndex);

    /*
        The table may have no empty slots but still have a tombstone.
        Reuse the tombstone if one was found.
    */
    if (firstDeleted != -1)
    {
        ht->caseId[firstDeleted] = caseId;
        ht->occupied[firstDeleted] = 1;

        printf("\nCase %d added to Hash Table at index %d.\n",
               caseId, firstDeleted);
        return;
    }

    printf("\nHash Table is full! Cannot insert Case %d.\n", caseId);
}

int searchHashCase(struct HashTable* ht, int caseId)
{
    int index;
    int startIndex;

    if (caseId <= 0)
    {
        return -1;
    }

    index = hashFunction(caseId);
    startIndex = index;

    do
    {
        if (ht->occupied[index] == 1 &&
            ht->caseId[index] == caseId)
        {
            return index;
        }

        /*
            Stop only at a genuinely empty slot.
            Do not stop at a tombstone because the case
            may occur later in the collision chain.
        */
        if (ht->occupied[index] == 0 &&
            ht->caseId[index] == -1)
        {
            return -1;
        }

        index = (index + 1) % HASH_SIZE;

    } while (index != startIndex);

    return -1;
}

void deleteHashCase(struct HashTable* ht, int caseId)
{
    int index = searchHashCase(ht, caseId);

    if (index == -1)
    {
        printf("\nCase not found in Hash Table!\n");
        return;
    }

    /*
        Preserve the probe chain with a tombstone.
    */
    ht->caseId[index] = -2;
    ht->occupied[index] = 0;

    printf("\nCase %d deleted from Hash Table.\n", caseId);
}

void displayHashTable(struct HashTable* ht)
{
    int i;

    printf("\n========== HASH TABLE ==========\n");
    printf("Capacity: %d\n\n", HASH_SIZE);

    for (i = 0; i < HASH_SIZE; i++)
    {
        if (ht->occupied[i] == 1)
        {
            printf("Index %2d -> Case ID: %d\n",
                   i, ht->caseId[i]);
        }
        else if (ht->caseId[i] == -2)
        {
            printf("Index %2d -> Deleted\n", i);
        }
        else
        {
            printf("Index %2d -> Empty\n", i);
        }
    }

    printf("================================\n");
}
