
// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLISTIMP_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLISTIMP_H

#include <hdf5.h>
#include <include/ius3DTransducerElementList.h>


int ius3DTransducerElementListSave
(
    iu3dtel_t list,
    hid_t handle
);

iu3dtel_t ius3DTransducerElementListLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTLISTIMP_H
