//
// Created by Ruijzendaal on 30/05/2018.
//

#ifndef IUSLIBRARY_IUSHLPULSEDICTIMP_H
#define IUSLIBRARY_IUSHLPULSEDICTIMP_H

#include <hdf5.h>
#include <iusPulseDict.h>


int iusPulseDictSave
(
    iupd_t dict,
    const char *path,
    hid_t handle
);

iupd_t iusPulseDictLoad
(
    hid_t handle,
    const char *path
);


#endif //IUSLIBRARY_IUSHLPULSEDICTIMP_H
