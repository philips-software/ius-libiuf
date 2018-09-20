//
// Created by Ruijzendaal on 30/07/2018.
//

#ifndef IUSLIBRARY_IUSHL2DPARAMETRICSOURCEIMP_H
#define IUSLIBRARY_IUSHL2DPARAMETRICSOURCEIMP_H

#include <hdf5.h>
#include <iusHL2DParametricSource.h>

int iusHL2DParametricSourceSave
(
    iu2dps_t source,
    hid_t handle
);

iu2dps_t iusHL2DParametricSourceLoad
(
    hid_t handle,
    char *label
);


#endif //IUSLIBRARY_IUSHL2DPARAMETRICSOURCEIMP_H
