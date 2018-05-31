//
// Created by Ruijzendaal on 30/05/2018.
//

#ifndef IUSLIBRARY_IUSHLPULSEDICTIMP_H
#define IUSLIBRARY_IUSHLPULSEDICTIMP_H

#include <hdf5.h>
#include <iusHLPulseDict.h>

int iusHLPulseDictSave
(
    iupd_t dict,
    char *path,
    hid_t handle
);

iupd_t iusHLPulseDictLoad
(
    hid_t handle,
    char *path
);

#endif //IUSLIBRARY_IUSHLPULSEDICTIMP_H