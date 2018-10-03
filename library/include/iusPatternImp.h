//
// Created by Ruijzendaal on 18/07/2018.
//

#ifndef IUSLIBRARY_IUSHLPATTERNIMP_H
#define IUSLIBRARY_IUSHLPATTERNIMP_H

#include <hdf5.h>
#include <iusPattern.h>

int iusPatternSave
(
    iupa_t pulse,
    hid_t handle
);

iupa_t iusPatternLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHLPATTERNIMP_H
