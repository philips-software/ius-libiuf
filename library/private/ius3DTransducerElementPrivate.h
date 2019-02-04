// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTIMP_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTIMP_H


#include <hdf5.h>
#include <ius3DTransducerElement.h>


int ius3DTransducerElementSave
(
    iu3dte_t element,
    hid_t handle
);

iu3dte_t ius3DTransducerElementLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHL3DTRANSDUCERELEMENTIMP_H
