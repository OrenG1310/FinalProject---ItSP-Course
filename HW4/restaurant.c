/* restaurant.c - Implementation file. */


#include "hw4.h"


// Creates a product from the given text file and adds it into a list.
// Option 1.
void CreateProducts(PList products)
{
	char name[MAXNAMELEN + 1];
	FILE* f = fopen("Manot.txt", "rt"); // Opening the file "Manot.txt".
	if (f == NULL)
		Error_Msg("Input file is wrong!\n");
	while (!feof(f)) {
		PProduct temp = (PProduct)malloc(sizeof(Product));
		if (temp == NULL) {
			DeleteList(products);
			printf("Memory allocation problem! Closing the program...\n");
		}
		if ((fscanf(f, "%s%d%d", &name, &temp->quantity, &temp->price)) < 3) {
			DeleteList(products);
			Error_Msg("Problem reading data form the file! Closing the program...\n");
		}
		if (!CheckInput(products, name, temp)) {
			continue;
		}
		temp->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
		if (temp->name == NULL) {
			DeleteList(products);
			Error_Msg("Memory allocation problem! Closing the program...\n");
		}
		strcpy(temp->name, name);
		AddAsLast(temp, products);
	}
	fclose(f);
	printf("The kitchen was created!\n");
}


// Adds or subtracts quantity to an existing product in the linked list.
// Option 2.
void AddItems(PList products, char* name, int quantity)
{
	PProduct temp = products->head;
	while (temp) {
		if (!strcmp(temp->name, name)) { // strcmp returns 0 if strings are equal.
			temp->quantity += quantity;
			printf("%d %s added to the kitchen!\n", quantity, name);
			return;
		}
		temp = temp->next;
	}
	printf("There are no %ss in the kitchen!\n", name); // If the product doesn't exist in the linked list.
}


// Adds an ordered product to a table's list of ordered items and updates the table's bill accordingly.
// Option 3.
void OrderItem(PList products, PTable* tables, int i, char* name, int quantity, int n)
{
	int exists = 0;
	PProduct temp = products->head;
	while (temp) {
		if (!(strcmp(temp->name, name))) {
			exists = 1;
			break;
		}
		temp = temp->next;
	}
	if (CheckAvailability(temp, quantity, exists, name))
		return;
	if (!CopyAndAddProduct(temp, tables, i, quantity)) {
		FreeMemory(products, tables, n);
		Error_Msg("Memory allocation problem! Closing the program...\n");
	}
	temp->quantity -= quantity; // Removing ordered products form the kitchen.
	tables[i]->bill += ((temp->price) * quantity); // Updating table bill.
}


// Returns an ordered product from the table to the kitchen. The product is wasted (doesn't return to the
// kitchen's stock).
// Option 4.
void RemoveItem(PTable* tables, int i, char* name, int quantity)
{
	PProduct temp = tables[i]->order_list->head;
	while (temp) {
		if (!strcmp(temp->name, name)) {
			if (temp->quantity < quantity) {
				printf("Cannot return more %s than the table ordered!\n", name);
				return;
			}
			temp->quantity -= quantity; // Removing products from the table.
			tables[i]->bill -= ((temp->price) * quantity); // Updating table bill.
			printf("%d %s returned to the kitchen from table number %d.\n", quantity, name, i);
			return;
		}
		temp = temp->next;
	}
	printf("Table number %d didn't order any %s!\n", i, name);
}


// Prints a table's ordered products and their amount, prints the bill and the tip. Afterwards, clears the table's
// order list and bill, making it ready for re-sitting.
// Option 5.
void RemoveTable(PTable* tables, int i)
{
	PProduct temp = tables[i]->order_list->head;
	PProduct toRemove;
	if (temp == NULL) {
		printf("Table number %d is not ordered yet!\n", i);
		return;
	}
	while (temp) {
		printf("%d %s. ", temp->quantity, temp->name);
		toRemove = temp;
		temp = temp->next;
		DeleteNode(toRemove); // Removing node from order_list.
		tables[i]->order_list->count -= 1;
	}
	printf("%d nis + %d tip, please!\n", tables[i]->bill, (int)((tables[i]->bill) * 0.1));
	// Resetting the table to be a new table:
	tables[i]->bill = 0;
	tables[i]->order_list->head = tables[i]->order_list->tail = NULL;
}


