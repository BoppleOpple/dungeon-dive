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

typedef struct {
	ROOM **array;
	int maxSize;
	int size;
}ROOM_LIST;

#ifndef ROOM_MANIP_CREATE
#define ROOM_MANIP_CREATE
ROOM *roomCreate(ROOM *room);
#endif

#ifndef ROOM_MANIP_READ_FILE
#define ROOM_MANIP_READ_FILE
ROOM *readRoomFile(const char *filepath, int *arraySize);
#endif

#ifndef ROOM_MANIP_CREATE_LIST
#define ROOM_MANIP_CREATE_LIST
ROOM_LIST roomListCreate();
#endif

#ifndef ROOM_MANIP_LIST_APPEND
#define ROOM_MANIP_LIST_APPEND
void roomListAppendRoom(ROOM_LIST *list, ROOM *room);
#endif

#ifndef ROOM_MANIP_LIST_GET
#define ROOM_MANIP_LIST_GET
ROOM *roomListGetElement(ROOM_LIST *list, int i);
#endif