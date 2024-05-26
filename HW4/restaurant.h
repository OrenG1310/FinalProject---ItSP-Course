#ifndef hw4
#define hw4

/* restaurant.h - Header file. */


#define _CRT_SECURE_NO_WARNINGS
#define MAXNAMELEN 50


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Declaration of struct type Product:
typedef struct Product {
	char* name;
	int price;
	int quantity;
	struct Product *next;
} Product, *PProduct;


// Declaration of struct type List: 
typedef struct List {
	Product* head;
	Product* tail;
	int count;
} List, *PList;


// Declaration of struct type Table:
typedef struct Table {
	int num, bill;
	PList order_list;
} Table, * PTable;


/* Declaration of functions: */


// Creates a product from the given text file and adds it into a list.
// Option 1.
void CreateProducts(PList products);


// Adds or subtracts quantity to an existing product in the linked list.
// Option 2.
void AddItems(PList products, char* name, int quantity);


// Adds an ordered product to a table's list of ordered items and updates the table's bill accordingly.
// Option 3.
void OrderItem(PList products, PTable* tables, int i, char* name, int quantity, int n);


// Returns an ordered product from the table to the kitchen. The product is wasted (doesn't return to the
// kitchen's stock).
// Option 4.
void RemoveItem(PTable* tables, int i, char* name, int quantity);


// Prints a table's ordered products and their amount, prints the bill and the tip. Afterwards, clears the table's
// order list and bill, making it ready for re-sitting.
// Option 5. 
void RemoveTable(PTable* tables, int i);


// Initializes the tables pointer array.
void CreateTables(PTable *tables, int quantity);


// Initializes the table's variables.
int InitiateTable(PTable table, int i);


// Checks if a product we're trying to add to the list already exits in it. If it does, returns 1. 0 otherwise.
int AlreadyExists(PList products, char* name);


// Checks if an ordered item exists in the kitchen, if the quantity is a positive number and if there's enough items
// of the requested product in the kitchen. Returns 1 if one of the above is false, 0 otherwise.
int CheckAvailability(PProduct prod, int quantity, int exists, char* name);


// Copies the data of an ordered product into a new node and adds it to the table's order list.
int CopyAndAddProduct(PProduct prod, PTable* tables, int i, int quantity);


// Inserts a given product into the given linked list as its last element (tail).
void AddAsLast(PProduct toAdd, PList products);


// Checks if the name already exists in the product list and if the price / quantity are positive numbers.
// Returns 1 in input is valid, 0 otherwise.
int CheckInput(PList L, char* name, PProduct product);


// Deletes the given array of tables and frees all allocated memory.
void DeleteTables(PTable* tables, int quantity);


// Frees all allocated memory.
void FreeMemory(PProduct products, PTable* tables, int quantity);


// Deletes the given list and frees all allocated memory.
void DeleteList(PList L);


// Frees allocated memory.
void DeleteNode(PProduct toRemove);


// In case of a runtime error (memory allocation or file opening), this function prints a message
// accordingly and exits the program.
void Error_Msg(char* msg);

#endif
