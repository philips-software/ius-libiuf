#ifndef IUFLIBRARY_IUFHL3DTRANSDUCERELEMENTLISTIMP_H
#define IUFLIBRARY_IUFHL3DTRANSDUCERELEMENTLISTIMP_H

#include <hdf5.h>
#include <iuf3DTransducerElementList.h>


int iuf3DTransducerElementListSave
(
    iu3dtel_t list,
    hid_t handle
);

iu3dtel_t iuf3DTransducerElementListLoad
(
    hid_t handle
);


#endif //IUFLIBRARY_IUFHL3DTRANSDUCERELEMENTLISTIMP_H
