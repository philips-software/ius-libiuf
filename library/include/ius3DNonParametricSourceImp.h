//
// Created by Ruijzendaal on 30/07/2018.
//

#ifndef IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCEIMP_H
#define IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCEIMP_H


#include <hdf5.h>
#include <ius3DNonParametricSource.h>

int ius3DNonParametricSourceSave
(
    iu3dnps_t source,
    hid_t handle
);

iu3dnps_t ius3DNonParametricSourceLoad
(
    hid_t handle,
    char *label
);

#endif //IUSLIBRARY_IUSHL3DNONPARAMETRICSOURCEIMP_H
