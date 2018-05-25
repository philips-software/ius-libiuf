//
// Created by Ruijzendaal on 12/04/2018.
//
#ifndef IUSLIBRARY_IUSHLSOURCELOCATIONLIST_H
#define IUSLIBRARY_IUSHLSOURCELOCATIONLIST_H

#include <iusTypes.h>
#include <iusHLPosition.h>


// ADT
typedef  struct IusSourceList IusSourceList;
typedef  IusSourceList   * iusl_t;
#define  IUSLL_INVALID (iusl_t) NULL

// operations
iusl_t iusHLCreateSourceList
(
    IusSourceType locationType,
    int numTransmitSources
);

int iusHLDeleteSourceList
(
    iusl_t list
);

int iusHLSourceLocationListSet
(
    iusl_t list,
    int index
);

void* iusHLSourceLocationListGet
(
    iusl_t list,
    int index
);


int iusHLSourceListSet2DPosition
(
    iusl_t pList,
    iu2dp_t pos,
    int index
);

int iusHLSourceListSet3DPosition
(
    iusl_t list,
    iu3dp_t pos,
    int index
);

int iusHLSourceListGetSize
(
    iusl_t list
);

int iusHLCompareSourceList
(
    iusl_t reference,
    iusl_t actual
);

#endif //IUSLIBRARY_IUSHLSOURCELOCATIONLIST_H
