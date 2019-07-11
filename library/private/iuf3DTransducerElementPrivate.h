#ifndef IUFLIBRARY_IUFHL3DTRANSDUCERELEMENTIMP_H
#define IUFLIBRARY_IUFHL3DTRANSDUCERELEMENTIMP_H


#include <hdf5.h>
#include <iuf3DTransducerElement.h>


int iuf3DTransducerElementSave
(
    iu3dte_t element,
    hid_t handle
);

iu3dte_t iuf3DTransducerElementLoad
(
    hid_t handle
);


#endif //IUFLIBRARY_IUFHL3DTRANSDUCERELEMENTIMP_H
