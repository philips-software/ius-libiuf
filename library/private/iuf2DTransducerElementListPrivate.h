// Created by nlv09165 on 22/08/2018.
#ifndef IUFLIBRARY_IUFHL2DTRANSDUCERELEMENTLISTIMP_H
#define IUFLIBRARY_IUFHL2DTRANSDUCERELEMENTLISTIMP_H

#include <hdf5.h>
#include <iuf2DTransducerElementList.h>

// ADT
struct Iuf2DTransducerElementList
{
    int numElements;
    iu2dte_t *   p2DTransducerElements ;
    IUF_BOOL deepDelete;
} ;


int iuf2DTransducerElementListSave
(
    iu2dtel_t list,
    hid_t handle
);

iu2dtel_t iuf2DTransducerElementListLoad
(
    hid_t handle
);


#endif //IUFLIBRARY_IUFHL2DTRANSDUCERELEMENTLISTIMP_H
