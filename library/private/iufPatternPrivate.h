#ifndef IUFLIBRARY_IUFHLPATTERNIMP_H
#define IUFLIBRARY_IUFHLPATTERNIMP_H

#include <hdf5.h>
#include <iufPattern.h>

int iufPatternSave
(
    iupa_t pulse,
    hid_t handle
);

iupa_t iufPatternLoad
(
    hid_t handle
);


#endif //IUFLIBRARY_IUFHLPATTERNIMP_H
