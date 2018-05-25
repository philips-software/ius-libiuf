//
// Created by Ruijzendaal on 30/03/2018.
//
#ifndef IUSLIBRARY_IUSHLPULSE_H
#define IUSLIBRARY_IUSHLPULSE_H

#include <iusTypes.h>

// ADT
struct IusPulse;
typedef  struct IusPulse IusPulse;
typedef  IusPulse    * iup_t;
#define  IUP_INVALID (iup_t) NULL

iup_t  iusHLPulseCreate
(
    IusPulseType type,
    char *label
);

int iusHLPulseDelete
(
    iup_t pulse
);

IUS_BOOL  iusHLPulseCompare
(
    iup_t reference,
    iup_t acttual
);

// getters
IusPulseType iusHLPulseGetType
(
    iup_t pulse
);


char* iusHLPulseGetLabel
(
    iup_t pulse
);

#endif //IUSLIBRARY_IUSHLPULSE_H