// Creates an array of pointers to tables. Allocates memory according to the given quantity of tables.
void CreateTables(PTable* arr, int quantity)
{
	for (int i = 0; i < quantity; i++) {
		PTable table = (PTable)malloc(sizeof(Table)); // Allocating memory for each table.
		if (table == NULL) {
			for (int j = 0; j < i; j++)
				free(&arr[j]);
			Error_Msg("Memory allocation problem! Closing the program...\n");
		}
		if (!InitiateTable(table, i)) {
			for (int j = 0; j < i; j++) {
				free(arr[j]->order_list);
				free(arr[j]);
			}
			Error_Msg("Memory allocation problem! Closing the program...\n");
		}
		arr[i] = table; // Inserting the table into the pointer array.
	}
}


// Initializes the table's variables.
int InitiateTable(PTable table, int i)
{
	table->num = (i + 1);
	table->bill = 0;
	// Allocating memory for the ordered items list:
	if ((table->order_list = (PList)malloc(sizeof(List))) == NULL)
		return 0;
	table->order_list->head = table->order_list->tail = NULL;
	table->order_list->count = 0;
	return 1;
}


// Checks if a product we're trying to add to the list already exits in it. If it does, returns 1. 0 otherwise.
int AlreadyExists(PList products, char* name)
{
	PProduct temp = products->head;
	while (temp) {
		if (!strcmp(temp->name, name)) { // strcmp returns 0 if strings are equal.
			printf("%s already exists in the kitchen!\n", name);
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}


// Checks if an ordered item exists in the kitchen, if the quantity is a positive number and if there's enough items
// of the requested product in the kitchen. Returns 1 if one of the above is false, 0 otherwise.
int CheckAvailability(PProduct prod, int quantity, int exists, char* name)
{
	if (!exists) {
		printf("We don't have %s, sorry!\n", name);
		return 1;
	}
	else if (quantity <= 0) {
		printf("Can't order less than 1 %s, sorry!\n", name);
		return 1;
	}
	else if (quantity > prod->quantity) {
		printf("We don't have enough %s in the kitchen, sorry!\n", name);
		return 1;
	}
	return 0;
}


// Copies the data of an ordered product into a new node and adds it to the table's order list.
int CopyAndAddProduct(PProduct prod, PTable* tables, int i, int quantity)
{
	PProduct temp = (PProduct)malloc(sizeof(Product));
	if (temp == NULL)
		return 0;
	if ((temp->name = (char*)malloc((strlen(prod->name) + 1) * sizeof(char))) == NULL) {
		free(temp);
		return 0;
	}
	strcpy(temp->name, prod->name);
	temp->price = prod->price;
	temp->quantity = quantity;
	AddAsLast(temp, tables[i]->order_list);
	return 1;
}


// Inserts a given product into the given linked list as its last element (tail).
void AddAsLast(PProduct toAdd, PList L)
{
	if (L->head == NULL) { // If list was empty, assigning head:
		L->head = toAdd;
	}
	else {
		L->tail->next = toAdd; // Linking the new product to the current last element of the list.
	}
	L->tail = toAdd;
	L->tail->next = NULL; // Setting the added element as end of the list (tail).
	L->count++;
}


// Checks if the name already exists in the product list and if the price / quantity are positive numbers.
// Returns 1 in input is valid, 0 otherwise.
int CheckInput(PList L, char* name, PProduct product)
{
	// Checking if the list isn't empty and the product already exists in it:
	if ((L->head != NULL) && AlreadyExists(L, name))
		return 0;
	if (product->price <= 0) {
		printf("Product price cannot be a non-positive number!\n");
		return 0;
	}
	if (product->quantity <= 0) {
		printf("Product quantity cannot be a non-positive number!\n");
		return 0;
	}
	return 1;
}


// Deletes the given array of tables and frees all allocated memory.
void DeleteTables(PTable* tables, int quantity)
{
	for (int i = 0; i < quantity; i++) {
		DeleteList(tables[i]->order_list);
		free(tables[i]);
	}
	free(tables);
}


// Frees all allocated memory.
void FreeMemory(PProduct products, PTable* tables, int quantity)
{
	DeleteTables(tables, quantity);
	DeleteList(products);
}


// Deletes the given list and frees all allocated memory.
void DeleteList(PList L)
{
	PProduct temp;
	while (L->head != NULL) {
		temp = L->head;
		L->head = L->head->next;
		DeleteNode(temp);
	}
}


// Frees allocated memory.
void DeleteNode(PProduct toRemove)
{
	free(toRemove->name);
	free(toRemove);
}


// In case of a runtime error (memory allocation or file opening), this function prints a message
// accordingly and exits the program.
void Error_Msg(char* msg)
{
	printf("\n%s", msg);
	exit(1);
}
