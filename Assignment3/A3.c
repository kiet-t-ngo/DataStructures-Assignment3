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
void loadNames(const char* fileName, LinkedList** linkedList, LinkedList* hashTable[]);
LinkedList* createNode(const char* name);
void insertSorted(struct LinkedList** head, const char* name);
LinkedList* searchLinkedList(char* searchName, LinkedList* linkedList, int* comparisonCount);
void searchForNameTwice(char* searchName, LinkedList* hashTable[], int comparisonCount[2], LinkedList* linkedList);
void PrintList(LinkedList* head);
void FreeList(LinkedList* head);
void FreeHashTable(LinkedList* hashTable[]);


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

// METHOD       : loadNames()
// DESCRIPTION  : Loads names from a text file into a linked list and a hash table
//				: Remove the new line characters
//				: Maintain sorted order in both linked list and hash table.
// PARAMETERS   : 
//      const char* fileName  : Pointer to the textfile filename/filepath to read from that contains 2000+ names, one name per line
//      LinkedList** linkedList  : Pointer to the head of the linked list
//      LinkedList* hashTable[]  : Array of pointers to the heads of the hash table buckets
// RETURNS      : returns 0 or void because nothing
void loadNames(const char* fileName, LinkedList** linkedList, LinkedList* hashTable[])
{
	char nameBuffer[MAX_NAME_LENGTH]; // Buffer to hold names read from the textfile

	// Open the file in read mode and check if it was successful
	FILE* fp = fopen(fileName, "r");
	// if NULL, then the file could not be opened, display error and exit
	if (fp == NULL)
	{
		printf("Error: Could not open file named: %s.\n", fileName);
		exit(EXIT_FAILURE);
	}

	// Read names from the textfile, line by line until the end of file is reached
	while (fgets(nameBuffer, sizeof(nameBuffer), fp))
	{
		// Remove new line character
		nameBuffer[strcspn(nameBuffer, "\r\n")] = '\0';
		// Calls insertSorted() to insert the name into the linked list
		insertSorted(linkedList, nameBuffer);
		// Calls GenerateHash() to get the hash value for the name
		int hash = GenerateHash(nameBuffer);
		// Calls insertSorted() to insert the name into the hash table
		insertSorted(&hashTable[hash], nameBuffer);
	}

	// Close the file after reading all the names
	fclose(fp);
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
//		char* searchName		: name to search for in the linked list (entered by user)
//      LinkedList* linkedList	: Pointer to the head node of the linked list to search
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

// METHOD       : searchForNameTwice()
// DESCRIPTION  : Searches for the given name twice in both the linked list and the hash table
//				: Returns a pointer to the node if found or NULL. 
//				: Also counts the number of comparisons that were made during the search.
// PARAMETERS   : 
//		char* searchName		: the name to search (entered by user)
//      LinkedList* hashTable[]	: Pointer to the hash table array of linked lists to search in
//		int *comparisonCount[2]	: Array containing 2 elements: [0] for linked list comparisons, [1] for hash table comparisons
//		LinkedList* linkedList	: Pointer to the head node of the linked list to search
// RETURNS      : returns 0 or void because nothing
void searchForNameTwice(char* searchName, LinkedList* hashTable[], int comparisonCount[2], LinkedList* linkedList)
{
	// Declare a pointer to hold the found node
	LinkedList* foundNode;

	// Search the linked list first
	// Calls searchLinkedList() and store the result in foundNode
	foundNode = searchLinkedList(searchName, linkedList, &comparisonCount[0]);

	printf("Searching in the linked list for %s\n", searchName);

	// If the node was found, print the message saying it was found in the linked list with the number of comparisons made
	if (foundNode != NULL)
	{
		printf("\t%s was found in the linked list in %d comparisons\n", searchName, comparisonCount[0]);
	}
	// If the node was not found, print the message saying it was not found in the linked list with the number of comparisons made
	else
	{
		printf("\t%s was NOT found in the linked list in %d comparisons\n", searchName, comparisonCount[0]);
	}

	// Now search the hash table
	printf("Searching in the hash table for %s\n", searchName);

	// Calls GenerateHash() to get the hash value for the searchName and stores it
	int hash = GenerateHash(searchName);
	// Calls searchLinkedList() and store the result in foundNode
	foundNode = searchLinkedList(searchName, hashTable[hash], &comparisonCount[1]);

	// If the node was found, print the message saying it was found in the hash table with the number of comparisons made
	if (foundNode != NULL)
	{
		printf("\t%s was found in the hash table in %d comparisons\n", searchName, comparisonCount[1]);
	}
	// If the node was not found, print the message saying it was not found in the hash table with the number of comparisons made
	else
	{
		printf("\t%s was NOT found in the hash table in %d comparisons\n", searchName, comparisonCount[1]);
	}
}

// METHOD       : PrintList()
// DESCRIPTION  : Goes through the list starting from the head node and prints the formatted list of names
// RETURNS      : returns 0 or nothing because void
void PrintList(LinkedList* head)
{
	LinkedList* current = head;
	while (current != NULL)
	{
		printf("%s\n", current->name);
		current = current->next;
	}
}

// METHOD       : FreeList()
// DESCRIPTION  : Free all dynamically allocated memory in the list
// RETURNS      : returns 0 or nothing because void
void FreeList(LinkedList* head) 
{
	LinkedList* current = head;
	LinkedList* tempNode;

	// Loop through the list until the end of the list, freeing each node and its name string
	while (current != NULL) 
	{
		tempNode = current;
		current = current->next;
		// Free the dynamically allocated name string and the node itself
		free(tempNode->name);
		free(tempNode);
	}
}

// METHOD       : FreeHashTable()
// DESCRIPTION  : Free all dynamically allocated memory in the hash table
// RETURNS      : returns 0 or nothing because void
void FreeHashTable(LinkedList* hashTable[])
{
	// Loop through each bucket in the hash table and free the linked list in that bucket
	for (int i = 0; i < HASH_SIZE; i++)
	{
		// If the bucket is not empty, free the linked list in that bucket
		if (hashTable[i] != NULL)
		{
			FreeList(hashTable[i]);
		}
	}
}

int main(void)
{
	LinkedList* linkedList = NULL; // Initialize the head of the linked list to NULL
	LinkedList* hashTable[HASH_SIZE] = {NULL}; // Initialize all buckets in the hash table to NULL
	// Basically sets first element to NULL and the rest will be automatically set to NULL as well
	// Won't need to loop through the hash table to set each bucket to NULL using a for loop
	int listComparisonCount = 0; // Set a comparison count for the linked list to 0
	int hashComparisonCount = 0; // Set a comparison count for the hash table to 0
	int totalSearches = 0; // Set a total search count to 0 which would be the sum of the linked list and hash table comparisons
	char searchNameBuffer[MAX_NAME_LENGTH]; // Buffer to hold the name to search for with a max length of 35 characters

	// Load names from the textfile into the linked list and hash table
	loadNames("names.txt", &linkedList, hashTable);
	printf("Names from textfile were successfully loaded into the linked list and hash table.\n");

	while (1) // Infinite loop until user chooses to exit
	{
		printf("Enter a name to search (or 'exit' to quit): ");

		// Read the user input for the name to search
		// If fgets() returns NULL, that means there is an error or reaches the end of file, so we break out of the loop
		if (fgets(searchNameBuffer, sizeof(searchNameBuffer), stdin) == NULL)
		{
			break;
		}

		// Removes new line and return characters from the input
		searchNameBuffer[strcspn(searchNameBuffer, "\r\n")] = '\0';
		
		// Convert each character in the searchNameBuffer to lowercase so we can compare it 
		// with the names in the linked list and hash table, which are also stored in lowercase
		for (int i = 0; searchNameBuffer[i] != '\0'; i++) {
			searchNameBuffer[i] = (char)tolower((unsigned char)searchNameBuffer[i]);
		}

		// Check if the user wants to exit the program, if they do, break out of the loop
		if (strcmp(searchNameBuffer, "exit") == 0)
		{
			break;
		}

		// Create a array to hold the comparison counts for linked list and hash table and initialize both to 0
		int comparisonCount[2] = {0, 0};
		// Call the searchForNameTwice() function to search for the name in both the linked list and hash table
		searchForNameTwice(searchNameBuffer, hashTable, comparisonCount, linkedList);

		// Add the linked list comparison count to the running list total
		listComparisonCount += comparisonCount[0];
		// Add the hash table comparisons count	to the running hash total
		hashComparisonCount += comparisonCount[1];
		totalSearches++; // Increment the total search count
	}

	// Display 3 blank lines after finishing the loop, as per assignment requirements
	printf("\n\n\n");
	printf("Total searches: %d\n", totalSearches);
	printf("Total number of comparisons for the linked list: %d\n", listComparisonCount);
	printf("Total number of comparisons for the hash table: %d\n", hashComparisonCount);

	// Free the dynamically allocated memory for the linked list and hash table
	FreeList(linkedList);
	FreeHashTable(hashTable);

	return 0;
}