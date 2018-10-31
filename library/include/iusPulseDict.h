// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSHLPULSEDICT_H
#define IUSLIBRARY_IUSHLPULSEDICT_H

#include <iusHDF5.h>
#include <iusPulse.h>

// ADT
typedef struct IusPulseDict IusPulseDict;
typedef IusPulseDict *iupd_t;
#define  IUPD_INVALID (iupd_t) NULL

iupd_t iusPulseDictCreate
(
    void
);

int iusPulseDictDelete
(
    iupd_t dict
);

int iusPulseDictDeepDelete
(
    iupd_t dict
);

int iusPulseDictCompare
(
    iupd_t reference,
    iupd_t actual
);

int iusPulseDictGetSize
(
    iupd_t dict
);

iup_t iusPulseDictGet
(
    iupd_t list,
    char * key
);

int iusPulseDictSet
(
    iupd_t dict,
    char * key,
    iup_t member
);

#endif //IUSLIBRARY_IUSHLPULSEDICT_H
