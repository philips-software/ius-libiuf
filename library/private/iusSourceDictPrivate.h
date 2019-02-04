//
// Created by Ruijzendaal on 31/07/2018.
//

#ifndef IUSLIBRARY_IUSHLSOURCEDICTIMP_H
#define IUSLIBRARY_IUSHLSOURCEDICTIMP_H

#include <hdf5.h>
#include <iusSourceDict.h>


int iusSourceDictSave
(
    iusd_t dict,
    hid_t handle
);

iusd_t iusSourceDictLoad
(
    hid_t handle
);


#endif //IUSLIBRARY_IUSHLSOURCEDICTIMP_H
