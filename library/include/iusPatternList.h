

// Created by nlv09165 on 18/07/2018.
#ifndef IUSLIBRARY_IUSHLPATTERNLIST_H
#define IUSLIBRARY_IUSHLPATTERNLIST_H

#include <include/iusPattern.h>
#include <iusReceiveSettingsDict.h>
#include <iusReceiveChannelMapDict.h>

// ADT
typedef struct IusPatternList IusPatternList;
typedef IusPatternList *iupal_t;
#define  IUPAL_INVALID (iupal_t) NULL

iupal_t iusPatternListCreate
(
    int numPattern,
    iursd_t receiveSettingsDict,
    iurcmd_t receiveChannelMapDict
);

int iusPatternListDelete
(
    iupal_t list
);

int iusPatternListDeepDelete
(
    iupal_t list
);

int iusPatternListCompare
(
    iupal_t reference,
    iupal_t actual
);

int iusPatternListGetSize
(
    iupal_t list
);

iupa_t iusPatternListGet
(
    iupal_t list,
    int index
);

int iusPatternListSet
(
    iupal_t list,
    iupa_t member,
    int index
);

#endif //IUSLIBRARY_IUSHLPATTERNLIST_H
