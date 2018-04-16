//
// Created by Ruijzendaal on 05/04/2018.
//

#ifndef IUSLIBRARY_IUSHLTRANSMITPULSELIST_H
#define IUSLIBRARY_IUSHLTRANSMITPULSELIST_H

#include "iusTypes.h"
#include "iusHLTransmitPulse.h"

#ifndef IUSLIBRARY_IMPLEMENTATION
typedef  struct IusTransmitPulseList IusTransmitPulseList;

#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusTransmitPulseList   * iutpl_t;
#define  IUTPL_INVALID (iutpl_t) NULL


// operations

iutpl_t iusHLCreateTransmitPulseList
(
    int numTransmitPulses
);

int iusHLTransmitPulseListSet
(
    iutpl_t list,
    iutp_t pulse,
    int index
);

IusTransmitPulse * iusHLTransmitPulseListGet
(
    iutpl_t list,
    int index
);

int iusHLTransmitPulseListGetSize
(
    iutpl_t list
);

int iusCompareTransmitPulseList
(
    iutpl_t reference,
    iutpl_t actual
);

#endif //IUSLIBRARY_IUSHLTRANSMITPULSELIST_H
