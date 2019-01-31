// hash_table.c

#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//---------------------------------------------------------------------------

ht_item *ht_new_item(const char *k, const char *v)
{
    ht_item *i = (ht_item *)malloc(sizeof(ht_item));
    if (i == NULL)
    {
        printf("new an item error!\n");
        exit(-1);
    }

    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

void ht_del_item(ht_item *i)
{
    free(i->key);
    free(i->value);
    free(i);
}

//---------------------------------------------------------------------------

ht_hash_table *ht_new_hash_table(const int size)
{
    ht_hash_table *ht = (ht_hash_table *)malloc(sizeof(ht_hash_table));
    if (ht == NULL)
    {
        printf("new a hash table error!\n");
        exit(-1);
    }

    ht->size = size;
    ht->count = 0;
    ht->items = (ht_item **)calloc(size, sizeof(ht_item *));
    return ht;
}

void ht_del_hash_table(ht_hash_table *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        ht_item *item = ht->items[i];
        if (item != NULL)
        {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

//---------------------------------------------------------------------------

int ht_hash(const char *s, const int a, const int m)
{
    long hash = 0;
    const int len_s = strlen(s);
    for (size_t i = 0; i < len_s; i++)
    {
        hash += (long)pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

int ht_get_hash(const char *s, const int num_buckets, const int attempt)
{
    int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + attempt * (hash_b == 0 ? 1 : hash_b)) % num_buckets;
}

static ht_item HT_DELETED_ITEM = {NULL, NULL};
void ht_delete(ht_hash_table *ht, const char *key)
{
    const int load = ht->count * 100 / ht->size;
    if (load < 10)
    {
        ht_resize_down(ht);
    }

    int index = ht_get_hash(key, ht->size, 0);
    ht_item *item = ht->items[index];
    int i = 1;
    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM)
        {
            if (strcmp(item->key, key) == 0)
            {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
                ht->count--;
                return;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
}

void ht_insert(ht_hash_table *ht, const char *key, const char *value)
{
    const int load = ht->count * 100 / ht->size;
    if (load > 70)
    {
        ht_resize_up(ht);
    }

    ht_item *item = ht_new_item(key, value); // item waiting to insert.

    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item *cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL)
    {
        if (cur_item != &HT_DELETED_ITEM)
        { // jump over HT_DELETED_ITEM
            if (strcmp(item->key, cur_item->key) == 0)
            { // same key, cover it!
                ht_del_item(cur_item);
                ht->items[index] = item;
                return;
            } // two different keys may result in the same hash value, then get next index.
        }
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    }

    // cur_item == NULL, insert!
    ht->items[index] = item;
    ht->count += 1;
}

char *ht_search(ht_hash_table *ht, const char *key)
{
    int index = ht_get_hash(key, ht->size, 0);
    ht_item *item = ht->items[index];
    int i = 1;
    while (item != NULL)
    {
        if (item != &HT_DELETED_ITEM)
        {
            if (strcmp(item->key, key) == 0)
            {
                return item->value;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL;
}

//---------------------------------------------------------------------------

void ht_resize(ht_hash_table *ht, const int to_size)
{
    if (to_size < HT_INITIAL_SIZE)
    {
        return;
    }

    ht_hash_table *new_ht = ht_new_hash_table(to_size);
    for (int i = 0; i < ht->size; i++)
    {
        ht_item *item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM)
        {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht_hash_table *tmp = ht;
    ht = new_ht;
    new_ht = tmp;

    ht_del_hash_table(new_ht);
}

void ht_resize_up(ht_hash_table *ht)
{
    const int new_size = ht->size * 2;
    ht_resize(ht, new_size);
}

void ht_resize_down(ht_hash_table *ht)
{
    const int new_size = ht->size / 2;
    ht_resize(ht, new_size);
}
