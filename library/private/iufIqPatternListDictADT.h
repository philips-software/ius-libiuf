// Created by nlv09165 on 10/08/2018.
#ifndef IUFLIBRARY_IUFIQPATTERNLISTDICTADT_H
#define IUFLIBRARY_IUFIQPATTERNLISTDICTADT_H

#include <hashmap.h>

// ADT
struct HashableIqPatternList
{
    iuiqpal_t iqPatternList;
    char key[256];
};

typedef struct HashableIqPatternList HashableIqPatternList;

struct IufIqPatternListDict
{
    struct hashmap map;
    IUF_BOOL deepDelete;
    char **kys;
};
#endif //IUFLIBRARY_IUFIQPATTERNLISTADT_H

