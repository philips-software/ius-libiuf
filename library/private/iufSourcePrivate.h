#ifndef IUFLIBRARY_IUFHLSOURCEIMP_H
#define IUFLIBRARY_IUFHLSOURCEIMP_H

#include <iufTypes.h>
#include <iufSource.h>
#include <hdf5.h>
#include <hdf5_hl.h>


struct IufSource
{
    IufSourceType type;
} ;

IUF_BOOL iufBaseSourceCompare
(
    ius_t reference,
    ius_t actual
);

int iufBaseSourceSave
(
    ius_t source,
    hid_t handle
);

int iufSourceSave
(
    ius_t source,
    hid_t handle
);

ius_t iufSourceLoad
(
    hid_t handle
);

#endif //IUFLIBRARY_IUFHLSOURCEIMP_H
