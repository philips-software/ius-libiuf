// Created by nlv09165 on 20/08/2018.
#ifndef IUSLIBRARY_IUSHL3DANGLEIMP_H
#define IUSLIBRARY_IUSHL3DANGLEIMP_H

#include <hdf5.h>
#include <ius3DAngle.h>

iu3da_t ius3DAngleLoad
(
    hid_t handle,
    char *parentPath
);

int ius3DAngleSave
(
    iu3da_t angle,
    char *parentPath,
    hid_t handle
);

#endif //IUSLIBRARY_IUSHL3DANGLEIMP_H
