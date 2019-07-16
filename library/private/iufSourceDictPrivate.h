#ifndef IUFLIBRARY_IUFHLSOURCEDICTIMP_H
#define IUFLIBRARY_IUFHLSOURCEDICTIMP_H

#include <hdf5.h>
#include <iufSourceDict.h>


int iufSourceDictSave
(
    iusd_t dict,
    hid_t handle
);

iusd_t iufSourceDictLoad
(
    hid_t handle
);


#endif //IUFLIBRARY_IUFHLSOURCEDICTIMP_H
