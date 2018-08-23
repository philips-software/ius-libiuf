//
// Created by Ruijzendaal on 30/07/2018.
//

#ifndef IUSLIBRARY_IUSHL2DPARAMETRICSOURCEIMP_H
#define IUSLIBRARY_IUSHL2DPARAMETRICSOURCEIMP_H

#include <hdf5.h>
#include <ius2DParametricSource.h>

int ius2DParametricSourceSave
(
    iu2dps_t source,
    char *parentPath,
    hid_t handle
);

iu2dps_t ius2DParametricSourceLoad
(
    hid_t handle,
    char *parentPath,
    char *label
);


#endif //IUSLIBRARY_IUSHL2DPARAMETRICSOURCEIMP_H
