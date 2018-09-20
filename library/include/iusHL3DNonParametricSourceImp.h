//
// Created by Ruijzendaal on 30/07/2018.
//

#ifndef IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCEIMP_H
#define IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCEIMP_H


#include <hdf5.h>
#include <iusHL3DNonParametricSource.h>

int iusHL3DNonParametricSourceSave
(
    iu3dnps_t source,
    hid_t handle
);

iu3dnps_t iusHL3DNonParametricSourceLoad
(
    hid_t handle,
    char *label
);

#endif //IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCEIMP_H
