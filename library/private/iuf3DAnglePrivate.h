#ifndef IUFLIBRARY_IUFHL3DANGLEIMP_H
#define IUFLIBRARY_IUFHL3DANGLEIMP_H

#include <hdf5.h>
#include <iuf3DAngle.h>

iu3da_t iuf3DAngleLoad
(
    hid_t handle
);

int iuf3DAngleSave
(
    iu3da_t angle,
    hid_t handle
);

#endif //IUFLIBRARY_IUFHL3DANGLEIMP_H
