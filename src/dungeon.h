#include "roomManip.h"
#include "list.h"

#ifndef DUNGEON_CREATE
#define DUNGEON_CREATE
/*
 * creates a 1d dungeon by extending a linked list using the rooms in roomArray,
 * and returns a pointer to the first room of the dungeon.
**/
ROOM *createDungeon(LIST *roomList, int *width, int *height);
#endif

#ifndef DUNGEON_DFT
#define DUNGEON_DFT
/*
 * iterates through each room of a dungeon and calls `fn` on it
**/
void dungeonDFT(ROOM *dungeon, void (*fn)(ROOM*));
#endif

#ifndef DUNGEON_PRINT
#define DUNGEON_PRINT
/*
 * iterates through and prints the contents of each room in the dungeon
**/
void printDungeon(ROOM *dungeonCorner, int *width, int *height);
#endif

#ifndef DUNGEON_DELETE
#define DUNGEON_DELETE
/*
 * frees each room of a dungeon, which assumes the content of each room will be freed in the roomArray.
 * as a result, this function is really simple since the pointers in each struct are all duplicates
**/
void deleteDungeon(ROOM *dungeon);
#endif