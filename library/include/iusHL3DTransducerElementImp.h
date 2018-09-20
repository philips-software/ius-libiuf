// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTIMP_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTIMP_H


#include <hdf5.h>
#include <include/iusHL3DTransducerElement.h>


int iusHL3DTransducerElementSave
(
    iu3dte_t element,
    hid_t handle
);

iu3dte_t iusHL3DTransducerElementLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTIMP_H
