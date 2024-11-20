#include "roomManip.h"
#include "stringManip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// some macros in case more fields are added later
#define ROOM_CODE "Room Code: "
#define ROOM_NAME "Room Name: "
#define ROOM_DESC "Room Description: "

/*
 * takes a ROOM pointer and creates a shallow copy
**/
ROOM *roomCreate(ROOM *room) {
	ROOM *newRoom = malloc(sizeof(ROOM));
	if (room) {
		newRoom->roomCode = room->roomCode;
		newRoom->name = room->name;
		newRoom->description = room->description;
		newRoom->north = room->north;
		newRoom->south = room->south;
		newRoom->east = room->east;
		newRoom->west = room->west;
	} else {
		newRoom->roomCode = NULL;
		newRoom->name = NULL;
		newRoom->description = NULL;
		newRoom->north = NULL;
		newRoom->south = NULL;
		newRoom->east = NULL;
		newRoom->west = NULL;
	}
	return newRoom;
}

LIST readRoomFile(const char *filepath) {
	// forward declare a bunch of variables
	FILE *roomFile = NULL;
	LIST roomList = listCreate(); // allocate enough space for the current maximum number of rooms
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
	ROOM *currentRoom = NULL;
	char **currentField = NULL;
	
	char lineBuffer[1024]; // allocate some space for reading lines from roomFile
	char *lineData = NULL; // this will contain only the "important" parts of the line (so not the prefix)
	char *prefix = NULL; // when reading lines, this is the c string determining which field the text falls into

	// try to read the passed filepath
	roomFile = fopen(filepath, "r");
	if (roomFile == NULL) {
		printf("file \"%s\" could not be found\n", filepath);
	}

	// iterate through each line of the file
	while (fgets(lineBuffer, sizeof(lineBuffer), roomFile)) {
		if (strncmp(lineBuffer, ROOM_NAME, strlen(ROOM_NAME)) == 0)
			listAppendItem(&roomList, roomCreate(NULL));

		currentRoom = (ROOM *)(listGetElement(&roomList, roomList.size - 1));

		// set the prefix and c string pointer depending on the starting text
		if (strncmp(lineBuffer, ROOM_NAME, strlen(ROOM_NAME)) == 0) {
			prefix = ROOM_NAME;
			currentField = &currentRoom->name;
		} else if (strncmp(lineBuffer, ROOM_CODE, strlen(ROOM_CODE)) == 0) {
			prefix = ROOM_CODE;
			currentField = &currentRoom->roomCode;
		} else if (strncmp(lineBuffer, ROOM_DESC, strlen(ROOM_DESC)) == 0) {
			prefix = ROOM_DESC;
			currentField = &currentRoom->description;
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

		free(lineData);
		lineData = NULL;
	}
	roomFile = NULL;

	return roomList;
}