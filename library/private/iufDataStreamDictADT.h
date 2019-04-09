// Created by nlv09165 on 10/08/2018.
#ifndef IUFLIBRARY_IUFHLDATASTREAMDICTADT_H
#define IUFLIBRARY_IUFHLDATASTREAMDICTADT_H

#include <hashmap.h>

// ADT
struct HashableDataStream
{
    iuds_t dataStream;
    char key[256];
};

typedef struct HashableDataStream HashableDataStream;

struct IufDataStreamDict
{
    struct hashmap map;
    IUF_BOOL deepDelete;
    char **kys;
};
#endif // IUFLIBRARY_IUFHLDATASTREAMDICTADT_H

