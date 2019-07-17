#ifndef IUFLIBRARY_IUFHL2DPARAMETRICSOURCEIMP_H
#define IUFLIBRARY_IUFHL2DPARAMETRICSOURCEIMP_H

#include <hdf5.h>
#include <iuf2DParametricSource.h>

int iuf2DParametricSourceSave
(
    iu2dps_t source,
    hid_t handle
);

iu2dps_t iuf2DParametricSourceLoad
(
    hid_t handle
);


#endif //IUFLIBRARY_IUFHL2DPARAMETRICSOURCEIMP_H
