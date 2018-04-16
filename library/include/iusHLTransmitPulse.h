//
// Created by Ruijzendaal on 30/03/2018.
//

#ifndef IUSLIBRARY_IUSHLTRANSMITPULSE_H
#define IUSLIBRARY_IUSHLTRANSMITPULSE_H

#include "iusTypes.h"

#ifndef IUSLIBRARY_IMPLEMENTATION


struct IusDrivingScheme;
typedef  struct IusTransmitPulse IusTransmitPulse;

#endif // IUSLIBRARY_IMPLEMENTATION

// ADT
typedef  IusTransmitPulse    * iutp_t;
#define  IUTP_INVALID (iutp_t) NULL

IUS_BOOL  iusCompareTransmitPulse
(
    iutp_t reference,
    iutp_t acttual
);
#endif //IUSLIBRARY_IUSHLTRANSMITPULSE_H
