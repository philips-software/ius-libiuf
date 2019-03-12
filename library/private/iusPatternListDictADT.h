#ifndef IUSLIBRARY_IUSPATTERNLISTDICTADT_H
#define IUSLIBRARY_IUSPATTERNLISTDICTADT_H

#include <iusPatternList.h>
#include <hashmap.h>

// ADT
struct HashablePatternList
{
	iupal_t patternList;
	char key[256];
};

typedef struct HashablePatternList HashablePatternList;

struct IusPatternListDict
{
	struct hashmap map;
	IUS_BOOL deepDelete;
    char **kys;
};

#endif
