// hash_table.h

#ifndef hash_table_h
#define hash_table_h

//---------------------------------------------------------------------------

typedef struct
{
    char *key;
    char *value;
} ht_item;

ht_item *ht_new_item(const char *k, const char *v);
void ht_del_item(ht_item *i);

//---------------------------------------------------------------------------

typedef struct
{
    int size;
    int count;
    ht_item **items;
} ht_hash_table;

ht_hash_table *ht_new_hash_table(const int size);
void ht_del_hash_table(ht_hash_table *ht);

//---------------------------------------------------------------------------

#define HT_PRIME_1 79
#define HT_PRIME_2 59
int ht_hash(const char *s, const int a, const int m);
int ht_get_hash(const char *s, const int num_buckets, const int attempt);

void ht_delete(ht_hash_table *ht, const char *key);
void ht_insert(ht_hash_table *ht, const char *key, const char *value);
char *ht_search(ht_hash_table *ht, const char *key);

//---------------------------------------------------------------------------

#define HT_INITIAL_SIZE 50 // next_prime is 53
void ht_resize(ht_hash_table *ht, const int to_size);
void ht_resize_up(ht_hash_table *ht);
void ht_resize_down(ht_hash_table *ht);

//---------------------------------------------------------------------------

#endif