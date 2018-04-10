//
// Created by Ruijzendaal on 10/04/2018.
//

#ifndef IUSLIBRARY_IUSHLPARAMETRICTRANSMITPULSELIST_H
#define IUSLIBRARY_IUSHLPARAMETRICTRANSMITPULSELIST_H

#include "iusHLParametricTransmitPulse.h"

#ifndef IUSLIBRARY_IMPLEMENTATION
typedef  struct IusParametricTransmitPulseList IusParametricTransmitPulseList;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusParametricTransmitPulseList   * iuptpl_t;
#define  IUPTPL_INVALID (iuptpl_t) NULL


// operations
iuptpl_t iusHLCreateParametricTransmitPulseList
(
    int numTransmitPulses
);

int iusHLDeleteParametricTransmitPulseList
(
    iuptpl_t pulseList
);

int iusHLParametricTransmitPulseListSet
(
    iuptpl_t list,
    float pulseFrequency,
    float pulseAmplitude,
    int pulseCount,
    int pulseIndex
);

iuptp_t iusHLParametricTransmitPulseListGet
(
    iuptpl_t list,
    int pulseIndex
);

int iusHLParametricTransmitPulseListGetSize
(
    iuptpl_t list
);

int iusCompareParametricTransmitPulseList
(
    iuptpl_t reference,
    iuptpl_t actual
);

#endif //IUSLIBRARY_IUSHLPARAMETRICTRANSMITPULSELIST_H
