//
// Created by Ruijzendaal on 26/07/2018.
//

#ifndef IUSLIBRARY_IUSHLSOURCEIMP_H
#define IUSLIBRARY_IUSHLSOURCEIMP_H

#include <iusTypes.h>
#include <iusHLSource.h>
#include <hdf5.h>

struct IusSource
{
    IusSourceType type;
    char *label;
} ;


IUS_BOOL iusHLBaseSourceCompare
(
    ius_t reference,
    ius_t actual
);

int iusHLBaseSourceSave
(
    ius_t source,
    hid_t handle
);

int iusHLSourceSave
(
    ius_t source,
    hid_t handle
);

ius_t iusHLSourceLoad
(
    hid_t handle,
	char *label
);

#endif //IUSLIBRARY_IUSHLSOURCEIMP_H
