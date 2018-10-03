//
// Created by Ruijzendaal on 26/07/2018.
//

#ifndef IUSLIBRARY_IUSHLSOURCEIMP_H
#define IUSLIBRARY_IUSHLSOURCEIMP_H

#include <iusTypes.h>
#include <iusSource.h>
#include <hdf5.h>

struct IusSource
{
    IusSourceType type;
    char *label;
} ;


IUS_BOOL iusBaseSourceCompare
(
    ius_t reference,
    ius_t actual
);

int iusBaseSourceSave
(
    ius_t source,
    hid_t handle
);

int iusSourceSave
(
    ius_t source,
    hid_t handle
);

ius_t iusSourceLoad
(
    hid_t handle
);

#endif //IUSLIBRARY_IUSHLSOURCEIMP_H
