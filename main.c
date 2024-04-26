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

unsigned int hash(char *key) {
  unsigned int hash = 0;
  for (unsigned int i = 0; i < strlen(key); i++) {
    hash += (i * key[i]) / 31;
  }
  return hash % HASHTABLE_SIZE;
}

HT_Item *create_item(char *key, char *val) {
  HT_Item *item = malloc(sizeof(HT_Item));
  item->key = malloc(strlen(key) + 1);
  item->val = malloc(strlen(val) + 1);
  strcpy(item->key, key);
  strcpy(item->val, val);
  return item;
}

HashTable *create_table() {
  HashTable *table = (HashTable *)malloc(sizeof(HashTable));
  table->items = (HT_Item *)calloc(HASHTABLE_SIZE, sizeof(HT_Item));
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

HashTable *insert(HashTable *table, char *key, char *val) {
  unsigned int index = hash(key);
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    // if (table->items != NULL) {
    //   printf("Space occupied");
    //   exit(1);
    // }
    strncpy(table->items[index]->val, val, strlen(val));
  }
  return table;
}

void print_table(HashTable *table) {
  if (table == NULL) {
    printf("No hashtable to print");
  }
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    if (table->items[i]) {
      printf("%s\n", table->items[i]->val);
    } else {
      printf("%d. _____\n", i);
    }
  }
}

int main(void) {
  HashTable *table = create_table();
  insert(table, "one", "10");
  print_table(table);
  return 0;
}
