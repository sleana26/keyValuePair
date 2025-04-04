/**
 * @file map.c
 * @author Sean Leana (smleana)
 * The map file is able to make a map and adjust a map by removing and setting. Also getting values and the size of the map.
 * It also handles allocating and freeing memory
 */
#include "map.h"
#include <stdlib.h>
#include <string.h>

/** Node containing a key / value pair. */
typedef struct NodeStruct {
  /** String key for this map entry. */
  char key[ KEY_LIMIT + 1 ];
  
  /** Pointer to the value part of the key / value pair. */
  Value *val;
  
  /** Pointer to the next node at the same element of this table. */
  struct NodeStruct *next;
} Node;

/** Representation of a hash table implementation of a map. */
struct MapStruct {
  /** Table of key / value pairs. */
  Node **table;
  /** Current length of the table. */
  int tlen;
  /** Current size of the map (number of different keys). */
  int size;
};

/**
 * This function creates an empty dynamically allocated map, which initializes its fields and returns a pointer to the map
 * @param len number of elements in the hash table
 * @return a pointer to the map it creates
 */
Map *makeMap(int len)
{
  Map *map = (Map *)malloc(sizeof(Map));
  map->table = (Node **)malloc(sizeof(Node *) * len);

  for (int i = 0; i < len; i++) {
        map->table[i] = NULL;
  }
  
  map->tlen = len;
  map->size = 0;
  return map;
}

/**
 * returns the size of a map
 * @param m map to return size of
 * @return size of map as int
 */
int mapSize(Map *m)
{
  return m->size;
}

/**
 * sets a key and value inside a map, if the key already exist then the value is updated, if the key doesnt exist the key is added
 * @param m pointer to map to set key value pair to
 * @param key pointer to key to set inside map with val
 * @param val pointer to val to set inside map with key
 */
void mapSet(Map *m, char const *key, Value *val)
{
  //calculate hash
  unsigned int hash = 0;
  size_t i = 0;
  while (i != strlen(key)) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  
  hash = hash % (m->tlen);

  //iterate through proper linked list to check for key and set value if key is found, if key isnt found
  Node *current = m->table[hash];
  while (current) {
    if (strcmp(current->key, key) == 0) {
      current->val = val;
      return;
    }
    current = current->next;
  }

  Node *newNode = (Node *)malloc(sizeof(Node));
  strcpy(newNode->key, key);
  newNode->val = val;
  newNode->next = m->table[hash]; // Insert at the head of the list
  m->table[hash] = newNode;
  // Update map size
  m->size++;
}

/** 
 * gets the value associated with the given key and map
 * @param m pointer to map to get value from
 * @param key pointer to key to get its value from
 * @return pointer to value associated with key
 */
Value *mapGet(Map *m, char const *key)
{
  //calculate hash
  unsigned int hash = 0;
  size_t i = 0;
  while (i != strlen(key)) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  
  hash = hash % (m->tlen);

  //traverse
  Node *current = m->table[hash];
  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      // if found return value
      return current->val;
    }
    current = current->next;
  }
    // if not found
    return NULL;
}

/**
 * removes the key and value inside of the map and frees memory for that pair if it exists
 * @param map pointer to map to remove key from
 * @param key key to remove from map
 * @return true if key is removed from map
 */
bool mapRemove (Map *map, char const *key)
{
  //calculate hash
  unsigned int hash = 0;
  size_t i = 0;
  while (i != strlen(key)) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  
  hash = hash % (map->tlen);

  Node *current = map->table[hash];
  Node *prev = NULL;
  while (current) {
    if (strcmp(current->key, key) == 0) {
      // Found the key
      if (prev) {
        prev->next = current->next; // Remove the Node from the list
      } else {
        map->table[hash] = current->next; // Update the bucket head
      }
      free(current);      // Free the Node
      map->size--;
      return true;
    }
    prev = current;
    current = current->next;
  }
  return false;
}

/**
 * frees the map m of all memory associated with it, including memory for the hash table all the nodes and all the value objects used by the nodes
 * @param m map to remove memory from
 */
void freeMap(Map *m)
{
  if(m == NULL)
    return;
    
  // Iterate through each bucket in the hash table
  for (int i = 0; i < m->tlen; i++) {
    Node *current = m->table[i];
    while (current != NULL) {
      Node *next = current->next;
      // Free the value
      if (current->val != NULL) {
        free(current->val);
      }
      // Free the current node
      free(current);
      // Move to the next node
      current = next;
    }
  }

  // Free the hash table array
  free(m->table);

  // Free the map structure itself
  free(m);
}