#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHTABLE_SIZE 10

// HashTable item
typedef struct HT_Item {
  char key[256];
  char val[512];
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
  if (item == NULL) {
    fprintf(stderr, "Failed to allocate memory for item");
    exit(1);
  }
  snprintf(item->key, strlen(key) + 1, "%s", key);
  snprintf(item->val, strlen(val) + 1, "%s", val);
  return item;
}

HashTable *create_table() {
  HashTable *table = (HashTable *)malloc(sizeof(HashTable));
  table->items = (HT_Item **)calloc(HASHTABLE_SIZE, sizeof(HT_Item));
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    table->items[i] = NULL;
  }

  return table;
}

void free_table(HashTable *table) {
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    HT_Item *item = table->items[i];

    if (item != NULL) {
      free(item);
    }
  }

  free(table->items);
  free(table);
}

void handle_collision(HashTable *table, char *key, char *val) {
  unsigned int index = hash(key);
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    if (table->items[i] != NULL) {
      index = (index + i) % HASHTABLE_SIZE;
    }
  }
  HT_Item *item = create_item(key, val);
  table->items[index] = item;
}

void insert(HashTable *table, char *key, char *val) {
  HT_Item *item = create_item(key, val);
  unsigned int index = hash(key);
  HT_Item *current_item = table->items[index];
  if (current_item == NULL) {
    table->items[index] = item;
  } else {
    handle_collision(table, key, val);
  }
}

char *search(HashTable *table, char *key) {
  unsigned int index = hash(key);
  HT_Item *item = table->items[index];
  if (item != NULL) {
    if (strcmp(item->key, key) == 0) {
      return item->val;
    }
  }
  return NULL;
}

void print_table(HashTable *table) {
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    if (table->items[i]) {
      printf("%d. %s\n", i, table->items[i]->val);
    } else {
      printf("%d. -----\n", i);
    }
  }
}

int main(void) {
  HashTable *table = create_table();
  insert(table, "one", "50");
  insert(table, "one", "40");
  print_table(table);
  // char *val = search(table, "one");
  // printf("%s\n", val);
  free_table(table);
  return 0;
}
