#ifndef IUFLIBRARY_IUFHL3DNONPARAMETRICSOURCEIMP_H
#define IUFLIBRARY_IUFHL3DNONPARAMETRICSOURCEIMP_H


#include <hdf5.h>
#include <iuf3DNonParametricSource.h>

int iuf3DNonParametricSourceSave
(
    iu3dnps_t source,
    hid_t handle
);

iu3dnps_t iuf3DNonParametricSourceLoad
(
    hid_t handle
);

#endif //IUFLIBRARY_IUFHL3DNONPARAMETRICSOURCEIMP_H
