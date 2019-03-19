//
// Created by Ruijzendaal on 30/07/2018.
//

#ifndef IUFLIBRARY_IUFHL3DPARAMETRICSOURCEIMP_H
#define IUFLIBRARY_IUFHL3DPARAMETRICSOURCEIMP_H

#include <hdf5.h>
#include <iuf3DParametricSource.h>

int iuf3DParametricSourceSave
(
    iu3dps_t source,
    hid_t handle
);

iu3dps_t iuf3DParametricSourceLoad
(
    hid_t handle
);

#endif //IUFLIBRARY_IUFHL3DPARAMETRICSOURCEIMP_H
