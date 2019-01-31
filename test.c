#include "prime.h"
#include "hash_table.h"
#include <stdio.h>

void test_insert(ht_hash_table *ht)
{
    ht_insert(ht, "10", "afaf");
    ht_insert(ht, "20", "afaefer");
    ht_insert(ht, "20", "afaxxxefer");
    ht_insert(ht, "30", "4rafcag");
    ht_insert(ht, "40", "afsb fer");
    ht_insert(ht, "50", "aACsxefer");
    ht_insert(ht, "60", "4rfdng");
}

void test_search(ht_hash_table *ht)
{
    printf("%s\n", ht_search(ht, "40"));
    printf("%s\n", ht_search(ht, "20"));
}

void test_delete(ht_hash_table *ht)
{
    ht_delete(ht, "30");
}

void test_output(ht_hash_table *ht)
{
    printf("size %d, count %d\n", ht->size, ht->count);
    for (int i = 0; i < ht->size; i++)
    {
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
        ht_item *item = ht->items[i];
        if (item == NULL)
        {
            printf("(:)\t\t\t");
        }
        else
        {
            printf("(%s:%s)\t\t\t", ht->items[i]->key, ht->items[i]->value);
        }
    }
}

int main()
{
    ht_hash_table *ht = ht_new_hash_table(next_prime(HT_INITIAL_SIZE));

    test_insert(ht);
    test_search(ht);
    test_delete(ht);
    test_output(ht);

    ht_del_hash_table(ht);
    return 0;
}