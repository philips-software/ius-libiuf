//
// Created by Ruijzendaal on 30/07/2018.
//

#ifndef IUSLIBRARY_IUSHL3DPARAMETRICSOURCEIMP_H
#define IUSLIBRARY_IUSHL3DPARAMETRICSOURCEIMP_H

#include <hdf5.h>
#include <iusHL3DParametricSource.h>

int iusHL3DParametricSourceSave
(
    iu3dps_t source,
    char *parentPath,
    hid_t handle
);

iu3dps_t iusHL3DParametricSourceLoad
(
    hid_t handle,
    char *parentPath,
    char *label
);

#endif //IUSLIBRARY_IUSHL3DPARAMETRICSOURCEIMP_H
