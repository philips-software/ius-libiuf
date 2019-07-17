#ifndef IUFLIBRARY_IUFHLFRAMEIMP_H
#define IUFLIBRARY_IUFHLFRAMEIMP_H

#include <iufTypes.h>
#include <iufFrame.h>
#include <hdf5.h>

// ADT
int iufFrameSave
(
    iufr_t frame,
    hid_t handle
);

iufr_t iufFrameLoad
(
    hid_t handle
);

#endif //IUFLIBRARY_IUFHLFRAMEIMP_H
