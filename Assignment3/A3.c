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
void insertSorted(struct LinkedList** head, const char* name);
LinkedList* searchLinkedList(char* searchName, LinkedList* linkedList, int* comparisonCount);

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
// DESCRIPTION  : Allocates and initializes a new LinkedList node with a duplicated name string
// PARAMETERS   : 
// 	    const char* name  : Pointer to the null terminated character string that will be duplicated
// RETURNS      : returns pointer to the newly allocated LinkedList node OR exits the program if memory allocation fails
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

// METHOD       : insertSorted()
// DESCRIPTION  : Creates and inserts a new node into the sorted linked list containing the given name, maintaining the sorted order of the list
// PARAMETERS   : 
//      LinkedList **head	: Pointer to the head node of the linked list
//		const char* name	: Pointer to the null terminated character string to store in the new node
// RETURNS      : returns 0 or void because nothing
void insertSorted(LinkedList** head, const char* name)
{
    LinkedList* newNode = createNode(name);
    
	// safe check, if allocation fails or is out of memory, display error and exit
	if (newNode == NULL)
	{
		printf("No Memory!");
		exit(EXIT_FAILURE);
	}

	// Compares new name against the current head node
	// Checks if head is NULL, meaning the list is empty OR
	// Checks if the new name is less than the head name, insert at the beginning
	if (*head == NULL || strcmp(newNode->name, (*head)->name) < 0)
	{
		newNode->next = *head;
		*head = newNode;
		return;
	}

	// Set current pointer to head
	LinkedList* current = *head;
	// Moves current pointer to next as long as the next node is not NULL and is less than the new name
	// Basically will traverse the list to find the correct position for the new node
	while (current->next != NULL && strcmp(current->next->name, name) < 0)
	{
		current = current->next;
	}
	// Insert the new node in the correct position
	newNode->next = current->next;
	current->next = newNode;
}

// METHOD       : searchLinkedList()
// DESCRIPTION  : Searches the sorted linked list for a node containing the given name, 
//				: Returns a pointer to the node if found or NULL. 
//				: Also counts the number of comparisons that were made during the search.
// PARAMETERS   : 
//		char* searchName		: name to search for in the linked list
//      LinkedList **head		: Pointer to the head node of the linked list
//		int *comparisonCount	: Pointer storing the number of comparisons made during the search
// RETURNS      : returns LinkedList* - pointer to the node if found or NULL.
LinkedList* searchLinkedList(char* searchName, LinkedList* linkedList, int* comparisonCount)
{
	*comparisonCount = 0; // Initialize comparison count to 0
	LinkedList* current = linkedList; // Start from the head of the linked list

	// loop through the linked list until the end or if a name is found
	while (current != NULL)
	{
		(*comparisonCount)++; // Increment comparison count

		// if result is 0, then the names are equal/matching
		if (strcmp(current->name, searchName) == 0)
		{
			return current; // Found the name, return the node
		}
		// if result is greater than 0, then the current name is greater than the search name so it will come after it
		if (strcmp(current->name, searchName) > 0)
		{
			return NULL; // Stop searching since name not found and current name is greater
		}

		current = current->next; // Move to the next node
	}
	// reached the end of the linked list without finding the name
	return NULL;
}

int main(void)
{
	return 0;
}