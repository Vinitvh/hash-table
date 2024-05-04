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

typedef struct Linkedlist {
  HT_Item *item;
  struct Linkedlist *next;
} Linkedlist;

typedef struct HashTable {
  HT_Item **items;
  Linkedlist **overflow_buckets;
} HashTable;

unsigned int hash(char *key) {
  unsigned int hash = 0;
  for (unsigned int i = 0; i < strlen(key); i++) {
    hash += (i * key[i]) / 31;
  }
  return hash % HASHTABLE_SIZE;
}

Linkedlist *allocate_list_memory() {
  Linkedlist *list = malloc(sizeof(Linkedlist));
  if (list == NULL) {
    fprintf(stderr, "Failed to allocate memory for Linkedlist");
    exit(1);
  }
  return list;
}

Linkedlist *list_insert(Linkedlist *list, HT_Item *item) {
  if (!list) {
    Linkedlist *head = allocate_list_memory();
    head->item = item;
    head->next = NULL;
    list = head;
    return list;
  } else if (list->next) {
    Linkedlist *node = allocate_list_memory();
    node->item = item;
    node->next = NULL;
    list = node;
    return list;
  }
  Linkedlist *temp = list;
  while (temp->next->next) {
    temp = temp->next;
  }
  Linkedlist *node = allocate_list_memory();
  node->item = item;
  node->next = NULL;
  temp->next = node;
  return list;
}

void free_linkedlist(Linkedlist *list) {
  Linkedlist *temp = list;
  while (list) {
    temp = list;
    list = list->next;
    free(temp->item);
    free(temp);
  }
}

Linkedlist **create_overflow_buckets() {
  // Create an array of linkedlists
  Linkedlist **buckets = calloc(HASHTABLE_SIZE, sizeof(Linkedlist *));
  for (int i = 0; i < HASHTABLE_SIZE; i++)
    buckets[i] = NULL;
  return buckets;
}

void free_overflow_buckets(HashTable *table) {
  Linkedlist **buckets = table->overflow_buckets;
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    free_linkedlist(buckets[i]);
  }
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
  HashTable *table = malloc(sizeof(HashTable));
  table->items = calloc(HASHTABLE_SIZE, sizeof(HT_Item));
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    table->items[i] = NULL;
  }

  table->overflow_buckets = create_overflow_buckets();

  return table;
}

void free_table(HashTable *table) {
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    HT_Item *item = table->items[i];

    if (item != NULL) {
      free(item);
    }
  }
  free_overflow_buckets(table);
  free(table->items);
  free(table);
}

// Handle collision by linear probing
/* void handle_collision(HashTable *table, HT_Item *item) {
   unsigned int index = hash(item->key);
   for (int i = 0; i < HASHTABLE_SIZE; i++) {
     if (table->items[i] != NULL) {
       index = (index + i) % HASHTABLE_SIZE;
     }
   }
   table->items[index] = item;
} */

// Handle collision by Seperate chaining
void handle_collision(HashTable *table, HT_Item *item) {
  unsigned int index = hash(item->key);
  Linkedlist *head = table->overflow_buckets[index];
  if (head == NULL) {
    head = allocate_list_memory();
    head->item = item;
    table->overflow_buckets[index] = head;
    return;
  } else {
    table->overflow_buckets[index] = list_insert(head, item);
    return;
  }
}

void ht_insert(HashTable *table, char *key, char *val) {
  HT_Item *item = create_item(key, val);
  unsigned int index = hash(key);
  HT_Item *current_item = table->items[index];
  if (current_item == NULL) {
    table->items[index] = item;
  } else {
    if (strcmp(current_item->key, key) == 0) {
      snprintf(current_item->val, strlen(val) + 1, "%s", val);
      return;
    } else {
      handle_collision(table, item);
      return;
    }
  }
}

char *ht_search(HashTable *table, char *key) {
  unsigned int index = hash(key);
  HT_Item *item = table->items[index];
  Linkedlist *head = table->overflow_buckets[index];
  if (item != NULL) {
    if (strcmp(item->key, key) == 0) {
      return item->val;
    }
    if (!head) {
      return NULL;
    }

    item = head->item;
    head = head->next;
  }
  return NULL;
}

void print_table(HashTable *table) {
  for (int i = 0; i < HASHTABLE_SIZE; i++) {
    if (table->items[i]) {
      printf("%d. %s\n", i, table->items[i]->val);
    } else {
      printf("%d. ____\n", i);
    }
  }
}

int main(void) {
  HashTable *table = create_table();
  ht_insert(table, "one", "50");
  ht_insert(table, "three", "40");
  print_table(table);
  // char *val = ht_search(table, "one");
  // printf("%s\n", val);
  free_table(table);
  return 0;
}
