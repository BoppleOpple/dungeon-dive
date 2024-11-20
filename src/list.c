#include "list.h"
#include <stdio.h>
#include <stdlib.h>

LIST listCreate() {
	LIST list = { malloc(sizeof(void*) * DEFAULT_LIST_SIZE), DEFAULT_LIST_SIZE, 0 };
	return list;
}

void listAppendItem(LIST *list, void *item) {
	list->size++;
	
	// if the current item is oob, add more items
	if (list->size >= list->maxSize) {
		// double the size of the array
		list->maxSize *= 2;
		// and safely reallocate its space
		void **newArray = realloc(list->array, list->maxSize*sizeof(void*));
		if (!newArray) {
			free(list->array);
			printf("could not allocate memory somehow");
			exit(1);
			return;
		}
		
		list->array = newArray;
	}

	// add item to array
	*(list->array + list->size - 1) = item;
}

void *listPop(LIST *list, int i) {
	if (i < 0 || i >= list->size) {
		printf("index %i out of range in list with %i elements.\n", i, list->size);
		return NULL;
	}
	void *item = NULL;

	// this could be faster with a circular array or by using a set instead of a list,
	// but i really dont mind this temporarily
	for (int j = i + 1; j < list->size; j++) {
		void *temp = *(list->array + j);
		*(list->array + j) = *(list->array + j - 1);
		*(list->array + j - 1) = temp;
	}

	item = *(list->array + --list->size);
	*(list->array + list->size) = NULL;
	return item;
}

int listIncludesItem(LIST *list, const void *room) {
	for (int i = 0; i < list->size; i++)
		if (*(list->array + i) == room)
			return 1;
	return 0;
}

void *listGetElement(LIST *list, int i) {
	return *(list->array + i);
}

void listClear(LIST *list) {
	free(list->array);
	list->array = NULL;
	list->maxSize = 0;
	list->size = 0;
}