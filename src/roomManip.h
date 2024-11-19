#include "list.h"

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

#ifndef ROOM_MANIP_CREATE
#define ROOM_MANIP_CREATE
ROOM *roomCreate(ROOM *room);
#endif

#ifndef ROOM_MANIP_READ_FILE
#define ROOM_MANIP_READ_FILE
LIST readRoomFile(const char *filepath);
#endif