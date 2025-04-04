/** 
 * @file map.h
 * @author Sean Leana
 * This file giver an alias for the MapStruct  
 */

#ifndef MAP_H
#define MAP_H

#include "value.h"
#include <stdbool.h>

/** Incomplete type for the Map representation. */
typedef struct MapStruct Map;

// Maximum length of a key.
#define KEY_LIMIT 24

/**
 * This function creates an empty dynamically allocated map, which initializes its fields and returns a pointer to the map
 * @param len number of elements in the hash table
 * @return a pointer to the map it creates
 */
Map *makeMap(int len);

/**
 * returns the size of a map
 * @param m map to return size of
 * @return size of map as int
 */
int mapSize(Map *m);

/**
 * sets a key and value inside a map, if the key already exist then the value is updated, if the key doesnt exist the key is added
 * @param m pointer to map to set key value pair to
 * @param key pointer to key to set inside map with val
 * @param val pointer to val to set inside map with key
 */
void mapSet(Map *m, char const *key, Value *val);

/** 
 * gets the value associated with the given key and map
 * @param m pointer to map to get value from
 * @param key pointer to key to get its value from
 * @return pointer to value associated with key
 */
Value *mapGet(Map *m, char const *key);

/**
 * removes the key and value inside of the map and frees memory for that pair if it exists
 * @param map pointer to map to remove key from
 * @param key key to remove from map
 * @return true if key is removed from map
 */
bool mapRemove (Map *map, char const *key);

/**
 * frees the map m of all memory associated with it, including memory for the hash table all the nodes and all the value objects used by the nodes
 * @param m map to remove memory from
 */
void freeMap(Map *m);







#endif
