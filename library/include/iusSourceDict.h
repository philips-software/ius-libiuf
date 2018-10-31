// Created by nlv09165 on 31/07/2018.
#ifndef IUSLIBRARY_IUSHLSOURCEDICT_H
#define IUSLIBRARY_IUSHLSOURCEDICT_H

#include <iusHDF5.h>
#include <iusSource.h>

// ADT
typedef struct IusSourceDict IusSourceDict;
typedef IusSourceDict *iusd_t;
#define  IUSD_INVALID (iusd_t) NULL

iusd_t iusSourceDictCreate
(
    void
);

int iusSourceDictDelete
(
    iusd_t dict
);

int iusSourceDictDeepDelete
(
    iusd_t dict
);

int iusSourceDictCompare
(
    iusd_t reference,
    iusd_t actual
);

int iusSourceDictGetSize
(
    iusd_t dict
);

ius_t iusSourceDictGet
(
    iusd_t list,
    char * key
);

int iusSourceDictSet
(
    iusd_t dict,
    char * key,
    ius_t member
);

#endif //IUSLIBRARY_IUSHLSOURCEDICT_H
