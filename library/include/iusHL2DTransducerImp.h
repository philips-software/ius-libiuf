// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERIMP_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERIMP_H

#include <iusHL2DTransducer.h>

herr_t iusHL2DTransducerSave
(
    iu2dt_t transducer,
    hid_t handle
);

iu2dt_t iusHL2DTransducerLoad
(
    hid_t handle
);

#endif //IUSLIBRARY_IUSHL2DTRANSDUCERIMP_H
