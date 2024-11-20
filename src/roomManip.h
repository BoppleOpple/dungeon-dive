#include "list.h"

#ifndef ROOM_STRUCT
#define ROOM_STRUCT
typedef struct ROOM ROOM;

// let Room be a structure of dynamically allocated strings
// (this is the cause of most of my comments :P)
struct ROOM {
	char *roomCode;
	char *name;
	char *description;
	ROOM *north;
	ROOM *south;
	ROOM *east;
	ROOM *west;
};
#endif

#ifndef ROOM_MANIP_CREATE
#define ROOM_MANIP_CREATE
/*
 * takes a ROOM pointer and creates a shallow copy
**/
ROOM *roomCreate(ROOM *room);
#endif

#ifndef ROOM_MANIP_READ_FILE
#define ROOM_MANIP_READ_FILE
/*
 * creates an array of rooms from `filepath`, and returns it. also stores the array size in the passed pointer.
**/
LIST readRoomFile(const char *filepath);
#endif