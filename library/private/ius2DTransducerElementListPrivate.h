// Created by nlv09165 on 22/08/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLISTIMP_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLISTIMP_H

#include <hdf5.h>
#include <ius2DTransducerElementList.h>

// ADT
struct Ius2DTransducerElementList
{
    int numElements;
    iu2dte_t *   p2DTransducerElements ;
    IUS_BOOL deepDelete;
} ;


int ius2DTransducerElementListSave
(
    iu2dtel_t list,
    hid_t handle
);

iu2dtel_t ius2DTransducerElementListLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTLISTIMP_H
