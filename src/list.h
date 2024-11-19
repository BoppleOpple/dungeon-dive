#ifndef LIST_STRUCT
#define LIST_STRUCT
typedef struct {
	void **array;
	int maxSize;
	int size;
}LIST;
#endif

#define DEFAULT_LIST_SIZE 4

#ifndef LIST_LIST_CREATE
#define LIST_LIST_CREATE
LIST listCreate();
#endif

#ifndef LIST_LIST_APPEND
#define LIST_LIST_APPEND
void listAppendItem(LIST *list, void *item);
#endif

#ifndef LIST_LIST_DELETE
#define LIST_LIST_DELETE
void *listPop(LIST *list, int i);
#endif

#ifndef LIST_LIST_INCLUDES
#define LIST_LIST_INCLUDES
int listIncludesItem(LIST *list, const void *item);
#endif

#ifndef LIST_LIST_GET
#define LIST_LIST_GET
void *listGetElement(LIST *list, int i);
#endif

#ifndef LIST_LIST_FREE
#define LIST_LIST_FREE
void listFree(LIST *list);
#endif