
#include <stdio.h>
#include "HashTable/hash_table.h"

int main(void)
{
    struct HashTable ht;
    int result;

    initializeHashTable(&ht);

    printf("\nTEST 1: Insert colliding case IDs\n");

    insertHashCase(&ht, 17);
    insertHashCase(&ht, 117);

    result = searchHashCase(&ht, 17);
    printf("Search 17: %s\n",
           result != -1 ? "PASS" : "FAIL");

    result = searchHashCase(&ht, 117);
    printf("Search 117: %s\n",
           result != -1 ? "PASS" : "FAIL");

    printf("\nTEST 2: Delete the first colliding case\n");

    deleteHashCase(&ht, 17);

    result = searchHashCase(&ht, 17);
    printf("Deleted case 17 not found: %s\n",
           result == -1 ? "PASS" : "FAIL");

    result = searchHashCase(&ht, 117);
    printf("Case 117 still found: %s\n",
           result != -1 ? "PASS" : "FAIL");

    printf("\nTEST 3: Reuse a deleted slot\n");

    insertHashCase(&ht, 217);

    result = searchHashCase(&ht, 217);
    printf("Search 217 after insertion: %s\n",
           result != -1 ? "PASS" : "FAIL");

    printf("\nTEST 4: Reject a duplicate\n");

    insertHashCase(&ht, 117);

    result = searchHashCase(&ht, 117);
    printf("Case 117 remains searchable: %s\n",
           result != -1 ? "PASS" : "FAIL");

    printf("\nTEST 5: Reject invalid IDs\n");

    insertHashCase(&ht, -5);

    result = searchHashCase(&ht, -5);
    printf("Negative ID not found: %s\n",
           result == -1 ? "PASS" : "FAIL");

    printf("\n========== FINAL HASH TABLE ==========\n");
    displayHashTable(&ht);

    return 0;
}
