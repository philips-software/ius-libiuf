#ifndef IUFLIBRARY_IUFPULSEDICTADT_H
#define IUFLIBRARY_IUFPULSEDICTADT_H

#include <hashmap.h>

// ADT
struct HashablePulse
{
    iup_t pulse;
    char key[256];
};

typedef struct HashablePulse HashablePulse;

struct IufPulseDict
{
    struct hashmap map;
    IUF_BOOL deepDelete;
    char **kys;
};
#endif //IUFLIBRARY_IUFPULSEDICTADT_H

