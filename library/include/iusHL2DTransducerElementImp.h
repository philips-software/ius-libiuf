// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTIMP_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTIMP_H


#include <hdf5.h>
#include <include/iusHL2DTransducerElement.h>


int iusHL2DTransducerElementSave
(
    iu2dte_t element,
    const char *parentPath,
    hid_t handle
);

iu2dte_t iusHL2DTransducerElementLoad
(
    hid_t handle,
    const char *parentPath
);


#endif //IUSLIBRARY_IUSHL2DTRANSDUCERELEMENTIMP_H
