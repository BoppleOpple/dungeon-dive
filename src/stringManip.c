#include "stringManip.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *str_cut(char *str, int start, int end) {
	// allocate a new string of size end - start + 2 (length of the string + 1 for
	// including the end + 1 for termination)
	char *cut = malloc(sizeof(char) * (end - start + 2));

	printf("%i\n", end - start+2);
	memcpy(cut, str + start, sizeof(char) * (end - start + 1));
	*(cut + end - start + 1) = 0;

	return cut;
}

char *str_trim(char *str){
	int start = 0;
	int end = strlen(str) - 1;

	// trimming leftmost whitespace
	while (isspace(*(str + start))) start++;

	// trimming righttmost whitespace
	while (isspace(*(str + end))) end--;

	// return the substring between `start` and `end`
	return str_cut(str, start, end);
}