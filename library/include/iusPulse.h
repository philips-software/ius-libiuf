//
// Created by Ruijzendaal on 30/03/2018.
//
#ifndef IUSLIBRARY_IUSHLPULSE_H
#define IUSLIBRARY_IUSHLPULSE_H

#include <iusTypes.h>

// ADT
struct   IusPulse;
typedef  struct IusPulse IusPulse;
typedef  IusPulse    * iup_t;
#define  IUP_INVALID (iup_t) NULL


typedef enum {
  IUS_INVALID_PULSETYPE = 0,
  IUS_PARAMETRIC_PULSETYPE,
  IUS_NON_PARAMETRIC_PULSETYPE
} IusPulseType;


//iup_t  iusPulseCreate
//(
//    IusPulseType type,
//    const char *label
//);

int iusPulseDelete
(
    iup_t pulse
);

IUS_BOOL  iusPulseCompare
(
    iup_t reference,
    iup_t acttual
);

// getters
IusPulseType iusPulseGetType
(
    iup_t pulse
);


char* iusPulseGetLabel
(
    iup_t pulse
);

#endif //IUSLIBRARY_IUSHLPULSE_H