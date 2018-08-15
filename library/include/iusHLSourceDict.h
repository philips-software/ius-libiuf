// Created by nlv09165 on 31/07/2018.
#ifndef IUSLIBRARY_IUSHLSOURCEDICT_H
#define IUSLIBRARY_IUSHLSOURCEDICT_H

#include <iusHDF5.h>
#include <iusHLSource.h>

// ADT
typedef struct IusSourceDict IusSourceDict;
typedef IusSourceDict *iusd_t;
#define  IUSD_INVALID (iusd_t) NULL

iusd_t iusHLSourceDictCreate
(
    void
);

int iusHLSourceDictDelete
(
    iusd_t dict
);

int iusHLSourceDictCompare
(
    iusd_t reference,
    iusd_t actual
);

int iusHLSourceDictGetSize
(
    iusd_t dict
);

ius_t iusHLSourceDictGet
(
    iusd_t list,
    char * key
);

int iusHLSourceDictSet
(
    iusd_t dict,
    char * key,
    ius_t member
);

#endif //IUSLIBRARY_IUSHLSOURCEDICT_H
