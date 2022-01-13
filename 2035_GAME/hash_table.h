
#ifndef HASHTABLE_H
#define HASHTABLE_H


 /**
  * This defines a type that is a pointer to a function which takes
  * an unsigned int argument and returns an unsigned int value.
  * The name of the type is "HashFunction".
  */
typedef unsigned int (*HashFunction)(unsigned int key);

/**
 * This defines a type that is a _HashTable struct. 
 */
typedef struct _HashTable HashTable;

/**
 * This defines a type that is a _HashTableEntry struct. The definition for
 * _HashTableEntry is implemented in hash_table.c. 
 */
typedef struct _HashTableEntry HashTableEntry;

/**
 * createHashTable
 *
 * Creates a hash table by allocating memory for it on the heap. Initialize num_buckets
 * and hash based on function arguments.
 *
 * @param myHashFunc The pointer to the custom hash function.
 * @param numBuckets The number of buckets available in the hash table.
 * @return a pointer to the new hash table
 */
HashTable* createHashTable(HashFunction myHashFunc, unsigned int numBuckets);

/**
 * destroyHashTable
 *
 * Destroy the hash table. The nodes (HashTableEntry objects) of singly linked
 * list, the values stored on the linked list, the buckets, and the hashtable
 * itself are freed from the heap.
 *
 * @param myHashTable The pointer to the hash table.
 *
 */
void destroyHashTable(HashTable* myHashTable);

/**
 * insertItem
 *
 * Insert the value into the hash table based on the key.
 *
 * @param myHashTable The pointer to the hash table.
 * @param key The key that corresponds to the value.
 * @param value The value to be stored in the hash table.
 * @return old value if it is overwritten, or NULL if not replaced
 */
void* insertItem(HashTable* myHashTable, unsigned int key, void* value);

/**
 * getItem
 *
 * Get the value that corresponds to the key in the hash table.
 *
 * @param myHashTable The pointer to the hash table.
 * @param key The key that corresponds to the item.
 * @return the value corresponding to the key, or NULL if the key is not present
 */
void* getItem(HashTable* myHashTable, unsigned int key);

/**
 * removeItem
 *
 * Remove the item in hash table based on the key and return the value stored in it.
 *
 * @param myHashTable The pointer to the hash table.
 * @param key The key that corresponds to the item.
 * @return the pointer of the value corresponding to the key, or NULL if the key is not present
 */
void* removeItem(HashTable* myHashTable, unsigned int key);

/**
 * deleteItem
 *
 * Delete the item in the hash table based on the key.
 *
 * @param myHashTable The pointer to the hash table.
 * @param key The key that corresponds to the item.
 *
 */
void deleteItem(HashTable* myHashTable, unsigned int key);

#endif