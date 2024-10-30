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
 * takes a ROOM pointer and creates a copy
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

ROOM *readRoomFile(const char *filepath, int *arraySize) {
	// forward declare a bunch of variables
	FILE *roomFile;
	int maxRooms = ROOM_ARRAY_SIZE; // maximum size of the array
	*arraySize = 0; // current size of the array
	int incrementRoom = 0; // bool for whether or not to increment the room in the loop (I know bools exist, but I dont mind ints)
	ROOM *roomArray = malloc(sizeof(ROOM)*maxRooms); // allocate enough space for the current maximum number of rooms
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
		if (strncmp(lineBuffer, ROOM_CODE, strlen(ROOM_CODE)) == 0) {
			prefix = ROOM_CODE;
			currentField = &(roomArray + *arraySize)->roomCode;
		} else if (strncmp(lineBuffer, ROOM_NAME, strlen(ROOM_NAME)) == 0) {
			prefix = ROOM_NAME;
			currentField = &(roomArray + *arraySize)->name;
		} else if (strncmp(lineBuffer, ROOM_DESC, strlen(ROOM_DESC)) == 0) {
			prefix = ROOM_DESC;
			currentField = &(roomArray + *arraySize)->description;

			// since description is the last one passed, move to the next room once it's read
			incrementRoom = 1;
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

		if (incrementRoom) {
			incrementRoom = 0;

			// debug output, mostly just muddies up the terminal
			// printf("Room %s: %s\n%s\n\n", (roomArray + *arraySize)->roomCode, (roomArray + *arraySize)->name, (roomArray + *arraySize)->description);

			(*arraySize)++;

			// if the current room is oob, add more rooms
			if (*arraySize >= maxRooms) {
				// debug output, might as well keep it around
				// printf("current size: %li\n", maxRooms*sizeof(Room));
				// printf("new size: %li\n", maxRooms*sizeof(Room)*2);

				// double the size of the array
				maxRooms *= 2;

				// and safely reallocate its space
				ROOM *newRoomArray = realloc(roomArray, maxRooms*sizeof(ROOM));
				if (!newRoomArray) {
					free(roomArray);
					printf("could not allocate memory somehow");
					return NULL;
				}
				roomArray = newRoomArray;
			}
		}
	}

	return roomArray;
}