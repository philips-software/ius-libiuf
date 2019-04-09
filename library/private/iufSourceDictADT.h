// Created by nlv12901 on 21/03/2019.
#ifndef IUFLIBRARY_IUFSOURCEDICTADT_H
#define IUFLIBRARY_IUFSOURCEDICTADT_H

#include <hashmap.h>

// ADT
struct HashableSource
{
    ius_t source;
    char key[256];
};

typedef struct HashableSource HashableSource;

struct IufSourceDict
{
    struct hashmap map;
    IUF_BOOL deepDelete;
    char **kys;
};
#endif //IUFLIBRARY_IUFSOURCEDICTADT_H

