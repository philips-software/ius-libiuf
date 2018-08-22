
// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLISTIMP_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLISTIMP_H

#include <hdf5.h>
#include <include/iusHL3DTransducerElementList.h>


int iusHL3DTransducerElementListSave
(
    iu3dtel_t list,
    const char *parentPath,
    hid_t handle
);

iu3dtel_t iusHL3DTransducerElementListLoad
(
    hid_t handle,
    const char *parentPath
);


#endif //IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLISTIMP_H
