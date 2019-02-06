// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL3DTRANSDUCERIMP_H
#define IUSLIBRARY_IUSHL3DTRANSDUCERIMP_H

#include <ius3DTransducer.h>

herr_t ius3DTransducerSave
(
    iu3dt_t transducer,
    hid_t handle
);

iu3dt_t ius3DTransducerLoad
(
    hid_t handle
);

#endif //IUSLIBRARY_IUSHL3DTRANSDUCERIMP_H
