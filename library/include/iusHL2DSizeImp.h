// Created by nlv09165 on 20/08/2018.
#ifndef IUSLIBRARY_IUSHL2DSIZEIMP_H
#define IUSLIBRARY_IUSHL2DSIZEIMP_H

#include <hdf5.h>
#include <iusHL2DSize.h>

iu2ds_t iusHL2DSizeLoad
(
    hid_t handle,
    char *parentPath
);

int iusHL2DSizeSave
(
    iu2ds_t size,
    char *parentPath,
    hid_t handle
);

#endif //IUSLIBRARY_IUSHL2DSIZEIMP_H
