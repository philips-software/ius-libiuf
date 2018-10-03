// Created by nlv09165 on 20/08/2018.
#ifndef IUSLIBRARY_IUSHL2DSIZEIMP_H
#define IUSLIBRARY_IUSHL2DSIZEIMP_H

#include <hdf5.h>
#include <ius2DSize.h>

iu2ds_t ius2DSizeLoad
(
    hid_t handle
);

int ius2DSizeSave
(
    iu2ds_t size,
    hid_t handle
);

#endif //IUSLIBRARY_IUSHL2DSIZEIMP_H
