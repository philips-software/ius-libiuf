//
// Created by Ruijzendaal on 31/07/2018.
//

#ifndef IUSLIBRARY_IUSHLSOURCEDICTIMP_H
#define IUSLIBRARY_IUSHLSOURCEDICTIMP_H

#include <hdf5.h>
#include <iusHLSourceDict.h>


int iusHLSourceDictSave
(
    iusd_t dict,
    const char *path,
    hid_t handle
);

iusd_t iusHLSourceDictLoad
(
    hid_t handle,
    const char *path
);


#endif //IUSLIBRARY_IUSHLSOURCEDICTIMP_H
