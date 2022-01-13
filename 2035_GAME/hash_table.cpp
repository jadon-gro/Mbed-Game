/*
 Student Name: Jadon Grossberg
 Date: November 2021

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"
#include "globals.h"
#include "graphics.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {
  // Allocates space in the heap for the new entry
  HashTableEntry* ht = (HashTableEntry*) malloc(sizeof(HashTableEntry));

  // Fills new entry's key, value, and NULL for next
  ht->key = key;
  ht->value = value;
  ht->next = NULL;

  // Returns pointer to this new Hash Table Entry
  return ht;
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {

  // Hash of the key is stored in ui hash
  unsigned int hash = hashTable->hash(key);

  // Creates a pointer "answer" to the head of the bucket of the list that the entry we are looking for should be in
  HashTableEntry* answer = hashTable->buckets[hash % hashTable->num_buckets];

  // Iterates through the list and stops when we've reached a match
  while (answer) {
    if (answer->key == key) {
        // Returns the hash table entry when it's reached the entry with a matching key
        return answer;
    }
    // Iterates to the next item in the list
    answer = answer->next;
  }
  //returns NULL if the key doesn't exist.
  return answer;

}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets contain indeterminant values, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable* hashTable) {

  // Index initialization for the for loop
  unsigned int i;
  // Creates a current and trailing pointer to iterate through the list.
  HashTableEntry *trailing, *curr;
  // Outer loop allows for one iteration for each bucket
  for (i = 0; i < hashTable->num_buckets; i++) {
    
    // Set curr to be the head of the list
    curr = hashTable->buckets[i];

    // Iterates through the list, freeing the values and entries as it goes.
    while(curr) {
        trailing = curr;
        curr = curr->next;
        if(trailing->value) {
          free(trailing->value);
        }
        free(trailing);
    }
  }

  // Frees the buckets and hashtable at the end.
  free(hashTable->buckets);
  free(hashTable);
}

void* insertItem(HashTable* hashTable, unsigned int key, void* value) {
//  uLCD.printf("start");
//  if (hashTable ==  NULL)
//    uLCD.printf("null");
//  if (hashTable->hash == NULL)
//    uLCD.printf("asdkjfhlaksdfa");
  
  // Hashes the key and makes curr point to the head of the list the new entry should be in.
  unsigned int hash = hashTable->hash(key);
//    uLCD.printf("boop");
  HashTableEntry* curr = hashTable->buckets[hash % hashTable->num_buckets];
  // Iterates through the linked list
  while(curr) {
    // If there is a key match then replace the value with the new one and return the replaced value
    if (curr->key == key) {
      void* old = curr->value;
      curr->value = value;
      return old;
    }
    curr = curr->next;
  }
  // If the key wasn't already in the list, then make a new entry and insert it at the beginning
  HashTableEntry* ht = createHashTableEntry(key, value);
  ht->next = hashTable->buckets[hash % hashTable->num_buckets];
  hashTable->buckets[hash % hashTable->num_buckets] = ht;
  return NULL;
}

void* getItem(HashTable* hashTable, unsigned int key) {
  
  // Calls helper function findItem to get the hashTableEntry pointer of the desired item
  HashTableEntry* item = findItem(hashTable, key);

  // If it doesn't exist, return NULL
  if (!item) {
    return NULL;
  }

  // Otherwise you can just return the value of the found item
  return item->value;
}

void* removeItem(HashTable* hashTable, unsigned int key) {
  
  // Hashes the key and finds head of the corrosponding LL
  unsigned int hash = hashTable->hash(key);
  HashTableEntry* curr = hashTable->buckets[hash % hashTable->num_buckets];

  // Previous pointer helps for splicing
  HashTableEntry* previous;

  // i keeps track of where in the LL you are
  unsigned int i = 0;

  // Iterates through the bucket
  while (curr) {

    //If found, begin removal process
    if (curr->key == key) {
      // If you're at the beginning of the LL, you need to set the head to the next when splicing, otherwise, just splice with previous
      if (i == 0) {
        hashTable->buckets[hash % hashTable->num_buckets] = curr->next;
      } else {
        previous->next = curr->next;
      }

      // free the item and return the value
      void* old = curr->value;
      free(curr);
      return old;
      
    }

    // Perform the iteration
    previous = curr;
    curr = curr->next;
    i++;
  }

  // If not found, return null
  return NULL;
}

void deleteItem(HashTable* hashTable, unsigned int key) {
  // free the value returned by remove item
  free(removeItem(hashTable, key));
}
