// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTIMP_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTIMP_H


#include <hdf5.h>
#include <ius2DTransducerElement.h>

struct Ius2DTransducerElement
{
    iu2dp_t   position;
    float     theta;
    iu2ds_t   size;
    IUS_BOOL  deepDelete;
} ;

int ius2DTransducerElementSave
(
    iu2dte_t element,
    hid_t handle
);

iu2dte_t ius2DTransducerElementLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTIMP_H
