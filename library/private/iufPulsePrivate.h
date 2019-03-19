//
// Created by Ruijzendaal on 24/05/2018.
//
#ifndef IUFLIBRARY_IUFHLPULSEIMP_H
#define IUFLIBRARY_IUFHLPULSEIMP_H

#include <iufTypes.h>
#include <iufPulse.h>
#include <hdf5.h>
#include <hdf5_hl.h>

// ADT
struct IufPulse
{
  IufPulseType type;
};

int iufPulseSave
(
    iup_t pulse,
    hid_t handle
);

iup_t iufPulseLoad
(
    hid_t handle
);

int iufBasePulseSave
(
    iup_t pulse,
    hid_t handle
);

iup_t iufBasePulseLoad
(
    hid_t handle
);

IUF_BOOL iufBasePulseCompare
(
    iup_t reference,
    iup_t actual
);

#endif //IUFLIBRARY_IUFHLPULSEIMP_H
