//
// Created by Ruijzendaal on 31/07/2018.
//

#ifndef IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCEIMP_H
#define IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCEIMP_H


#include <hdf5.h>
#include <ius2DNonParametricSource.h>

int ius2DNonParametricSourceSave
(
    iu2dnps_t source,
    char *parentPath,
    hid_t handle
);

iu2dnps_t ius2DNonParametricSourceLoad
(
    hid_t handle,
    char *parentPath,
    char *label
);

#endif //IUSLIBRARY_IUSHL2DNONPARAMETRICSOURCEIMP_H
