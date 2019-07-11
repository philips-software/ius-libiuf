#ifndef IUFLIBRARY_IUFHLDEMODULATIONDICTADT_H
#define IUFLIBRARY_IUFHLDEMODULATIONDICTADT_H

#include <hashmap.h>

// ADT
struct HashableDemodulation
{
    iudm_t demodulation;
    char key[256];
};

typedef struct HashableDemodulation HashableDemodulation;

struct IufDemodulationDict
{
    struct hashmap map;
    IUF_BOOL deepDelete;
    char **kys;
};
#endif //IUFLIBRARY_IUFHLDEMODULATIONDICTADT_H

