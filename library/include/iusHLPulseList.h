// Created by nlv09165 on 02/05/2018.
#ifndef IUSLIBRARY_IUSHLPULSELIST_H
#define IUSLIBRARY_IUSHLPULSELIST_H

#include <iusHLPulse.h>

// ADT
typedef struct IusPulseList IusPulseList;
typedef IusPulseList *iupl_t;
#define  IUPL_INVALID (iupl_t) NULL

iupl_t iusHLPulseListCreate
(
    int numPulse
);

int iusHLPulseListDelete
(
    iupl_t list
);

int iusHLPulseListCompare
(
    iupl_t reference,
    iupl_t actual
);

int iusHLPulseListGetSize
(
    iupl_t list
);

iup_t iusHLPulseListGet
(
    iupl_t list,
    int index
);

int iusHLPulseListSet
(
    iupl_t list,
    iup_t member,
    int index
);

#endif //IUSLIBRARY_IUSHLPULSELIST_H
