// Created by nlv09165 on 12/09/2018.
#ifndef IUSLIBRARY_IUSPARAMETERDICTIMP_H
#define IUSLIBRARY_IUSPARAMETERDICTIMP_H

#include <hdf5.h>
#include <iusParameterDict.h>


int iusParameterDictSave
(
    iupad_t dict,
    hid_t handle
);

iupad_t iusParameterDictLoad
(
    hid_t handle
);

#endif //IUSLIBRARY_IUSPARAMETERDICTIMP_H
