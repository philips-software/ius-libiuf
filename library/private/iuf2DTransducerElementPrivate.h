#ifndef IUFLIBRARY_IUFHL2DTRANSDUCERELEMENTIMP_H
#define IUFLIBRARY_IUFHL2DTRANSDUCERELEMENTIMP_H


#include <hdf5.h>
#include <iuf2DTransducerElement.h>

struct Iuf2DTransducerElement
{
    iu2dp_t   position;
    float     theta;
    iu2ds_t   size;
    IUF_BOOL  deepDelete;
} ;

int iuf2DTransducerElementSave
(
    iu2dte_t element,
    hid_t handle
);

iu2dte_t iuf2DTransducerElementLoad
(
    hid_t handle
);


#endif //IUFLIBRARY_IUFHL2DTRANSDUCERELEMENTIMP_H
