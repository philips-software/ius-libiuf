// Created by nlv09165 on 03/05/2018.
#ifndef IUSLIBRARY_IUSHLPULSEDICT_H
#define IUSLIBRARY_IUSHLPULSEDICT_H

#include <iusHDF5.h>
#include <iusHLPulse.h>

// ADT
typedef struct IusPulseDict IusPulseDict;
typedef IusPulseDict *iupd_t;
#define  IUPD_INVALID (iupd_t) NULL

iupd_t iusHLPulseDictCreate
(
    void
);

int iusHLPulseDictDelete
(
    iupd_t dict
);

int iusHLPulseDictCompare
(
    iupd_t reference,
    iupd_t actual
);

int iusHLPulseDictGetSize
(
    iupd_t dict
);

iup_t iusHLPulseDictGet
(
    iupd_t list,
    char * key
);

int iusHLPulseDictSet
(
    iupd_t dict,
    char * key,
    iup_t member
);

#endif //IUSLIBRARY_IUSHLPULSEDICT_H
