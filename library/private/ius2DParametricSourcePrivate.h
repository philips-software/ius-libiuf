//
// Created by Ruijzendaal on 30/07/2018.
//

#ifndef IUSLIBRARY_IUSHL2DPARAMETRICSOURCEIMP_H
#define IUSLIBRARY_IUSHL2DPARAMETRICSOURCEIMP_H

#include <hdf5.h>
#include <ius2DParametricSource.h>

int ius2DParametricSourceSave
(
    ius_t source,
    hid_t handle
);

ius_t ius2DParametricSourceLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHL2DPARAMETRICSOURCEIMP_H
