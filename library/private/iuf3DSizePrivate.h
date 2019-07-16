#ifndef IUFLIBRARY_IUFHL3DSIZEIMP_H
#define IUFLIBRARY_IUFHL3DSIZEIMP_H

#include <hdf5.h>
#include <iuf3DSize.h>

iu3ds_t iuf3DSizeLoad
(
    hid_t handle
);

int iuf3DSizeSave
(
    iu3ds_t size,
    hid_t handle
);

#endif //IUFLIBRARY_IUFHL3DSIZEIMP_H
