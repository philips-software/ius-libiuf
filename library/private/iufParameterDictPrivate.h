// Created by nlv09165 on 12/09/2018.
#ifndef IUFLIBRARY_IUFPARAMETERDICTIMP_H
#define IUFLIBRARY_IUFPARAMETERDICTIMP_H

#include <hdf5.h>
#include <iufParameterDict.h>


int iufParameterDictSave
(
    iupad_t dict,
    hid_t handle
);

iupad_t iufParameterDictLoad
(
    hid_t handle
);

#endif //IUFLIBRARY_IUFPARAMETERDICTIMP_H
