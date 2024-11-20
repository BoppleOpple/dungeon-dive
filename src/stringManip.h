#ifndef STR_MANIP_TRIM
#define STR_MANIP_TRIM
/*
 * trims whitespace from a c string and returns a new string
**/
char *str_trim(char *str);
#endif

#ifndef STR_MANIP_CUT
#define STR_MANIP_CUT
/*
 * takes a c string, start index, and end index, and returns a *copy* of the
 * substring from start to end, inclusive
**/
char *str_cut(char *str, int start, int end);
#endif