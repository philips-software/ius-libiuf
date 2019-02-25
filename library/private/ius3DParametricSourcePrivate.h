//
// Created by Ruijzendaal on 30/07/2018.
//

#ifndef IUSLIBRARY_IUSHL3DPARAMETRICSOURCEIMP_H
#define IUSLIBRARY_IUSHL3DPARAMETRICSOURCEIMP_H

#include <hdf5.h>
#include <ius3DParametricSource.h>

int ius3DParametricSourceSave
(
    ius_t source,
    hid_t handle
);

ius_t ius3DParametricSourceLoad
(
    hid_t handle
);

#endif //IUSLIBRARY_IUSHL3DPARAMETRICSOURCEIMP_H
