#include "roomManip.h"
#include <stdio.h>
#include <stdlib.h>

// ROOM *createDungeon(ROOM *roomArray, int roomArraySize, int dungeonSize) {
// 	ROOM *dungeon = NULL;
// 	for (int i = 0; i < dungeonSize; i++) {

// 	}
// }

int main(int argc, char *argv[]) {
	int roomCount = 0;
	ROOM *roomArray;

	// yell is a file isn't passed
	if (argc < 2) {
		printf("please pass a file name\n");
		return 1;
	}

	roomArray = readRoomFile(argv[1], &roomCount);

	// as proof all the rooms are saved, yell about it
	printf("Saved %i rooms:\n", roomCount);
	for (int i = 0; i < roomCount; i++) {
		printf("Room %s: %s\n\n", (roomArray+i)->roomCode, (roomArray+i)->name);
	}

	if (roomArray == NULL) return 1;

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