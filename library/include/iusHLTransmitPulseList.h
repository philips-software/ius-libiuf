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

#endif //IUSLIBRARY_IUSHLTRANSMITPULSELIST_H
