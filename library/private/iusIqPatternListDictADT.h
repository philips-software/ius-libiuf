// Created by nlv09165 on 10/08/2018.
#ifndef IUSLIBRARY_IUSIQPATTERNLISTDICTADT_H
#define IUSLIBRARY_IUSIQPATTERNLISTDICTADT_H

#include <hashmap.h>

// ADT
struct HashableIqPatternList
{
    iuiqpal_t iqPatternList;
    char key[256];
};

typedef struct HashableIqPatternList HashableIqPatternList;

struct IusIqPatternListDict
{
    struct hashmap map;
    IUS_BOOL deepDelete;
    char **kys;
};
#endif //IUSLIBRARY_IUSIQPATTERNLISTADT_H

