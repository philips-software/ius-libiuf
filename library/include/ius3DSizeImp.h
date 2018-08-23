// Created by nlv09165 on 17/08/2018.
#ifndef IUSLIBRARY_IUSHL3DSIZEIMP_H
#define IUSLIBRARY_IUSHL3DSIZEIMP_H

#include <hdf5.h>
#include <ius3DSize.h>

iu3ds_t ius3DSizeLoad
(
    hid_t handle,
    char *parentPath
);

int ius3DSizeSave
(
    iu3ds_t size,
    char *parentPath,
    hid_t handle
);

#endif //IUSLIBRARY_IUSHL3DSIZEIMP_H
