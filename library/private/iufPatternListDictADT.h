#ifndef IUFLIBRARY_IUFPATTERNLISTDICTADT_H
#define IUFLIBRARY_IUFPATTERNLISTDICTADT_H

#include <iufPatternList.h>
#include <hashmap.h>

// ADT
struct HashablePatternList
{
	iupal_t patternList;
	char key[256];
};

typedef struct HashablePatternList HashablePatternList;

struct IufPatternListDict
{
	struct hashmap map;
	IUF_BOOL deepDelete;
    char **kys;
};

#endif
