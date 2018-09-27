//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUSLIBRARY_IUSHLPULSEIMP_H
#define IUSLIBRARY_IUSHLPULSEIMP_H

#include <iusTypes.h>
#include <iusHLPulse.h>
#include <hdf5.h>

// ADT
struct IusPulse
{
  IusPulseType type;
  char *label;
} ;


int iusHLPulseSave
(
    iup_t pulse,
    hid_t handle
);

iup_t iusHLPulseLoad
(
    hid_t handle
);

int iusHLBasePulseSave
(
    iup_t pulse,
    hid_t handle
);

iup_t iusHLBasePulseLoad
(
    hid_t handle
);

IUS_BOOL iusHLBasePulseCompare
(
    iup_t reference,
    iup_t actual
);



#endif //IUSLIBRARY_IUSHLPULSEIMP_H
