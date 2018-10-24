//
// Created by Ruijzendaal on 08/08/2018.
//

#ifndef IUSLIBRARY_IUSHLTGCIMP_H
#define IUSLIBRARY_IUSHLTGCIMP_H

#include <iusTGC.h>
#include <hdf5.h>
#include <hdf5_hl.h>

int iusTGCSave
(
    iutgc_t iusTGC,
    hid_t handle
);

iutgc_t iusTGCLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHLTGCIMP_H
