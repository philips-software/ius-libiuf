#ifndef IUFLIBRARY_IUFHL2DSIZEIMP_H
#define IUFLIBRARY_IUFHL2DSIZEIMP_H

#include <hdf5.h>
#include <iuf2DSize.h>

iu2ds_t iuf2DSizeLoad
(
    hid_t handle
);

int iuf2DSizeSave
(
    iu2ds_t size,
    hid_t handle
);

#endif //IUFLIBRARY_IUFHL2DSIZEIMP_H
