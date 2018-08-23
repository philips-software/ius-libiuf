// Created by nlv09165 on 22/08/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLISTIMP_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLISTIMP_H

#include <hdf5.h>
#include <include/ius2DTransducerElementList.h>


int ius2DTransducerElementListSave
(
    iu2dtel_t list,
    const char *parentPath,
    hid_t handle
);

iu2dtel_t ius2DTransducerElementListLoad
(
    hid_t handle,
    const char *parentPath
);


#endif //IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLISTIMP_H
