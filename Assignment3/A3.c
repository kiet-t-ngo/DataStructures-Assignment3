/*
 * FILE         : A3
 * PROJECT      : Data Structures - Assignment 3
 * PROGRAMMER   : Kiet Ngo
 * FIRST VERSION: 2026-11-19
 * DESCRIPTION  : 
 */

#pragma warning (disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// According to assignment requirements, each name length is not more than 30 chars long
// But don't use a fixed 30 char length for linked list node, so i am using 35 instead
#define MAX_NAME_LENGTH 35 
#define HASH_SIZE 127 // Max hash size of 127
#define MAX_NAMES 10000 // NOTE: DONT KNOW IF NEED YET - Max 10000 names in file

// Structs
typedef struct LinkedList
{
	char* name;
	struct LinkedList* next;
} LinkedList;

// Function Prototypes
int GenerateHash(char* word);
LinkedList* createNode(const char* name);

// METHOD       : GenerateHash()
// DESCRIPTION  : Generates a non-negative hash index
// PARAMETERS   : 
//      char* word  : Pointer to the null terminated character string that will get hashed
// RETURNS      : returns int - the computed hash value
// Copied from Week10_HashTable
int GenerateHash(char* word) 
{
    int hash = 0;
    for (int i = 0; word[i] != '\0'; i++) 
    {
        int asciiValue = word[i];
        hash = (hash + asciiValue) % HASH_SIZE;
    }
    return hash;
}

// METHOD       : createNode()
// DESCRIPTION  : Allocates and initializes a new LinkedList node
// RETURNS      : returns 0 or nothing because void
LinkedList* createNode(const char* name)
{
    // allocating memory for node
    LinkedList* newNode = (LinkedList*)malloc(sizeof(LinkedList));
    // safe check, if allocation fails or is out of memory, display error and exit
    if (newNode == NULL) 
    {
        printf("No Memory!");
        exit(EXIT_FAILURE);
    }
    newNode->name = strdup(name);
	// safe check, checks if stddup failed to allocate memory for the name string
    if (newNode->name == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
	newNode->next = NULL; // Initialize next pointer to NULL
    return newNode;
}

int main(void)
{
	return 0;
}