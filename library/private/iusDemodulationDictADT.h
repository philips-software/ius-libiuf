// Created by nlv09165 on 10/08/2018.
#ifndef IUSLIBRARY_IUSHLDEMODULATIONDICTADT_H
#define IUSLIBRARY_IUSHLDEMODULATIONDICTADT_H

#include <hashmap.h>

// ADT
struct HashableDemodulation
{
    iudm_t demodulation;
    char key[256];
};

typedef struct HashableDemodulation HashableDemodulation;

struct IusDemodulationDict
{
    struct hashmap map;
    IUS_BOOL deepDelete;
    char **keys;
};
#endif //IUSLIBRARY_IUSHLDEMODULATIONDICTADT_H

