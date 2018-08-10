//
// Created by Ruijzendaal on 08/08/2018.
//

#ifndef IUSLIBRARY_IUSHLTGCIMP_H
#define IUSLIBRARY_IUSHLTGCIMP_H

#include <hdf5.h>
#include <iusHLTGC.h>

int iusHLTGCSave
(
    iutgc_t iusTGC,
    char *parentPath,
    hid_t handle
);

iutgc_t iusHLTGCLoad
(
    hid_t handle,
    char *parentPath
);


#endif //IUSLIBRARY_IUSHLTGCIMP_H
