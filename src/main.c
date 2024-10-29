#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// some macros in case more fields are added later
#define ROOM_CODE "Room Code: "
#define ROOM_NAME "Room Name: "
#define ROOM_DESC "Room Description: "

// let Room be a structure of dynamically allocated strings
// (this is the cause of most of my comments :P)
typedef struct {
	char *roomCode;
	char *name;
	char *description;
}Room;
/*
 * trims whitespace from a c string in-place
**/
void trim(char *str){
	// trimming leftmost whitespace
	char *start = str;
	while (isspace(*start)) start++;

	// trimming righttmost whitespace
	char *end = str + strlen(str) - 1;
	while (isspace(*end)) end--;
	*(end+1) = 0;
	
	// move the string and its new termination character to the original pointer
	strcpy(str, start);
}


int main(int argc, char *argv[]) {
	// forward declare a bunch of variables
	FILE *roomFile;
	int maxRooms = 1; // maximum size of the array
	int roomCount = 0; // current size of the array
	int incrementRoom = 0; // bool for whether or not to increment the room in the loop (I know bools exist, but I dont mind ints)
	Room *roomArray = malloc(sizeof(Room)*maxRooms); // allocate enough space for the current maximum number of rooms

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

	// yell is a file isn't passed
	if (argc < 2) {
		printf("please pass a file name\n");
		return 1;
	}

	// try to read the passed filepath
	roomFile = fopen(argv[1], "r");
	if (roomFile == NULL) {
		printf("file \"%s\" could not be found\n", argv[1]);
	}

	// iterate through each line of the file
	while (fgets(lineBuffer, sizeof(lineBuffer), roomFile)) {
		

		// set the prefix and c string pointer depending on the starting text
		if (strncmp(lineBuffer, ROOM_CODE, strlen(ROOM_CODE)) == 0) {
			prefix = ROOM_CODE;
			currentField = &(roomArray + roomCount)->roomCode;
		} else if (strncmp(lineBuffer, ROOM_NAME, strlen(ROOM_NAME)) == 0) {
			prefix = ROOM_NAME;
			currentField = &(roomArray + roomCount)->name;
		} else if (strncmp(lineBuffer, ROOM_DESC, strlen(ROOM_DESC)) == 0) {
			prefix = ROOM_DESC;
			currentField = &(roomArray + roomCount)->description;

			// since description is the last one passed, move to the next room once it's read
			incrementRoom = 1;
		} else {
			// if it is unknown, there's nothing to do here
			continue;
		}

		// select only the content after the prefix (description/name/code specifier) and trim whitespace
		lineData = lineBuffer + strlen(prefix);
		trim(lineData);

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
			// printf("Room %s: %s\n%s\n\n", (roomArray + roomCount)->roomCode, (roomArray + roomCount)->name, (roomArray + roomCount)->description);

			roomCount++;

			// if the current room is oob, add more rooms
			if (roomCount >= maxRooms) {
				// debug output, might as well keep it around
				// printf("current size: %li\n", maxRooms*sizeof(Room));
				// printf("new size: %li\n", maxRooms*sizeof(Room)*2);

				// double the size of the array
				maxRooms *= 2;

				// and safely reallocate its space
				Room *newRoomArray = realloc(roomArray, maxRooms*sizeof(Room));
				if (!newRoomArray) {
					free(roomArray);
					printf("could not allocate memory somehow");
					return 1;
				}
				roomArray = newRoomArray;
			}
		}
	}

	// as proof all the rooms are saved, yell about it
	printf("Saved %i rooms:\n", roomCount);
	for (int i = 0; i < roomCount; i++) {
		printf("Room %s: %s\n\n", (roomArray+i)->roomCode, (roomArray+i)->name);
	}

	// now be a "good" programmer and dont leak your memory
	for (int i = 0; i < roomCount; i++) {
		// also I know this is a horrible idea 99/100 times but I wanted to try void pointers for this.
		// if I'm using c I might as well have fun with it :P (this is why good was in quotes)
		void **field = (void **)(roomArray+i);

		// since every field is a pointer, I can just iterate through each field bc theyre all the size
		// of a pointer (until the end of the struct)
		while (field < (void **)(roomArray + i + 1))
			free(*field++); // and free each memory pointer in that struct
	}

	// finally free the array
	free(roomArray);

	return 0;
}