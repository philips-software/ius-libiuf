

// Created by nlv09165 on 18/07/2018.
#ifndef IUSLIBRARY_IUSHLPATTERNLIST_H
#define IUSLIBRARY_IUSHLPATTERNLIST_H

#include <include/iusHLPattern.h>

// ADT
typedef struct IusPatternList IusPatternList;
typedef IusPatternList *iupal_t;
#define  IUPAL_INVALID (iupal_t) NULL

iupal_t iusHLPatternListCreate
(
    int numPattern
);

int iusHLPatternListDelete
(
    iupal_t list
);

int iusHLPatternListCompare
(
    iupal_t reference,
    iupal_t actual
);

int iusHLPatternListGetSize
(
    iupal_t list
);

iupa_t iusHLPatternListGet
(
    iupal_t list,
    int index
);

int iusHLPatternListSet
(
    iupal_t list,
    iupa_t member,
    int index
);

#endif //IUSLIBRARY_IUSHLPATTERNLIST_H
