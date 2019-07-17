#ifndef IUFLIBRARY_IUFHLTGCIMP_H
#define IUFLIBRARY_IUFHLTGCIMP_H

#include <iufTGC.h>
#include <hdf5.h>
#include <hdf5_hl.h>

int iufTGCSave
(
    iutgc_t tgc,
    hid_t handle
);

iutgc_t iufTGCLoad
(
    hid_t handle
);


#endif //IUFLIBRARY_IUFHLTGCIMP_H
