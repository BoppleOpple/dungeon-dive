#include "roomManip.h"
#include "stringManip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// some macros in case more fields are added later
#define ROOM_CODE "Room Code: "
#define ROOM_NAME "Room Name: "
#define ROOM_DESC "Room Description: "
#define ROOM_ARRAY_SIZE 4

/*
 * takes a ROOM pointer and creates a shallow copy
**/
ROOM *roomCreate(ROOM *room) {
	ROOM *newRoom = malloc(sizeof(ROOM));
	newRoom->roomCode = room->roomCode;
	newRoom->name = room->name;
	newRoom->description = room->description;
	newRoom->north = room->north;
	newRoom->south = room->south;
	newRoom->east = room->east;
	newRoom->west = room->west;
	return newRoom;
}

/*
 * creates and returns an empty array list of ROOMs
**/
ROOM_LIST roomListCreate() {
	ROOM_LIST list = { malloc(sizeof(ROOM) * ROOM_ARRAY_SIZE), ROOM_ARRAY_SIZE, 0 };
	return list;
}

/*
 * takes a ROOM pointer and creates a shallow copy
**/
void roomListAppendRoom(ROOM_LIST *list, ROOM *room) {
	list->size++;
	
	// if the current room is oob, add more rooms
	if (list->size >= list->maxSize) {
		// double the size of the array
		list->maxSize *= 2;
		// and safely reallocate its space
		ROOM *newArray = realloc(list->array, list->maxSize*sizeof(ROOM));
		if (!newArray) {
			free(list->array);
			printf("could not allocate memory somehow");
			exit(1);
			return;
		}
		list->array = newArray;
	}

	// add a new empty room if no template room is passed
	if (room != NULL) {
		*(list->array + list->size - 1) = *roomCreate(room);
	} else {
		ROOM *newRoom = list->array + list->size - 1;
		newRoom->roomCode = "";
		newRoom->name = "";
		newRoom->description = "";
		newRoom->north = NULL;
		newRoom->south = NULL;
		newRoom->east = NULL;
		newRoom->west = NULL;
	}
}

/*
 * sifts a room to the end of the array and decrements the size
 * (effectively deleting it)
**/
void roomListDelete(ROOM_LIST *list, int i) {
	if (i < 0 || i >= list->size) {
		printf("index %i out of range in list with %i elements.\n", i, list->size);
		return;
	}

	// this could be faster with a circular array or by using a set instead of a list,
	// but i really dont mind this temporarily
	for (int j = i + 1; j < list->size; j++) {
		ROOM temp = *(list->array + j);
		*(list->array + j) = *(list->array + j - 1);
		*(list->array + j - 1) = temp;
	}

	list->size--;
}

/*
 * takes a ROOM pointer and checks if it matches a specific room code (unused)
**/
int roomListIncludesCode(ROOM_LIST *list, const char *roomCode) {
	for (int i = 0; i < list->size; i++)
		if (strcmp((list->array + i)->roomCode, roomCode) == 0)
			return 1;
	return 0;
}

/*
 * returns the element at index i, useful for if I want to change how addressing works later
**/
ROOM *roomListGetElement(ROOM_LIST *list, int i) {
	return list->array + i;
}

/*
 * creates an array of rooms from `filepath`, and returns it. also stores the array size in the passed pointer.
**/
ROOM *readRoomFile(const char *filepath, int *arraySize) {
	// forward declare a bunch of variables
	FILE *roomFile;
	ROOM_LIST roomList = roomListCreate(); // allocate enough space for the current maximum number of rooms
	/*
	currentField points to a pointer, so this allocates the space needed for the line data to the
	character pointer in the Room structure.

	+------+-------------+--------+
	|      | roomCode    | char * |
	|      +-------------+--------+
	| Room | name        | char * |
	|      +-------------+--------+
	|      | description | char * | <-- currentField
	+------+-------------+--------+

	so mallocing (its a verb now) *currentField just mallocs whichever field it is pointing to in the
	current Room structure, just as
		roomPtr->description = malloc(...);
	would.
	*/
	char **currentField;
	
	char lineBuffer[1024]; // allocate some space for reading lines from roomFile
	char *lineData = lineBuffer; // this will contain only the "important" parts of the line (so not the prefix)
	char *prefix; // when reading lines, this is the c string determining which field the text falls into

	// try to read the passed filepath
	roomFile = fopen(filepath, "r");
	if (roomFile == NULL) {
		printf("file \"%s\" could not be found\n", filepath);
	}

	// iterate through each line of the file
	while (fgets(lineBuffer, sizeof(lineBuffer), roomFile)) {

		// set the prefix and c string pointer depending on the starting text
		if (strncmp(lineBuffer, ROOM_NAME, strlen(ROOM_NAME)) == 0) {
			roomListAppendRoom(&roomList, NULL);
			prefix = ROOM_NAME;
			currentField = &roomListGetElement(&roomList, roomList.size - 1)->name;
		} else if (strncmp(lineBuffer, ROOM_CODE, strlen(ROOM_CODE)) == 0) {
			prefix = ROOM_CODE;
			currentField = &roomListGetElement(&roomList, roomList.size - 1)->roomCode;
		} else if (strncmp(lineBuffer, ROOM_DESC, strlen(ROOM_DESC)) == 0) {
			prefix = ROOM_DESC;
			currentField = &roomListGetElement(&roomList, roomList.size - 1)->description;
		} else {
			// if it is unknown, there's nothing to do here
			continue;
		}

		// select only the content after the prefix (description/name/code specifier) and trim whitespace
		lineData = str_trim(lineBuffer + strlen(prefix));
		
		// now, do that weird thing.
		*currentField = malloc( sizeof(char) * ( strlen(lineData) + 1 ) );
		strcpy(*currentField, lineData);
		// this allocates exactly as much memory as is needed to hold all the strings, and doesnt set limits
		// on the length of names, codes, or descriptions. whether or not this is good is in the eye of the
		// beholder, but I think it is (it was also my first thought before you statically allocated space
		// in class) :P
	}

	*arraySize = roomList.size;
	return roomList.array;
}