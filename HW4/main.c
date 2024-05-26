/* main.c */
/* Description: This is a program for managing a restaurant. The program creates a linked list of products in the kitchen from
a given text file and creates an array of tables. Each table has its own linked list of ordered products.
The program can add products to the kitchen's stock (Option 2), order products for a table (Option 3), remove
products from a table (Option 4), and print the ordered products and the final bill with the tip included (Option 5). */


#include "hw4.h"


int main()
{
	List products; // List of products in the kitchen.
	PTable* tables; // Array of Table pointers.
	int choice = 1, i, table_quantity, prod_quantity;
	char name[MAXNAMELEN + 1];

	// Initializing products list:
	products.head = products.tail = NULL;
	products.count = 0;

	FILE* f = fopen("Instructions.txt", "rt"); // Opening the file "Instructions.txt".
	if (f == NULL)
		Error_Msg("Input file is wrong!\n");

	if ((fscanf(f, "%d", &table_quantity)) != 1) // Reading number of tables in the dining room (first line in the text file).
		Error_Msg("Problem reading from the file! Closing the program...\n");
	if ((tables = (PTable*)malloc((table_quantity) * sizeof(PTable))) == NULL) // Allocating memory to the array.
		Error_Msg("Memory allocation problem! Closing the program...\n");
	CreateTables(tables, table_quantity); // Creating tables pointer array.

	while (!feof(f)) {
		if ((fscanf(f, "%d", &choice)) != 1) {
			FreeMemory(&products, tables, table_quantity);
			Error_Msg("Problem reading from the file! Closing the program...\n");
		}
		switch (choice) {
		case 1:
			CreateProducts(&products);
			break;
		case 2:
			if ((fscanf(f, "%s%d", &name, &prod_quantity)) != 2) {
				FreeMemory(&products, tables, table_quantity);
				Error_Msg("Problem reading from the file! Closing the program...\n");
			}
			AddItems(&products, name, prod_quantity);
			break;
		case 3:
			if ((fscanf(f, "%d%s%d", &i, &name, &prod_quantity)) != 3) {
				FreeMemory(&products, tables, table_quantity);
				Error_Msg("Problem reading from the file! Closing the program...\n");
			}
			if (i > table_quantity || i < 1) {
				printf("There is no such table in the dining hall!\n");
				break;
			}
			OrderItem(&products, tables, i, name, prod_quantity, table_quantity);
			break;
		case 4:
			if ((fscanf(f, "%d%s%d", &i, &name, &prod_quantity)) != 3) {
				FreeMemory(&products, tables, table_quantity);
				Error_Msg("Problem reading from the file! Closing the program...\n");
			}
			if (prod_quantity <= 0) {
				printf("Cannot return 0 or negative number of products!\n");
				break;
			}
			RemoveItem(tables, i, name, prod_quantity);
			break;
		case 5:
			if ((fscanf(f, "%d", &i)) != 1) {
				FreeMemory(&products, tables, table_quantity);
				Error_Msg("Problem reading from the file! Closing the program...\n");
			}
			RemoveTable(tables, i);
			break;
		}
	}
	
	fclose(f);
	FreeMemory(&products, tables, table_quantity);
	return 0;
}