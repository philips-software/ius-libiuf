//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUSLIBRARY_IUSHLPULSEIMP_H
#define IUSLIBRARY_IUSHLPULSEIMP_H

#include <iusTypes.h>
#include <iusPulse.h>
#include <hdf5.h>

// ADT
struct IusPulse
{
  IusPulseType type;
  char *label;
} ;


int iusPulseSave
(
    iup_t pulse,
    hid_t handle
);

iup_t iusPulseLoad
(
    hid_t handle
);

int iusBasePulseSave
(
    iup_t pulse,
    hid_t handle
);

iup_t iusBasePulseLoad
(
    hid_t handle
);

IUS_BOOL iusBasePulseCompare
(
    iup_t reference,
    iup_t actual
);



#endif //IUSLIBRARY_IUSHLPULSEIMP_H
