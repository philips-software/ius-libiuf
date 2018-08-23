// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL2DTRANSDUCERIMP_H
#define IUSLIBRARY_IUSHL2DTRANSDUCERIMP_H

#include <ius2DTransducer.h>

herr_t ius2DTransducerSave
(
    iu2dt_t transducer,
    char *parentPath,
    hid_t handle
);

iu2dt_t ius2DTransducerLoad
(
    hid_t handle,
    char *parentPath
);

#endif //IUSLIBRARY_IUSHL2DTRANSDUCERIMP_H
