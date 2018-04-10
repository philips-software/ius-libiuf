//
// Created by Ruijzendaal on 10/04/2018.
//

#ifndef IUSLIBRARY_IUSHLNONPARAMETRICTRANSMITPULSELIST_H
#define IUSLIBRARY_IUSHLNONPARAMETRICTRANSMITPULSELIST_H


#include "iusHLNonParametricTransmitPulse.h"

#ifndef IUSLIBRARY_IMPLEMENTATION
typedef struct IusNonParametricTransmitPulseList IusNonParametricTransmitPulseList;
#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef IusNonParametricTransmitPulseList *iunptpl_t;
#define  IUNPTPL_INVALID (iunptpl_t) NULL


// operations
iunptpl_t iusHLCreateNonParametricTransmitPulseList
(
    int numTransmitPulses,
    int numPulseValues
);

int iusHLDeleteNonParametricTransmitPulseList
(
    iunptpl_t list
);

iunptp_t iusHLNonParametricTransmitPulseListGet
(
    iunptpl_t list,
    int pulseIndex
);

int iusHLNonParametricTransmitPulseListSet
(
    iunptpl_t list,
    iunptp_t pulse,
    int pulseIndex
);

int iusHLNonParametricTransmitPulseListGetSize
(
    iunptpl_t list
);

int iusCompareNonParametricTransmitPulseList
(
    iunptpl_t reference,
    iunptpl_t actual
);


#endif //IUSLIBRARY_IUSHLNONPARAMETRICTRANSMITPULSELIST_H
