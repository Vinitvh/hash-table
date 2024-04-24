#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHTABLE_SIZE 10

// HashTable item
typedef struct HT_Item {
  char *key;
  char *val;
} HT_Item;

// HashTable
typedef struct HashTable {
  HT_Item **items;
} HashTable;

HT_Item *create_item(char *key, char *val) {
  HT_Item *item = malloc(sizeof(HT_Item));
  item->key = malloc(strlen(key) + 1);
  item->val = malloc(strlen(val) + 1);
  strcpy(item->key, key);
  strcpy(item->val, val);
  return item;
}

HashTable *create_table() {
  HashTable *table = malloc(sizeof(HashTable));
  table->items = calloc(HASHTABLE_SIZE, sizeof(HT_Item));
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    table->items[i] = NULL;
  }

  return table;
}

void free_item(HT_Item *item) {
  free(item->key);
  free(item->val);
  free(item);
}

void free_table(HashTable *table) {
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    HT_Item *item = table->items[i];

    if (item != NULL) {
      free_item(item);
    }
  }

  free(table->items);
  free(table);
}

int main(void) {
  HT_Item *res = create_item("Hello", "10");
  printf("%s\n", res->val);
  return 0;
}
