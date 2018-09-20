// Created by nlv09165 on 20/08/2018.
#ifndef IUSLIBRARY_IUSHL3DANGLEIMP_H
#define IUSLIBRARY_IUSHL3DANGLEIMP_H

#include <hdf5.h>
#include <iusHL3DAngle.h>

iu3da_t iusHL3DAngleLoad
(
    hid_t handle
);

int iusHL3DAngleSave
(
    iu3da_t angle,
    hid_t handle
);

#endif //IUSLIBRARY_IUSHL3DANGLEIMP_H
