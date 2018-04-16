//
// Created by Ruijzendaal on 12/04/2018.
//

#ifndef IUSLIBRARY_IUSHLSOURCELOCATIONLIST_H
#define IUSLIBRARY_IUSHLSOURCELOCATIONLIST_H
#include "iusTypes.h"
#include "iusHLPosition.h"

#ifndef IUSLIBRARY_IMPLEMENTATION
typedef  struct IusSourceLocationList IusSourceLocationList;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusSourceLocationList   * iusll_t;
#define  IUSLL_INVALID (iusll_t) NULL


// operations
iusll_t iusHLCreateSourceLocationList
(
    IusSourceLocationType locationType,
    int numTransmitSources
);

int iusHLSourceLocationListSet
(
    iusll_t list,
    int index
);

void* iusHLSourceLocationListGet
(
    iusll_t list,
    int index
);


int iusHLSourceLocationListSet2DPosition
(
    iusll_t pList,
    iu2dp_t dpos,
    int index
);

int iusHLSourceLocationListSet3DPosition
(
    iusll_t list,
    iu3dp_t pos,
    int index
);

int iusHLSourceLocationListGetSize
(
    iusll_t list
);

int iusCompareSourceLocationListList
(
    iusll_t reference,
    iusll_t actual
);

#endif //IUSLIBRARY_IUSHLSOURCELOCATIONLIST_H
